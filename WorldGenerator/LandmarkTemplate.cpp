// Created by Eric Marquez. All rights reserved

#include "LandmarkTemplate.h"
#include <random>
#include <ctime>

namespace WorldGenerator
{
	enum CellPosition
	{
		LEFT,
		RIGHT,
		TOP,
		BOTTOM,
		TRCORNER,
		TLCORNER,
		BRCORNER,
		BLCORNER,
	};

	// Define defaults
	LandmarkTemplate::LandmarkTemplate()
	{
		m_RowRange = std::make_pair(0, 0);
		m_ColumnRange = std::make_pair(0, 0);
		m_HeightRange = std::make_pair(0, 0);
		m_CellSet = nullptr;
		m_PassageWayCount = 1;
	}

	// Extra for ease of acess
	LandmarkTemplate::LandmarkTemplate(int minRowSize, int maxRowSize, int minColumnSize, int maxColumnSize, std::string cellSetName)
	{
		m_RowRange = std::make_pair(0, 0);
		m_ColumnRange = std::make_pair(0, 0);
		m_HeightRange = std::make_pair(0, 0);
		m_CellSet = nullptr;
		m_PassageWayCount = 1;

		SetDefaults(minRowSize, maxRowSize, minColumnSize, maxColumnSize, cellSetName);
	}

	//On destroy
	LandmarkTemplate::~LandmarkTemplate()
	{
		ClearInteractablesList();
	}

	// Generates an array for the current alloted space using a non-zero seed
	bool LandmarkTemplate::GenerateLandmark(WorldGrid& grid, unsigned int seed)
	{
		// Make sure the cell set is valid
		if (!m_CellSet)
			return false;

		// Make sure the size range is valid
		if(!m_RowRange.first || !m_RowRange.second || ! m_ColumnRange.first || !m_ColumnRange.second)
			return false;

		// Set seed if not set
		seed = (seed) ? seed : (unsigned)time(0);

		// Roll row and column values
		std::mt19937 rng(seed);

		// distribution in range [rowMin, rowMax]
		std::uniform_int_distribution<std::mt19937::result_type> rowDistribution(m_RowRange.first, m_RowRange.second);

		// distribution in range [columnMin, columnMax]
		std::uniform_int_distribution<std::mt19937::result_type> columnDistribution(m_ColumnRange.first, m_ColumnRange.second);

		// distribution in range [elevationMin, elevationMax]
		std::uniform_int_distribution<std::mt19937::result_type> heightDistribution(m_HeightRange.first, m_HeightRange.second);

		// distribution in range [exit1, exit4]
		std::uniform_int_distribution<std::mt19937::result_type> exit1CountDistribution(0, 1);
		std::uniform_int_distribution<std::mt19937::result_type> exit2CountDistribution(0, 1);
		std::uniform_int_distribution<std::mt19937::result_type> exit3CountDistribution(0, 1);
		std::uniform_int_distribution<std::mt19937::result_type> exit4CountDistribution(0, 1);

		int rowCount = rowDistribution(rng);
		int columnCount = rowDistribution(rng);
		int startDepth = heightDistribution(rng);
		int exit1 = exit1CountDistribution(rng);
		int exit2 = exit2CountDistribution(rng);
		int exit3 = exit3CountDistribution(rng);
		int exit4 = exit4CountDistribution(rng);

		// Get the correct cell type for the current settings
		std::vector<CellType> cellTypes = GetBorderTypes(startDepth);

		// Start generation process
		grid.resize(rowCount);
		for (int x = 0; x < rowCount; x++)
		{
			grid[x].resize(columnCount);
			for (int y = 0; y < columnCount; y++)
			{
				Cell current = m_CellSet->GetCell(CellType::Default);

				// top right corner
				if (x == 0 && y == 0)
				{
					current = m_CellSet->GetCell(cellTypes[TLCORNER]);
				}
				else if (x == rowCount - 1 && y == 0)
				{
					current = m_CellSet->GetCell(cellTypes[TRCORNER]);
				}
				else if (x == rowCount - 1 && y == columnCount - 1)
				{
					current = m_CellSet->GetCell(cellTypes[BRCORNER]);
				}
				else if (x == 0 && y == columnCount - 1)
				{
					current = m_CellSet->GetCell(cellTypes[BLCORNER]);
				}
				else if (x != rowCount / 2 && y != columnCount / 2 && !(x == (rowCount / 2) + 1 || x == (rowCount / 2) - 1) && !(y == (columnCount / 2) + 1 || y == (columnCount / 2) - 1))
				{
					if ((x == 0 || x == rowCount - 1))
					{
						current = m_CellSet->GetCell(cellTypes[LEFT]);
					}
					else if ((y == 0 || y == columnCount - 1))
					{
						current = m_CellSet->GetCell(cellTypes[BOTTOM]);
					}
				}
				else if (x == rowCount / 2 || x == (rowCount / 2) + 1 || x == (rowCount / 2) - 1)
				{
					if (!exit1 && y == 0)
					{
						current = m_CellSet->GetCell(cellTypes[TOP]);
					}
					else if (!exit2 && y == columnCount - 1)
					{
						current = m_CellSet->GetCell(cellTypes[BOTTOM]);
					}
					else
					{
						current = m_CellSet->GetCell(CellType::Default);
					}
				}
				else if (y == columnCount / 2 || y == (columnCount / 2) + 1 || y == (columnCount / 2) - 1)
				{
					if (!exit3 && x == 0)
					{
						current = m_CellSet->GetCell(cellTypes[LEFT]);
					}
					else if (!exit4 && x == rowCount - 1)
					{
						current = m_CellSet->GetCell(cellTypes[RIGHT]);
					}
					else
					{
						current = m_CellSet->GetCell(CellType::Default);
					}
				}

				grid[x][y] = current;
			}
		}

		return true;
	}

