// Created by Eric Marquez. All rights reserved

#include <iostream>
#include "Generator.h"

using namespace WorldGenerator;
using namespace std;

void PrintGrid(WorldGrid& grid);
void PrintCell(CellType type);

int main()
{
	// Create a cell set
	if (CellSetLibrary::CreateCellSet("grasslands", Cell(0, true, CellType::Ground)))
	{
		int seed = 0;
		WorldGrid grid;
		//LandmarkTemplate landmark(10, 20, 10, 20, "grasslands");
		//if (landmark.GenerateLandmark(Grid, seed))
		//{
		//	cout << "-----Landmark: [" << Grid.RowCount() << ", " << Grid.ColumnCount() << "]-----" << endl;

		//}

		Generator myGenerator("grasslands");
		if (myGenerator.GenerateMap(grid, seed))
		{
			PrintGrid(grid);
		}
	}
}

void PrintGrid(WorldGrid& grid)
{
	for (int y = grid.ColumnCount() - 1; y >= 0; y--)
	{
		for (int x = 0; x < grid.RowCount(); x++)
		{
			PrintCell(grid[x][y].Type);
		}

		cout << endl;
	}
}

void PrintCell(CellType type)
{
	switch (type)
	{
	case WorldGenerator::CellType::Ground:
		cout << ".";
		break;
	case WorldGenerator::CellType::LeftWall:
		cout << "|";
		break;
	case WorldGenerator::CellType::RightWall:
		cout << "|";
		break;
	case WorldGenerator::CellType::TopWall:
		cout << "-";
		break;
	case WorldGenerator::CellType::BottomWall:
		cout << "-";
		break;
	case WorldGenerator::CellType::TRCornerWall:
		cout << "\\";
		break;
	case WorldGenerator::CellType::TLCornerWall:
		cout << "/";
		break;
	case WorldGenerator::CellType::BRCornerWall:
		cout << "/";
		break;
	case WorldGenerator::CellType::BLCornerWall:
		cout << "\\";
		break;
	default:
		cout << " ";
		break;
	}
}
