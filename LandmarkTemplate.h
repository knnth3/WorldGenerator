#pragma once
// Created by Eric Marquez. All rights reserved

#include "CellSetLibrary.h"
#include "Interactable.h"

namespace WorldGenerator
{
	class LandmarkTemplate
	{
	public:
		LandmarkTemplate();
		LandmarkTemplate(int minRowSize, int maxRowSize, int minColumnSize, int maxColumnSize, std::string cellSetName = "");
		~LandmarkTemplate();

		// Generates an array for the current alloted space using a non-zero seed
		bool GenerateLandmark(CellGrid& grid, unsigned int seed = 0);

		// Sets the current cell set
		void SetCellSet(std::string cellSet);

		// Sets the default settings for this Landmark
		void SetDefaults(int minRowSize, int maxRowSize, int minColumnSize, int maxColumnSize, std::string cellSetName = "");

		// Sets the column range for this Landmark
		void SetColumnRange(int minColumnSize, int maxColumnSize);

		// Sets the row range for this Landmark
		void SetRowRange(int minRowSize, int maxRowSize);

		// Sets the elevation range for this Landmark
		void SetElevationRange(int minElevation, int maxElevation);

		// Adds a possible interactable to the Landmark
		void AddInteractable(const Interactable& object);

		// Adds possible interactables to the Landmark
		void AddInteractables(const std::vector<Interactable>& objects);

		// Clears all possible interactables
		void ClearInteractablesList();

		// Gets the min row count
		int GetMinRowSize() const;

		// Gets the max row count
		int GetMaxRowSize() const;

		// Gets the min column count
		int GetMinColumnSize() const;

		// Gets the max column count
		int GetMaxColumnSize() const;

		// Gets the min column count
		int GetMinElevation() const;

		// Gets the max column count
		int GetMaxElevation() const;

		// Gets the range of the row
		std::pair<int, int> GetRowRange()const;

		// Gets the range of the column
		std::pair<int, int> GetColumnRange()const;

		// Gets an interactable at a given index
		Interactable* GetInteractable(unsigned int index);

		// Gets the exit positions generated for this Landmark
		const std::vector<std::pair<int, int>>& GetExitPositions()const;

		// Gets all interactrables
		const std::vector<Interactable*>& GetInteractables()const;

	private:

		// Gets the border information
		std::vector<CellType> GetBorderTypes(unsigned int elevation)const;

		int m_PassageWayCount;
		CellSet* m_CellSet;
		std::pair<int, int> m_RowRange;
		std::pair<int, int> m_ColumnRange;
		std::pair<int, int> m_HeightRange;
		std::vector<Interactable*> m_Interactables;
		std::vector<std::pair<int, int>>m_ExitPositions;
	};
}