// TLE is used to feed Artificial Satellites objects with data
// that is used to calculate their current position, TLE data comes from https://www.N2YO.com
// Author: Kuba Pawlowski

#pragma once
#include "ofMain.h"

class getTle : public ofThread
{
public:
	getTle();
	std::map<int, std::vector<std::string>>		getData();
	bool										good();
private:
	bool										isTleDataCurrent();
	void										getApiKey();
	void										getDefSatellites();
	void										threadedFunction();
	void										connectToN2YO(std::string noradId);
	bool										isConnected();
	void										populateData();
	std::vector<std::string>					readToVector(std::string path);
	void										writeTleToFile(ofBuffer data);

	std::map<int, std::vector<std::string>>		m_tleData;
	std::string									m_apiKey;
	std::vector<std::string>					m_defSat;
	ofHttpResponse								m_n2yoResponse;
	bool										m_isGood{ false };
};

