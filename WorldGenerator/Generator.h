#pragma once
// Created by Eric Marquez. All rights reserved

#include "LandmarkTemplate.h"
#include <random>

namespace WorldGenerator
{
	class Generator
	{
	public:
		Generator(std::string cellSetName);

		bool GenerateMap(WorldGrid& grid, std::pair<int, int> startPosition, unsigned int seed = 0);

		void SetMapSize(int rows, int columns);
		void SetMaxPathLength(int length);
		void SetMaxWalkers(int count);
		void SetPathDivergencePercent(int percent);
		void SetMagnification(unsigned int x, unsigned int y);

		std::pair<int, int> GetMagnification()const;
		int GetPathDivergencePercent()const;
		int GetMaxWalkers()const;
		int GetMaxPathLength()const;
		int GetMapColumns()const;
		int GetMapRows()const;

	private:
		void Duplicate(WorldGrid& grid, Cell cell, std::pair<int, int> count, std::pair<int, int> originalLocation);
		void ProcessResults(WorldGrid& grid, std::pair<int, int>& rowRange, std::pair<int, int>& columnRange);
		int clamp(int num, int min, int max);

		class Walker
		{
		public:
			Walker(WorldGrid& grid, int maxLength, std::pair<int, int> dimensions, std::pair<int, int> location, std::pair<int, int> direction, CellSet* cellSetName);
			bool Update(std::mt19937& rng, std::pair<int, int>& rowRange, std::pair<int, int>& columnRange, int StrayPercentage);
			std::pair<int, int> GetLocaton()const;
			std::pair<int, int> GetDirection()const;

		private:
			std::vector<unsigned int> GetAvailableDirections(bool& frontBlocked);
			std::pair<int, int> GetDirectionVector(unsigned int direction);

			int m_MaxLength;
			int m_CurrentPathLength;
			WorldGrid* m_Grid;
			CellSet* m_CellSet;
			std::pair<int, int> m_Forward;
			std::pair<int, int> m_Location;
			std::pair<int, int> m_Dimensions;

		};

		unsigned int m_PathDivergenceRate;
		unsigned int m_MaxWalkers;
		unsigned int m_MaxPathLenth;
		CellSet* m_CurrentCellSet;
		std::pair<int, int> m_MapDimensions;
		std::pair<int, int> m_Magnification;
	};
}

