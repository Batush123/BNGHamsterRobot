/*
 * Map.h
 *
 *  Created on: Jul 1, 2017
 *      Author: user
 */

#ifndef MAP_H_
#define MAP_H_

#include "Grid.h"
#include <unistd.h>
#include <HamsterAPIClientCPP/Hamster.h>
using namespace std;
using namespace HamsterAPI;

class Map
{
private:
	OccupancyGrid map;
	double mapResolutionInCm;	/* Each OccupancyGrid cell takes an area of mapResolutionInCm^2 centimeters */
	cv::Mat originalCvMat;
	cv::Mat blownCvMat;
	int mapWidth;
	int mapHeight;
	int robotSize;
	Location startLocation;
	Location destLocation;
	void InitCvMatFromMap();
	void InitBlownCvMat();
	vector< vector<bool> > GetBlownOccupationMap();
	bool DoesCellHaveOccupiedNeighbor(int rowIndex, int colIndex);

public:
	vector< vector<bool> > occupationMap;
	Grid grid;
	int blownRadius;
	Map();
	Map(HamsterAPI::OccupancyGrid * map, int robotSize,
		Location startLocation, Location goalLocation,
		double mapHeight, double mapWidth);
	virtual ~Map();
};

#endif /* MAP_H_ */
