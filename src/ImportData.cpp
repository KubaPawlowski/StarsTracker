#include "ImportData.h"

ImportData::ImportData(std::string path)
	: m_path(path)
{
	readData();
}

ImportData::ImportData()
{

}

ImportData::~ImportData()
{
}

void ImportData::setPath(std::string path)
{
	m_path = path;
}

void ImportData::threadedFunction()
{
	readData(1);
}

void ImportData::readData(int n)
{
	unsigned int counter = 0;
	std::ifstream filein(m_path);
	aria::csv::CsvParser parser(filein);
	std::vector<std::vector<std::string>> output;
	
	for (auto& row : parser)
	{
		if ((counter >= m_lowerLimit && counter <= m_upperLimit) || (m_upperLimit == 0))
		{
			if (counter % n == 0) {
				std::vector<std::string> row2;
				for (auto& field : row)
				{
					row2.push_back(field);
				}
				output.push_back(row2);
			}
			counter++;
		}
		else if (counter < m_lowerLimit)
		{
			counter++;
			continue;
		}
		else
		{
			break;
		}

		if(counter % 20 == 0)
			std::cout << "DATA: reading HYG database | " << ((float)counter / (float)m_upperLimit) * 100 << "%" << std::endl;
	}
	m_data = output;
}

void ImportData::setRowLimit(unsigned int lowerLimit, unsigned int upperLimit)
{
	m_lowerLimit = lowerLimit;
	m_upperLimit = upperLimit;
}

void ImportData::setSkip(unsigned int skip)
{
	m_skip = skip;
}

std::string ImportData::getFieldData(int row, columns field)
{
	return m_data[row][field];
}

std::vector<std::vector<std::string>> ImportData::getData()
{
	return m_data;
}

void ImportData::displayData(int row, columns field)
{
	std::string ffield = m_data[row][field].c_str();
	std::cout << ffield << std::endl;
}
void ImportData::displayData(int row)
{
	for (int i = 0; i < m_data[row].size(); i++)
	{
		std::string ffield = m_data[row][i].c_str();
		std::cout << ffield << std::endl;
	}
}

void ImportData::writeData()
{
	std::cout << "NOT IMPLEMENTED" << std::endl;
}