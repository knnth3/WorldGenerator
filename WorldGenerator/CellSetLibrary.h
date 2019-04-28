#pragma once
// Created by Eric Marquez. All rights reserved

#include "Cell.h"

namespace WorldGenerator
{
	class CellSetLibrary
	{
	public:

		~CellSetLibrary();

		static CellSet* GetCellSet(std::string uniqueName);

		static bool CreateCellSet(std::string uniqueName, Cell defaultCell);

		static bool CreateCellSet(std::string uniqueName, Cell defaultCell, std::vector<Cell> cells);

		static void RemoveAllSets();

	private:

		static std::map<std::string, CellSet*> m_CellSets;
	};
}
