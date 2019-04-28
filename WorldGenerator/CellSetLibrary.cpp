// Created by Eric Marquez. All rights reserved

#include "CellSetLibrary.h"

namespace WorldGenerator
{
	std::map<std::string, CellSet*> CellSetLibrary::m_CellSets = std::map<std::string, CellSet*>();

	CellSetLibrary::~CellSetLibrary()
	{
		RemoveAllSets();
	}


	CellSet* CellSetLibrary::GetCellSet(std::string uniqueName)
	{
		// Found
		if (m_CellSets.find(uniqueName) != m_CellSets.end())
		{
			return m_CellSets[uniqueName];
		}

		return nullptr;
	}


	bool CellSetLibrary::CreateCellSet(std::string uniqueName, Cell defaultCell)
	{
		// Found
		if (m_CellSets.find(uniqueName) != m_CellSets.end())
		{
			return false;
		}

		m_CellSets[uniqueName] = new CellSet(defaultCell);

		return true;
	}


	bool CellSetLibrary::CreateCellSet(std::string uniqueName, Cell defaultCell, std::vector<Cell> cells)
	{
		// Found
		if (m_CellSets.find(uniqueName) != m_CellSets.end())
		{
			return false;
		}

		m_CellSets[uniqueName] = new CellSet(defaultCell, cells);

		return true;
	}


	void CellSetLibrary::RemoveAllSets()
	{
		for (auto& set : m_CellSets)
		{
			delete set.second;
		}

		m_CellSets.clear();
	}
}