	// Sets the default settings for this Landmark
	void LandmarkTemplate::SetDefaults(int minRowSize, int maxRowSize, int minColumnSize, int maxColumnSize, std::string cellSetName)
	{
		SetColumnRange(minRowSize, maxRowSize);
		SetRowRange(minColumnSize, maxColumnSize);
		SetCellSet(cellSetName);
	}

	// Sets the current cell set
	void LandmarkTemplate::SetCellSet(std::string cellSet)
	{
		m_CellSet = CellSetLibrary::GetCellSet(cellSet);
	}

	// Sets the column range for this Landmark
	void LandmarkTemplate::SetColumnRange(int minColumnSize, int maxColumnSize)
	{
		m_ColumnRange = std::make_pair(minColumnSize, maxColumnSize);
	}

	// Sets the row range for this Landmark
	void LandmarkTemplate::SetRowRange(int minRowSize, int maxRowSize)
	{
		m_RowRange = std::make_pair(minRowSize, maxRowSize);
	}

	// Sets the elevation range for this Landmark
	void LandmarkTemplate::SetElevationRange(int minElevation, int maxElevation)
	{
		m_HeightRange = std::make_pair(minElevation, maxElevation);
	}

	// Adds a possible interactable to the Landmark
	void LandmarkTemplate::AddInteractable(const Interactable& object)
	{
		m_Interactables.push_back(new Interactable(object));
	}

	// Adds possible interactables to the Landmark
	void LandmarkTemplate::AddInteractables(const std::vector<Interactable>& objects)
	{
		for (const auto& obj : objects)
		{
			AddInteractable(obj);
		}
	}

	// Clears all possible interactables
	void LandmarkTemplate::ClearInteractablesList()
	{
		// Destroy all pointers first
		for (auto ptr : m_Interactables)
		{
			if (ptr)
			{
				delete ptr;
				ptr = nullptr;
			}
		}

		m_Interactables.clear();
	}

	// Gets the min row count
	int LandmarkTemplate::GetMinRowSize() const
	{
		return m_RowRange.first;
	}

	// Gets the max row count
	int LandmarkTemplate::GetMaxRowSize() const
	{
		return m_RowRange.second;
	}

	// Gets the min column count
	int LandmarkTemplate::GetMinColumnSize() const
	{
		return m_ColumnRange.first;
	}

	// Gets the max column count
	int LandmarkTemplate::GetMaxColumnSize() const
	{
		return m_ColumnRange.second;
	}

	int LandmarkTemplate::GetMinElevation() const
	{
		return m_HeightRange.first;
	}

	int LandmarkTemplate::GetMaxElevation() const
	{
		return m_HeightRange.second;
	}

	std::pair<int, int> LandmarkTemplate::GetRowRange() const
	{
		return m_RowRange;
	}

	std::pair<int, int> LandmarkTemplate::GetColumnRange() const
	{
		return m_ColumnRange;
	}

	// Gets an interactable at a given index
	Interactable* LandmarkTemplate::GetInteractable(unsigned int index)
	{
		if (m_Interactables.size() > index)
			return m_Interactables[index];

		return nullptr;
	}

	// Gets the exit positions generated for this Landmark
	const std::vector<std::pair<int, int>>& LandmarkTemplate::GetExitPositions() const
	{
		return m_ExitPositions;
	}

	// Gets all interactrables
	const std::vector<Interactable*>& LandmarkTemplate::GetInteractables() const
	{
		return m_Interactables;
	}

	// Gets the border information
	std::vector<CellType> LandmarkTemplate::GetBorderTypes(unsigned int elevation) const
	{
		std::vector<CellType> types;

		if (elevation == 0)
		{
			types.push_back(CellType::LeftWall);
			types.push_back(CellType::RightWall);
			types.push_back(CellType::TopWall);
			types.push_back(CellType::BottomWall);
			types.push_back(CellType::TRCornerWall);
			types.push_back(CellType::TLCornerWall);
			types.push_back(CellType::BRCornerWall);
			types.push_back(CellType::BLCornerWall);
		}
		else
		{
			types.push_back(CellType::LeftCliff);
			types.push_back(CellType::RightCliff);
			types.push_back(CellType::TopCliff);
			types.push_back(CellType::BottomCliff);
			types.push_back(CellType::TRCornerCliff);
			types.push_back(CellType::TLCornerCliff);
			types.push_back(CellType::BRCornerCliff);
			types.push_back(CellType::BLCornerCliff);
		}

		return types;
	}
}