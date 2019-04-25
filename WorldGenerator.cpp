// Created by Eric Marquez. All rights reserved

#include <iostream>
#include "LandmarkTemplate.h"

using namespace WorldGenerator;
using namespace std;

void PrintCell(CellType type);

int main()
{
	// Create a cell set
	if (CellSetLibrary::CreateCellSet("grasslands", Cell(0, true, CellType::Ground)))
	{
		int seed = 0;
		CellGrid Grid;
		LandmarkTemplate landmark(10, 20, 10, 20, "grasslands");
		if (landmark.GenerateLandmark(Grid, seed))
		{
			cout << "-----Landmark: [" << Grid.RowCount() << ", " << Grid.ColumnCount() << "]-----" << endl;

			for (int y = 0; y < Grid.ColumnCount(); y++)
			{
				for (int x = 0; x < Grid.RowCount(); x++)
				{
					PrintCell(Grid[x][y].Type);
				}

				cout << endl;
			}
		}
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
	case WorldGenerator::CellType::BackWall:
		cout << "-";
		break;
	case WorldGenerator::CellType::TRCornerWall:
		cout << "\\";
		break;
	case WorldGenerator::CellType::TLCorcerWall:
		cout << "/";
		break;
	case WorldGenerator::CellType::BRCornerWall:
		cout << "/";
		break;
	case WorldGenerator::CellType::BLCornerWall:
		cout << "\\";
		break;
	case WorldGenerator::CellType::LeftCliff:
		break;
	case WorldGenerator::CellType::RightCliff:
		break;
	case WorldGenerator::CellType::TopCliff:
		break;
	case WorldGenerator::CellType::BottmCliff:
		break;
	case WorldGenerator::CellType::TRCornerCliff:
		break;
	case WorldGenerator::CellType::TLCornerCliff:
		break;
	case WorldGenerator::CellType::BRCornerCliff:
		break;
	case WorldGenerator::CellType::BLCornerCliff:
		break;
	default:
		break;
	}
}
