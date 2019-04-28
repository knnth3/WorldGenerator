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
		BottomWall,
		TRCornerWall,
		TLCornerWall,
		BRCornerWall,
		BLCornerWall,
		LeftCliff,
		RightCliff,
		TopCliff,
		BottomCliff,
		TRCornerCliff,
		TLCornerCliff,
		BRCornerCliff,
		BLCornerCliff,

		CELL_TYPE_COUNT,
		Interactable,
		Default,
		Empty,
	};

	// Basic struct to hold cell information
	struct Cell
	{
		Cell()
		{
			Depth = 0;
			Passable = false;
			Type = CellType::Empty;
		}

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
			GenerateGenericSet();
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
		void GenerateGenericSet()
		{
			m_Cells.clear();
			Cell newCell;

			for (int index = 0; index < (unsigned int)CellType::CELL_TYPE_COUNT; index++)
			{
				newCell.Type = (CellType)index;
				newCell.Depth = 0;

				if (newCell.Type == CellType::Ground)
					newCell.Passable = true;
				else
					newCell.Passable = false;

				if (index >= (unsigned int)CellType::LeftCliff)
					newCell.Depth = newCell.Depth + 1;
				
				m_Cells.push_back(newCell);
			}
		}

		Cell m_DefaultCell;
		std::vector<Cell> m_Cells;
	};

	class WorldGrid
	{
	public:
		unsigned int RowCount()const
		{
			return (unsigned int)m_Grid.size();
		}

		unsigned int ColumnCount()const
		{
			if (m_Grid.size() > 0)
				return (unsigned int)m_Grid[0].size();

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