#include "Map.h"

#define WHITE_COLOR 255;
#define GRAY_COLOR 128;
#define BLACK_COLOR 0;

Map::Map()
{
}

Map::Map(HamsterAPI::OccupancyGrid * map, int robotSize,
		 Location startLocation, Location destLocation,
		 double mapHeight, double mapWidth)
{
	this->map = *map;
	this->robotSize = robotSize;
	this->startLocation = startLocation;
	this->destLocation = destLocation;

	this->mapHeight = mapHeight;
	this->mapWidth = mapWidth;

	// Initialize the occupation map - a boolean matrix indicating for
	// each cell whether or not it is occupied
	occupationMap.resize(mapHeight);

	for (int i=0; i < mapHeight; i++)
	{
		(occupationMap.at(i)).resize(mapWidth);
	}

	InitCvMatFromMap();

	vector< vector<bool> > BlownOccupationMap = GetBlownOccupationMap();

	this->grid = Grid(BlownOccupationMap, mapResolutionInCm,
					  mapHeight, mapWidth,
					  startLocation, destLocation);
}

void Map::InitCvMatFromMap()
{
	mapResolutionInCm = map.getResolution() * 100;
	int blownResolutionInCm = robotSize / 2;

	blownRadius = blownResolutionInCm / (int) mapResolutionInCm;

	unsigned char pixelColor;

	originalCvMat = cv::Mat(mapHeight, mapWidth, CV_8UC1);
	blownCvMat = cv::Mat(
		mapHeight + blownRadius * 2,
		mapWidth + blownRadius * 2,
		CV_8UC1);

	// Initialize the original matrix according to the obstacles in the map
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			HamsterAPI::Cell currentCell = map.getCell(i, j);
			bool isCurrentCellOccupied = currentCell == CELL_OCCUPIED;

			if (currentCell == CELL_FREE)
			{
				pixelColor = WHITE_COLOR;
			}
			else
			{
				if (isCurrentCellOccupied)
				{
					pixelColor = BLACK_COLOR;
				}
				else
				{
					pixelColor = GRAY_COLOR;
				}
			}

			originalCvMat.at<unsigned char>(i, j) = pixelColor;
			(occupationMap.at(i)).at(j) = isCurrentCellOccupied;
		}
	}
}

vector< vector<bool> > Map::GetBlownOccupationMap()
{
	InitBlownCvMat();

	vector< vector<bool> > BlownOccupationMap;

	BlownOccupationMap.resize(mapHeight);

	for (int i=0; i < mapHeight; i++)
	{
		(BlownOccupationMap.at(i)).resize(mapWidth);
	}

	// Iterate through the center of the copy matrix and check for each cell if it is
	// in the radius of blownRadius around an obstacle. If it is - color it in black
	// and mark it as occupied in the occupationMap
	for (int i = blownRadius; i < mapHeight - blownRadius; i++)
	{
		for (int j = blownRadius; j < mapWidth - blownRadius; j++)
		{
			bool shouldInflateCurrCell = DoesCellHaveOccupiedNeighbor(i, j);

			if (shouldInflateCurrCell)
			{
				// Color the cell which is a neighbor of an occupied cell in the "radius" of the padding
				// in black
				blownCvMat.at<unsigned char>(i, j) = BLACK_COLOR;
			}

			(BlownOccupationMap.at(i)).at(j) = shouldInflateCurrCell;
		}
	}

	return BlownOccupationMap;
}

void Map::InitBlownCvMat()
{
	// Copy the original matrix into a matrix with a white frame in the width of numOfCellsInPadding
	for (int i = 0; i < blownCvMat.rows; i++)
	{
		for (int j = 0; j < blownCvMat.cols; j++)
		{
			bool isCurrentCellInFrame =
				((i < blownRadius) || (i >= mapWidth - blownRadius)) &&
				((j < blownRadius) || (j >= mapHeight - blownRadius));

			if (isCurrentCellInFrame)
			{
				// Color the frame of the copy matrix in white so that it wouldn't be
				// considered as an obstacle
				blownCvMat.at<unsigned char>(i, j) = WHITE_COLOR;
			}
			else
			{
				// Copy the original matrix into the center of the copy matrix
				blownCvMat.at<unsigned char>(i, j) = originalCvMat.at<unsigned char>(i, j);
			}
		}
	}
}

bool Map::DoesCellHaveOccupiedNeighbor(int rowIndex, int colIndex)
{
	for (int row = rowIndex - blownRadius; row <= rowIndex + blownRadius; row++)
	{
		for (int col = colIndex - blownRadius; col <= colIndex + blownRadius; col++)
		{
			// Skip the current cell
			if (row != rowIndex || col != colIndex)
			{
				bool isNeighborOccupied = map.getCell(row, col) == CELL_OCCUPIED;

				if (isNeighborOccupied)
				{
					return true;
				}
			}
		}
	}

	return false;
}

Map::~Map() {
	// TODO Auto-generated destructor stub
}
