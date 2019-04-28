// Created by Eric Marquez. All rights reserved

#include "Generator.h"
#include <ctime>


namespace WorldGenerator
{
	enum Direction
	{
		FORWARD = 0,
		LEFT,
		RIGHT
	};

	Generator::Generator(std::string cellSetName)
	{
		m_CurrentCellSet = CellSetLibrary::GetCellSet(cellSetName);
		m_MaxWalkers = 4;
		m_Magnification = std::make_pair(1, 1);
		m_MaxPathLenth = 20;
		m_PathDivergenceRate = 30;
		m_MapDimensions = std::make_pair(100,100);
	}

	bool Generator::GenerateMap(WorldGrid& grid, std::pair<int, int> start, unsigned int seed)
	{
		if (!m_CurrentCellSet)
			return false;

		// Minimum 3x3 units
		if(m_MapDimensions.first < 3 && m_MapDimensions.second < 3)
			return false;

		// Setup grid
		grid.resize(m_MapDimensions.first);
		for (int x = 0; x < m_MapDimensions.first; x++)
		{
			grid.at(x).resize(m_MapDimensions.second);
		}

		std::vector<Walker*> walkers;
		walkers.emplace_back(new Walker(grid, m_MaxPathLenth, m_MapDimensions, start, std::make_pair(0, 1), m_CurrentCellSet));
		grid[start.first][start.second] = m_CurrentCellSet->GetCell(CellType::Ground);
		grid[start.first + 1][start.second] = m_CurrentCellSet->GetCell(CellType::Ground);
		grid[start.first - 1][start.second] = m_CurrentCellSet->GetCell(CellType::Ground);

		// Set seed if not set
		seed = (seed) ? seed : (unsigned)time(0);

		std::mt19937 rng(seed);
		std::uniform_int_distribution<std::mt19937::result_type> createWalkerRoll(0,1);

		// Set deault min and max
		std::pair<int, int> rowRange = std::make_pair(start.first, start.first);
		std::pair<int, int> columnRange = std::make_pair(start.second, start.second);

		bool bRunning = true;
		int inactiveCount = 0;
		while (bRunning)
		{
			unsigned int size = (unsigned int) walkers.size();
			for (unsigned int index = 0; index < size; index++)
			{
				Walker* walker = walkers[index];

				if (!walker)
					continue;

				if (!walker->Update(rng, rowRange, columnRange, m_PathDivergenceRate))
				{
					inactiveCount++;
					delete walker;
					walkers[index] = nullptr;
				}
				else
				{
					if (m_MaxWalkers > walkers.size() && createWalkerRoll(rng))
					{
						walkers.emplace_back(new Walker(grid, m_MaxPathLenth, m_MapDimensions, walker->GetLocaton(), walker->GetDirection(), m_CurrentCellSet));
					}
				}
			}

			if (inactiveCount == walkers.size())
				bRunning = false;
		}

		// Add padding to map
		rowRange.first = clamp(rowRange.first - 1, 0, rowRange.first);
		rowRange.second = clamp(rowRange.second + 1, rowRange.second, m_MapDimensions.first - 1);
		columnRange.first = clamp(columnRange.first - 1, 0, columnRange.first);
		columnRange.second = clamp(columnRange.second + 1, columnRange.second, m_MapDimensions.second - 1);

		ProcessResults(grid, rowRange, columnRange);

		return true;
	}

	void Generator::SetMapSize(int rows, int columns)
	{
		m_MapDimensions = std::make_pair(rows, columns);
	}

	void Generator::SetMaxPathLength(int length)
	{
		m_MaxPathLenth = length;
	}

	void Generator::SetMaxWalkers(int count)
	{
		m_MaxWalkers = count;
	}

	void Generator::SetPathDivergencePercent(int percent)
	{
		m_PathDivergenceRate = percent;
	}

	void Generator::SetMagnification(unsigned int x, unsigned int y)
	{
		m_Magnification = std::make_pair(x, y);
	}

