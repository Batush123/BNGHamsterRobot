#ifndef CONFIGURATIONMANAGER_H_
#define CONFIGURATIONMANAGER_H_
#include <string>
#include <unordered_map>
#include "Location.h"

#define CONFIG_PATH "/home/user/BNGHamsterRobot/parameters.txt"

using namespace std;

class ConfigurationManager
{
private:
	double mapHeight, mapWidth, mapResolution;

	unordered_map<std::string, std::string> configMap;
	Location startLocation;
	Location goal;
	double robotRadius;

	float ReadFloatToken(const char* szKey);
	string ReadString(const char* szKey);
	int	ReadIntToken(const char* szKey, int nTokenNum);

public:
	ConfigurationManager();
	ConfigurationManager(double mapHeight, double mapWidth, double mapResolution);
	Location GetStartLocation();
	Location GetGoalLocation();
	int GetRobotRadiusInCm();
};

#endif /* CONFIGURATIONMANAGER_H_ */
