// Created by Eric Marquez. All rights reserved

#include <iostream>
#include "Generator.h"

using namespace WorldGenerator;
using namespace std;

void PrintGrid(WorldGrid& grid);
char GetCellSymbol(CellType type);

int main()
{
	// Create a cell set
	if (CellSetLibrary::CreateCellSet("grasslands", Cell(0, true, CellType::Ground)))
	{
		//LandmarkTemplate landmark(10, 20, 10, 20, "grasslands");
		//if (landmark.GenerateLandmark(Grid, seed))
		//{
		//	cout << "-----Landmark: [" << Grid.RowCount() << ", " << Grid.ColumnCount() << "]-----" << endl;

		//}
		 
		int seed = 0;
		WorldGrid grid;
		Generator myGenerator("grasslands");
		myGenerator.SetMaxWalkers(5);
		myGenerator.SetMaxPathLength(1000);
		myGenerator.SetPathDivergencePercent(60);
		myGenerator.SetMapSize(1000, 1000);
		myGenerator.SetMagnification(2, 2);

		std::pair<int,int> startPosition = std::make_pair(500, 500);
		if (myGenerator.GenerateMap(grid, startPosition, seed))
		{
			cout << "--- Generated World [" << grid.RowCount() << ", " << grid.ColumnCount()<< "] ---" << endl;
			PrintGrid(grid);
		}
	}
}

void PrintGrid(WorldGrid& grid)
{
	std::string gridStr;
	unsigned int ymax = (int)grid.ColumnCount() - 1;
	for (int y = ymax; y >= 0; y--)
	{
		for (int x = 0; x < (int)grid.RowCount(); x++)
		{
			gridStr += GetCellSymbol(grid[x][y].Type);
		}

		gridStr += '\n';
	}

	cout << gridStr;
}

char GetCellSymbol(CellType type)
{
	switch (type)
	{
	case WorldGenerator::CellType::Ground:
		return '.';
	case WorldGenerator::CellType::LeftWall:
	case WorldGenerator::CellType::RightWall:
		return '|';
	case WorldGenerator::CellType::TopWall:
	case WorldGenerator::CellType::BottomWall:
		return '-';
	case WorldGenerator::CellType::TRCornerWall:
	case WorldGenerator::CellType::BLCornerWall:
		return '\\';
	case WorldGenerator::CellType::TLCornerWall:
	case WorldGenerator::CellType::BRCornerWall:
		return '/';
	default:
		return ' ';
	}
}