	std::pair<int, int> Generator::GetMagnification() const
	{
		return m_Magnification;
	}

	int Generator::GetPathDivergencePercent() const
	{
		return m_PathDivergenceRate;
	}

	int Generator::GetMaxWalkers() const
	{
		return m_MaxWalkers;
	}

	int Generator::GetMaxPathLength() const
	{
		return m_MaxPathLenth;
	}

	int Generator::GetMapColumns() const
	{
		return m_MapDimensions.second;
	}

	int Generator::GetMapRows() const
	{
		return m_MapDimensions.first;
	}

	void Generator::Duplicate(WorldGrid& grid, Cell cell, std::pair<int, int> count, std::pair<int, int> originalLocation)
	{
		for (int x = 0; x < count.first; x++)
		{
			for (int y = 0; y < count.second; y++)
			{
				grid[(originalLocation.first * count.first) + x][((size_t)originalLocation.second * count.second) + y] = cell;
			}
		}
	}

	void Generator::ProcessResults(WorldGrid& grid, std::pair<int, int>& rowRange, std::pair<int, int>& columnRange)
	{
		WorldGrid newGrid;
		newGrid.resize((rowRange.second - rowRange.first) * m_Magnification.first);
		for (unsigned int x = 0; x < newGrid.RowCount(); x++)
		{
			newGrid.at(x).resize(((size_t)columnRange.second - columnRange.first) * m_Magnification.second);
		}

		int rowOffset = 0;
		for (int x = rowRange.first; x < rowRange.second; x++)
		{
			int columnOffset = 0;
			for (int y = columnRange.first; y < columnRange.second; y++)
			{
				Duplicate(newGrid, grid[x][y], m_Magnification, std::pair<int, int>(x - rowRange.first, y - columnRange.first));
			}
		}

		grid = newGrid;
	}

	int Generator::clamp(int num, int min, int max)
	{
		return std::max(min, std::min(num, max));
	}

	Generator::Walker::Walker(WorldGrid& grid, int maxLength, std::pair<int, int> dimensions, std::pair<int, int> location, std::pair<int, int> direction, CellSet* cellSet)
	{
		m_CellSet = cellSet;
		m_MaxLength = maxLength;
		m_Grid = &grid;
		m_Location = location;
		m_Dimensions = dimensions;
		m_CurrentPathLength = 0;
		m_Forward = direction;
	}

	bool Generator::Walker::Update(std::mt19937& rng, std::pair<int, int>& rowRange, std::pair<int, int>& columnRange, int strayPercentage)
	{
		if (!m_Grid)
			return false;

		if(m_CurrentPathLength > m_MaxLength)
			return false;

		bool bFrontBlocked = false;
		auto directions = GetAvailableDirections(bFrontBlocked);
		if (directions.size() <= 0)
			return false;

		// Roll direction
		std::uniform_int_distribution<std::mt19937::result_type> changeDirectionRoll(0, 100);
		if (bFrontBlocked || (directions.size() > 1 && (unsigned int)strayPercentage >= changeDirectionRoll(rng)))
		{
			int offset = bFrontBlocked ? 0 : 1;
			std::uniform_int_distribution<std::mt19937::result_type> moveWalkerRoll(offset, (int)directions.size() - 1);
			int dirIndex = moveWalkerRoll(rng);
			auto direction = GetDirectionVector(directions[dirIndex]);
			m_Forward = direction;
		}

		// move walker
		m_CurrentPathLength++;
		m_Location.first += m_Forward.first;
		m_Location.second += m_Forward.second;

		// Fill in path
		m_Grid->at(m_Location.first).at(m_Location.second) = m_CellSet->GetCell(CellType::Ground);
		m_Grid->at(m_Location.first - m_Forward.second).at((size_t)m_Location.second - m_Forward.first) = m_CellSet->GetCell(CellType::Ground);
		m_Grid->at(m_Location.first + m_Forward.second).at((size_t)m_Location.second + m_Forward.first) = m_CellSet->GetCell(CellType::Ground);

		// Record min and max
		rowRange.first = (m_Location.first < rowRange.first) ? m_Location.first : rowRange.first;
		rowRange.second = (m_Location.first > rowRange.second) ? m_Location.first : rowRange.second;
		columnRange.first = (m_Location.second < columnRange.first) ? m_Location.second : columnRange.first;
		columnRange.second = (m_Location.second > columnRange.second) ? m_Location.second : columnRange.second;

		return true;
	}

