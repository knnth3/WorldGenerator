#pragma once
// Created by Eric Marquez. All rights reserved

namespace WorldGenerator
{
	class Interactable
	{
	public:
		bool bCanOverlap;
		int NumMaxSpawns;
		int NumRequiredSpawns;
		int RowCount;
		int ColumnCount;
	};

}
