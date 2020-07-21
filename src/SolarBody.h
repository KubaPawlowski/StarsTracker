// This is describes every body that is placed in Solar System - planets, moons, Sun.
// These bodies positions are calculated with Libnova library. 
// Author: Kuba Pawlowski

#pragma once
#include "ofMain.h"
#include "BodyNode.h"
#include "Constants.h"
#include "ProgramMode.h"
#include "libnova/ln_types.h"

class SolarBody : public BodyNode, public ofThread
{
public:
	SolarBody();
	void										setupBody();
	void										updateBody();
	void										drawOrbit();
	void										setSideRealDay(float sideRealDay);
	static glm::vec3							setVectorLength(glm::vec3 input, float length);
	void										setName(std::string name);
	void										setOrbitalPeriod(float orbitalPeriod);
	void										setScalarPosition(float scalarPosition);
	void										setStartPosition(glm::vec3 startPosition);
	void										setOrbitEnabled(bool enabled);
	void										setShouldRotating(bool shouldRotating);

	float										getScalarPosition();
private:
	glm::vec3									getPlanetPosition();
	glm::vec3									getPlanetPosition(double JD);
	ofQuaternion								getPlanetOrientation();
	void										calcOrbit(int resolution = 30);
public:
	std::function<void(double, ln_rect_posn*)>	m_planetPosFunction;
	std::function<void(double)>					m_planetRotFunction;
private:
	std::vector<glm::vec3>						m_pointsOfOrbit;
	bool										m_orbitEnabled{ false };
	struct ln_rect_posn							m_rectPos { 0, 0, 0 };
	float										m_sideRealDay{ 24.f }; /* hours */
	float										m_orbitalPeriod{ 365 }; /* days */
	float										m_scalePosition{ 1 };
	glm::vec3									m_startPosition{ 0.f,0.f,0.f };
	bool										m_shouldRotating{ true };
};