	std::pair<int, int> Generator::Walker::GetLocaton() const
	{
		return m_Location;
	}

	std::pair<int, int> Generator::Walker::GetDirection() const
	{
		return m_Forward;
	}

	std::vector<unsigned int> Generator::Walker::GetAvailableDirections(bool& frontBlocked)
	{
		std::vector<unsigned int> directions;

		bool xForward = (m_Location.first + m_Forward.first >= 0 && m_Location.first + m_Forward.first < m_Dimensions.first);
		bool yForward = (m_Location.second + m_Forward.second >= 0 && m_Location.second + m_Forward.second < m_Dimensions.second);

		if (xForward && yForward)
		{
			bool xBackward = (m_Location.first - m_Forward.first >= 0 && m_Location.first - m_Forward.first < m_Dimensions.first);
			bool yBackward = (m_Location.second - m_Forward.second >= 0 && m_Location.second - m_Forward.second < m_Dimensions.second);
			directions.push_back(FORWARD);

			if (xBackward && yBackward)
			{
				bool xRight = (m_Location.first + m_Forward.second >= 0 && m_Location.first + m_Forward.second < m_Dimensions.first);
				bool yRight = (m_Location.second + m_Forward.first >= 0 && m_Location.second + m_Forward.first < m_Dimensions.second);
				if (xRight && yRight)
				{
					directions.push_back(RIGHT);
				}

				bool xLeft = (m_Location.first - m_Forward.second >= 0 && m_Location.first - m_Forward.second < m_Dimensions.first);
				bool yLeft = (m_Location.second - m_Forward.first >= 0 && m_Location.second - m_Forward.first < m_Dimensions.second);

				if (xForward && yForward)
				{
					directions.push_back(LEFT);
				}
			}
		}
		else
		{
			int options = 0;
			std::pair<int, int> tempLocation = std::make_pair(m_Location.first - m_Forward.first, m_Location.second - m_Forward.second);
			bool xBackward = (tempLocation.first - m_Forward.first >= 0 && tempLocation.first - m_Forward.first < m_Dimensions.first);
			bool yBackward = (tempLocation.second - m_Forward.second >= 0 && tempLocation.second - m_Forward.second < m_Dimensions.second);

			if (xBackward && yBackward)
			{
				bool xRight = (tempLocation.first + m_Forward.second >= 0 && tempLocation.first + m_Forward.second < m_Dimensions.first);
				bool yRight = (tempLocation.second + m_Forward.first >= 0 && tempLocation.second + m_Forward.first < m_Dimensions.second);
				if (xRight && yRight)
				{
					directions.push_back(RIGHT);
					options++;
				}

				bool xLeft = (tempLocation.first - m_Forward.second >= 0 && tempLocation.first - m_Forward.second < m_Dimensions.first);
				bool yLeft = (tempLocation.second - m_Forward.first >= 0 && tempLocation.second - m_Forward.first < m_Dimensions.second);

				if (xForward && yForward)
				{
					directions.push_back(LEFT);
					options++;
				}

				if (options > 0)
				{
					frontBlocked = true;
					m_Location = tempLocation;
				}
			}
		}

		return directions;
	}

	std::pair<int, int> Generator::Walker::GetDirectionVector(unsigned int direction)
	{
		switch (direction)
		{
		case WorldGenerator::LEFT:
			return std::pair<int, int>(-m_Forward.second, -m_Forward.first);
		case WorldGenerator::RIGHT:
			return std::pair<int, int>(m_Forward.second, m_Forward.first);
		}

		return m_Forward;
	}
}
