#pragma once
// Created by Eric Marquez. All rights reserved

#include "LandmarkTemplate.h"
#include <random>

namespace WorldGenerator
{
	class Generator
	{
	public:
		Generator();

		bool GenerateMap(CellGrid& grid, unsigned int seed = 0);

		void SetMapSize(int rows, int columns);
		void SetMaxPathLength(int length);

		int GetMaxPathLength()const;
		int GetMapColumns()const;
		int GetMapRows()const;

	private:

		class Walker
		{
		public:
			Walker(CellGrid& grid, int maxLength, std::pair<int, int> dimensions, std::pair<int, int> location);
			bool Update(std::mt19937& rng);
			std::pair<int, int> GetLocaton()const;

		private:

			int m_MaxLength;
			int m_CurrentPathLength;
			CellGrid* m_Grid;
			std::pair<int, int> m_Location;
			std::pair<int, int> m_Dimensions;

		};
		
		int m_MaxWalkers;
		int m_MaxPathLenth;
		std::pair<int, int> m_MapDimensions;
	};
}

