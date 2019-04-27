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
		m_MaxPathLenth = 10;
		m_MapDimensions = std::make_pair(100,100);
	}

	bool Generator::GenerateMap(WorldGrid& grid, unsigned int seed)
	{
		if (!m_CurrentCellSet)
			return false;

		// Minimum 100 units each side
		if(m_MapDimensions.first < 100 && m_MapDimensions.second < 100)
			return false;

		std::pair<int, int> start = std::make_pair(30, 30);
		std::vector<Walker*> walkers;
		walkers.emplace_back(new Walker(grid, m_MaxPathLenth, m_MapDimensions, start, std::make_pair(0, 1), m_CurrentCellSet));

		// Set seed if not set
		seed = (seed) ? seed : (unsigned)time(0);

		std::mt19937 rng(seed);
		std::uniform_int_distribution<std::mt19937::result_type> createWalkerRoll(0,1);

		bool bRunning = true;
		int inactiveCount = 0;
		while (bRunning)
		{
			int size = walkers.size();
			for (int index = 0; index < size; index++)
			{
				Walker* walker = walkers[index];

				if (!walker)
					continue;

				if (!walker->Update(rng))
				{
					inactiveCount++;
				}
				else
				{
					if(m_MaxWalkers < walkers.size() && createWalkerRoll(rng))
						walkers.emplace_back(new Walker(grid, m_MaxPathLenth, m_MapDimensions, walker->GetLocaton(), walker->GetDirection(), m_CurrentCellSet));
				}
			}

			if (inactiveCount == walkers.size())
				bRunning = false;
		}

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

	bool Generator::Walker::Update(std::mt19937& rng)
	{
		if (!m_Grid)
			return false;

		if(m_CurrentPathLength > m_MaxLength)
			return false;

		auto directions = GetAvailableDirections();
		if (directions.size() <= 0)
			return false;

		// Start generation process
		m_Grid->resize(m_Dimensions.first);
		for (int x = 0; x < m_Dimensions.first; x++)
		{
			m_Grid->at(x).resize(m_Dimensions.second);
		}

		std::uniform_int_distribution<std::mt19937::result_type> moveWalkerRoll(0, directions.size() - 1);

		int dirIndex = moveWalkerRoll(rng);
		auto direction = GetDirectionVector(directions[dirIndex]);
		m_Forward = direction;

		m_Location.first += m_Forward.first;
		m_Location.second += m_Forward.second;

		m_Grid->at(m_Location.first).at(m_Location.second) = m_CellSet->GetCell(CellType::Ground);
		m_Grid->at(m_Location.first - m_Forward.second).at(m_Location.second - m_Forward.first) = m_CellSet->GetCell(CellType::Ground);
		m_Grid->at(m_Location.first + m_Forward.second).at(m_Location.second + m_Forward.first) = m_CellSet->GetCell(CellType::Ground);

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

	std::vector<unsigned int> Generator::Walker::GetAvailableDirections()
	{
		std::vector<unsigned int> directions;

		bool xForward = (m_Location.first + m_Forward.first >= 0 && m_Location.first + m_Forward.first < m_Dimensions.first);
		bool yForward = (m_Location.second + m_Forward.second >= 0 && m_Location.second + m_Forward.second < m_Dimensions.second);
		bool xBackward = (m_Location.first - m_Forward.first >= 0 && m_Location.first - m_Forward.first < m_Dimensions.first);
		bool yBackward = (m_Location.second - m_Forward.second >= 0 && m_Location.second - m_Forward.second < m_Dimensions.second);

		if (xForward && yForward)
		{
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
