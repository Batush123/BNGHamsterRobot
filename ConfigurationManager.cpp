#include "ConfigurationManager.h"
#include "Globals.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>

using namespace std;

string ConfigurationManager::ReadString(const char* szKey)
{
	return (this->configMap[szKey]);
}

int ConfigurationManager::ReadIntToken(const char* szKey, int nTokenNum)
{
	string strValue = this->configMap[szKey];

	for (int i = 0; i < nTokenNum; i++) {
		int nIndex = strValue.find(" ");
		strValue = strValue.substr(nIndex + 1);
	}

	unsigned nNextSpace = strValue.find(" ");

	if (nNextSpace != string::npos) {
		strValue = strValue.substr(0, nNextSpace);
	}

	int nResult = stoi(strValue);

	return (nResult);
}

float ConfigurationManager::ReadFloatToken(const char* szKey)
{
	string strValue = this->configMap[szKey];

	return (stof(strValue));
}

// trim from start
static inline string &ltrim(string &s)
{
	s.erase(s.begin(),
			find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
	return s;
}

// trim from end
static inline string &rtrim(string &s)
{
	s.erase(
			find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
			s.end());
	return s;
}

// trim from both ends
static inline string &trim(string &s)
{
	return ltrim(rtrim(s));
}

ConfigurationManager::ConfigurationManager()
{
}

ConfigurationManager::ConfigurationManager(double mapHeight, double mapWidth, double mapResolution)
{
	this->mapHeight = mapHeight;
	this->mapWidth = mapWidth;
	this->mapResolution = mapResolution;

	fstream fsConfig;
	fsConfig.open(CONFIG_PATH, ios::in);

	string strLine;
	while (getline(fsConfig, strLine)) {
		int index = strLine.find(":");
		string key = strLine.substr(0, index);
		;
		string value = strLine.substr(index + 1);
		value = trim(value);
		this->configMap[key] = value;
	}

	fsConfig.close();

	// Read all configuration values
	this->startLocation.x = this->ReadIntToken("startLocation", 0);
	this->startLocation.y = this->ReadIntToken("startLocation", 1);
	this->startLocation.yaw = this->ReadIntToken("startLocation", 2);
	this->goal.x = this->ReadIntToken("goal", 0);
	this->goal.y = this->ReadIntToken("goal", 1);
	this->robotRadius = this->ReadIntToken("robotSize", 0);
}

Location ConfigurationManager::GetStartLocation()
{
	return this->startLocation;
}

Location ConfigurationManager::GetGoalLocation()
{
	return this->goal;
}

int ConfigurationManager::GetRobotRadiusInCm()
{
	return this->robotRadius;
}
