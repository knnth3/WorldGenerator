#pragma once
// Created by Eric Marquez. All rights reserved

#include <vector>
#include <map>
#include <iostream>

namespace WorldGenerator
{
	// Defines the type of cells that the Cell struct can represent
	enum class CellType
	{
		Ground = 0,
		LeftWall,
		RightWall,
		TopWall,
		BackWall,
		TRCornerWall,
		TLCorcerWall,
		BRCornerWall,
		BLCornerWall,
		LeftCliff,
		RightCliff,
		TopCliff,
		BottmCliff,
		TRCornerCliff,
		TLCornerCliff,
		BRCornerCliff,
		BLCornerCliff,
		Interactable,
		Default,
	};

	// Basic struct to hold cell information
	struct Cell
	{
		Cell() = default;
		Cell(int depth, bool passable, CellType type)
		{
			Depth = depth;
			Passable = passable;
			Type = type;
		}

		int Depth;
		bool Passable;
		CellType Type;
	};

	class CellSet
	{
	public:
		CellSet(Cell defaultCell)
		{
			m_DefaultCell = defaultCell;
		}

		CellSet(Cell defaultCell, std::vector<Cell> cells)
		{
			m_DefaultCell = defaultCell;
			SetCellArray(cells);
		}

		void SetDefaultCell(Cell defaultCell)
		{
			m_DefaultCell = defaultCell;
		}

		void SetCellArray(std::vector<Cell>& arr)
		{
			m_Cells = arr;
		}

		Cell GetDefaultCell()
		{
			return m_DefaultCell;
		}

		Cell GetCell(CellType type)
		{
			if(m_Cells.size() > (unsigned int)type)
				return m_Cells[(unsigned int)type];

			return m_DefaultCell;
		}

	private:

		Cell m_DefaultCell;
		std::vector<Cell> m_Cells;
	};

	class CellSetLibrary
	{
	public:

		~CellSetLibrary()
		{
			std::cout << "Calling destructor" << std::endl;
			RemoveAllSets();
		}

		static CellSet* GetCellSet(std::string uniqueName)
		{
			// Found
			if (m_CellSets.find(uniqueName) != m_CellSets.end())
			{
				return m_CellSets[uniqueName];
			}

			return nullptr;
		}

		static bool CreateCellSet(std::string uniqueName, Cell defaultCell)
		{
			return CreateCellSet(uniqueName, defaultCell, std::vector<Cell>());
		}

		static bool CreateCellSet(std::string uniqueName, Cell defaultCell, std::vector<Cell> cells)
		{
			// Found
			if (m_CellSets.find(uniqueName) != m_CellSets.end())
			{
				return false;
			}

			m_CellSets[uniqueName] = new CellSet(defaultCell, cells);

			return true;
		}

		static void RemoveAllSets()
		{
			for (auto& set : m_CellSets)
			{
				delete set.second;
			}

			m_CellSets.clear();
		}

	private:

		static std::map<std::string, CellSet*> m_CellSets;
	};

	class CellGrid
	{
	public:
		int RowCount()const
		{
			return m_Grid.size();
		}

		int ColumnCount()const
		{
			if (m_Grid.size() > 0)
				return m_Grid[0].size();

			return 0;
		}

		void resize(int count)
		{
			m_Grid.resize(count);
		}

		std::vector<Cell>& at(int x)
		{
			return m_Grid[x];
		}

		std::vector<Cell>& operator[](int x)
		{
			return m_Grid[x];
		}

	private:
		std::vector<std::vector<Cell>> m_Grid;
	};
}