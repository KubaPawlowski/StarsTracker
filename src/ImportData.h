// This is used to load HYGDATABASE from local csv file.
// HYGDATABASE has all data needed to determine position and magnitude of every bright star in the sky.
// This class gives ability to load only part of over 100000 positions in file.
// Author: Kuba Pawlowski

#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "ofMain.h"
#include "CsvParser.h"

// Columns names in HYG-database
enum columns {
	id,
	hip,
	hd,
	hr,
	gl,
	bf,
	proper,
	ra,
	dec,
	dist,
	pmra,
	pmdec,
	rv,
	mag,
	absmag,
	spect,
	ci,
	x,
	y,
	z,
	con,
	lum,
};

class ImportData : public ofThread
{
private:
	std::string								m_path{ "null" };
	std::vector<std::vector<std::string>>	m_data = { {" "},{" "} };
	unsigned int							m_upperLimit{ 10 };
	unsigned int							m_lowerLimit{ 1 };
	unsigned int							m_skip{ 10 };

public:
	ImportData(std::string path);
	ImportData();
	~ImportData();

	void displayData(int row, columns field);
	void displayData(int row);

	void setPath(std::string path);

	void threadedFunction();
	void readData(int n = 1);
	void setRowLimit(unsigned int lowerLimit, unsigned int upperLimit);
	void setSkip(unsigned int skip);
	std::string getFieldData(int row, columns field);
	std::vector<std::vector<std::string>> getData();
	void writeData();
};
