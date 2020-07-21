// This class describes artificial satellites
// They positions are calculated based on TLE data with orbitTools library.
// TLE data is retrived from internet at start of the program or from local file. 
// Author: Kuba Pawlowski

#pragma once
#include "BodyNode.h"
#include "ProgramMode.h"
#include "cTLE.h"
#include "cSatellite.h"

class ArtificialSatellite : public BodyNode
{
public:
	ArtificialSatellite();
	void									setupBody();
	void									setTle(std::vector<std::string>* tleData);
	void									updateBody();

private:
	void									calcPosition();

private:
	Zeptomoby::OrbitTools::cTle*			m_tle;
	Zeptomoby::OrbitTools::cSatellite*		m_satellite;

};

