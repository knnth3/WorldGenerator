// Created by Eric Marquez. All rights reserved

#include "Generator.h"

namespace WorldGenerator
{
	Generator::Generator()
	{
		m_MaxWalkers = 4;
		m_MaxPathLenth = 10;
		m_MapDimensions = std::make_pair(100,100);
	}

	bool Generator::GenerateMap(CellGrid& grid, unsigned int seed)
	{
		// Minimum 100 units each side
		if(m_MapDimensions.first < 100 && m_MapDimensions.second < 100)
			return false;

		std::pair<int, int> start = std::make_pair(1, 0);
		std::vector<Walker*> walkers;
		walkers.emplace_back(new Walker(grid, m_MaxPathLenth, m_MapDimensions, start));

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
						walkers.emplace_back(new Walker(grid, m_MaxPathLenth, m_MapDimensions, walker->GetLocaton()));
				}
			}

			if (inactiveCount == walkers.size())
				bRunning = false;
		}
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

	Generator::Walker::Walker(CellGrid& grid, int maxLength, std::pair<int, int> dimensions, std::pair<int, int> location)
	{
		m_MaxLength = maxLength;
		m_Grid = &grid;
		m_Location = location;
		m_Dimensions = dimensions;
		m_CurrentPathLength = 0;
	}

	bool Generator::Walker::Update(std::mt19937& rng)
	{
		if(m_CurrentPathLength > m_MaxLength)
			return false;

		int possiblePaths = 0;
		bool xUp = (m_Location.first + 1 >= 0 && m_Location.first + 1 <= m_Dimensions.first);

		if (xUp)
		{

		}

		bool xDown = (m_Location.first - 1 >= 0 && m_Location.first - 1 <= m_Dimensions.first);
		bool yUp = (m_Location.second + 1 >= 0 && m_Location.second + 1 <= m_Dimensions.second);

		if (xUp && yUp)
			possiblePaths++;

		return true;
	}

	std::pair<int, int> Generator::Walker::GetLocaton() const
	{
		return m_Location;
	}
}
