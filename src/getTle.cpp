#include "getTle.h"
#define API_KEY_PATH "F:\\Kuba\\CPP\\StarsTracker\\bin\\data\\N2YO_API_KEY.txt"
#define DEF_SAT_LIST "F:\\Kuba\\CPP\\StarsTracker\\bin\\data\\DEF_SAT_LIST.txt"
#define TLE_DATA_PATH "F:\\Kuba\\CPP\\StarsTracker\\bin\\data\\TLE_DATA.txt"


getTle::getTle()
{
	startThread();
}

void getTle::getApiKey()
{
	std::cout << "TLE: getting api key for N2YO.com" << std::endl;
	m_apiKey = readToVector(API_KEY_PATH).at(0);
}

void getTle::getDefSatellites()
{
	std::cout << "TLE: getting default satellites list." << std::endl;
	m_defSat = readToVector(DEF_SAT_LIST);
}

void getTle::threadedFunction()
{
	if (!isTleDataCurrent() && isConnected())
	{
		getApiKey();
		getDefSatellites();
		for (auto& sat : m_defSat)
			connectToN2YO(sat);
	}
	
	populateData();
}

void getTle::connectToN2YO(std::string noradId)
{
	if (!noradId.empty())
	{
		std::cout << "TLE: getting TLE for " << noradId << std::endl;
		std::string url = "https:\/\/www.n2yo.com\/rest\/v1\/satellite\/tle\/" + noradId + "&apiKey=" + m_apiKey;

		m_n2yoResponse = ofLoadURL(url);
		if(m_n2yoResponse.error.empty())
			writeTleToFile(m_n2yoResponse.data);
		else
			std::cout << "TLE: connection error during fetching data." << std::endl;
	}
	else
		std::cout << "TLE: wrong NORAD ID." << noradId << std::endl;
}

bool getTle::isConnected()
{
	std::string url = "https:\/\/www.n2yo.com";
	ofHttpResponse n2yoTestResponse = ofLoadURL(url);
	if (n2yoTestResponse.error.empty())
	{
		std::cout << "TLE: connection test - success." << std::endl;
		return true;
	}
	else
	{
		std::cout << "TLE: connection test - failed. Error: " << n2yoTestResponse.error << std::endl;
		return false;
	}

}

void getTle::populateData()
{
	std::vector<std::string> vectorOfLines;

	std::ifstream file(TLE_DATA_PATH);

	if (file.is_open())
	{
		int lineCounter{ 0 };
		std::string line;
		int noradId{0};

		std::vector<std::string> tempData;

		while (std::getline(file, line))
		{
			if (lineCounter % 5 != 0)
			{
				if ((lineCounter - 1) % 5 == 0)
				{
					noradId = stoi(line);
				}
				else
				{
					tempData.push_back(line);
					if (tempData.size() == 3)
					{
						m_tleData.insert(std::pair<int, std::vector<std::string>>(noradId, tempData));
						tempData.clear();
					}
				}
			}
			lineCounter++;
		}
		file.close();
		m_isGood = true;
	}
	else
	{
		std::cout << "TLE: error during reading TLE file." << std::endl;
		m_isGood = false;
	}
}

bool getTle::good()
{
	return m_isGood;
}

bool getTle::isTleDataCurrent()
{
	std::cout << "TLE: checking if TLE data is current." << std::endl;
	std::ifstream file(TLE_DATA_PATH);

	if (file.is_open())
	{
		std::string line;
		std::getline(file, line);
		file.close();
		

		int fileDate = std::stoi(line);
		std::string y = std::to_string(ofGetYear());
		std::string m = std::to_string(ofGetMonth());
		std::string d = std::to_string(ofGetDay());
		std::string dateString = y + m + d;
		int currentDate = std::stoi(dateString);

		if (fileDate == currentDate)
		{
			std::cout << "TLE: tle data current, local file will be used." << std::endl;
			return true;
		}
		else
		{
			// Clearing file that is out of date
			std::ofstream file;
			file.open(TLE_DATA_PATH, std::ofstream::out | std::ofstream::trunc);
			file.close();

			std::cout << "TLE: tle data is not current, updating now." << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "TLE: tle data does not exists, creating now."  << std::endl;
		return false;
	}
}

std::vector<std::string> getTle::readToVector(std::string path)
{
	std::vector<std::string> vectorOfLines;
	if (path.empty())
	{
		std::cout << "READING FILE: path is empty." << std::endl;
		return vectorOfLines;
	}

	std::ifstream file(path);

	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			vectorOfLines.push_back(line);
		}
		file.close();
		return vectorOfLines;
	}
	else
	{
		std::cout << "READING FILE: error reading file at path." << path << std::endl;
		return vectorOfLines;
	}
}

void getTle::writeTleToFile(ofBuffer data)
{
	std::string bufData = data;
	
	// Extracting information from buffer, a bit of ugly
	std::string toFind = "\"satid\":";
	size_t index = bufData.find(toFind);
	bufData.erase(0, index + toFind.size());

	toFind = ",";
	index = bufData.find(toFind);
	std::string satId = bufData.substr(0, index);

	toFind = "\"satname\":\"";
	index = bufData.find(toFind);
	bufData.erase(0,index+toFind.size());
	
	toFind = "\"";
	index = bufData.find(toFind);
	std::string satName = bufData.substr(0, index);

	toFind = "\"tle\":\"";
	index = bufData.find(toFind);
	bufData.erase(0,index+toFind.size());

	toFind = "\\r\\n";
	index = bufData.find(toFind);
	std::string tleLine1 = bufData.substr(0, index);
	bufData.erase(0,index+toFind.size());

	toFind = "\"}";
	index = bufData.find(toFind);
	std::string tleLine2 = bufData.substr(0, index);

	// Wrting information to file

	std::ofstream file;
	file.open(TLE_DATA_PATH, std::ios::app);

	if (file.is_open())
	{
		file << ofGetYear() << ofGetMonth() << ofGetDay() << endl;
		file << satId << endl;
		file << satName << endl;
		file << tleLine1 << endl;
		file << tleLine2 << endl;
		file.close();
	}
	else
		std::cout << "TLE: error during writing file." << std::endl;
}

std::map<int, std::vector<std::string>> getTle::getData()
{
	return m_tleData;
}