#include "SolarBody.h"

SolarBody::SolarBody()
{
}

void SolarBody::setupBody()
{
	ofSpherePrimitive bodyMesh;
	bodyMesh.setResolution(200);
	bodyMesh.setRadius(getRadius());
	getMesh() = bodyMesh.getMesh();
	
	setGlobalPosition(getPlanetPosition());
}

void SolarBody::updateBody()
{
	setPosition(getPlanetPosition());
	if (m_shouldRotating)
	setOrientation(getPlanetOrientation());
}

ofQuaternion SolarBody::getPlanetOrientation()
{
	ofQuaternion orientation = { 360 * (ProgramMode::getInstance()->getSideRealTime() / m_sideRealDay) + 80, glm::vec3(0, 1, 0) };
	return orientation;
}
void SolarBody::setStartPosition(glm::vec3 startPosition)
{
	m_startPosition = startPosition;
}

//for current date
glm::vec3 SolarBody::getPlanetPosition()
{
	if (m_planetPosFunction != nullptr)
		m_planetPosFunction(ProgramMode::getInstance()->getJulianDay(), &m_rectPos);
	return glm::vec3({ -(float)m_rectPos.X, (float)m_rectPos.Z, (float)m_rectPos.Y }) * m_scalePosition + m_startPosition;
}
//for provided date
glm::vec3 SolarBody::getPlanetPosition(double JD)
{
	if (m_planetPosFunction != nullptr)
		m_planetPosFunction(JD, &m_rectPos);
	return glm::vec3({ -(float)m_rectPos.X, (float)m_rectPos.Z, (float)m_rectPos.Y });
}
void SolarBody::setName(std::string name)
{
	properName = name;
}
void SolarBody::setOrbitEnabled(bool enabled)
{
	m_orbitEnabled = enabled;
}
void SolarBody::setShouldRotating(bool shouldRotating)
{
	m_shouldRotating = shouldRotating;
}
void SolarBody::setSideRealDay(float sideRealDay)
{
	m_sideRealDay = sideRealDay;
}
void SolarBody::setOrbitalPeriod(float orbitalPeriod)
{
	m_orbitalPeriod = orbitalPeriod;
	calcOrbit();
}
void SolarBody::setScalarPosition(float scalarPosition)
{
	m_scalePosition = scalarPosition;
}

float SolarBody::getScalarPosition()
{
	return m_scalePosition;
}

void SolarBody::calcOrbit(int resolution)
{
	//This function calculates object orbit (if orbit is enabled) and defines points of this orbit
	//these points are calculated based on provided resolution from functions that calculates positions of objects
	m_pointsOfOrbit.clear();
	double JDPart = (double)(m_orbitalPeriod / (float)resolution);
	for (int i = 0; i < resolution; i++)
		m_pointsOfOrbit.push_back(getPlanetPosition((JDPart * (double)(i))+ProgramMode::getInstance()->getJulianDay()));
}
void SolarBody::drawOrbit()
{	//Function to draw orbits of objects if enabled, it is calculated only once for performance reason (except moon)

	ofDisableLighting();
	if (properName == "Moon")
	{
		double JD = ProgramMode::getInstance()->getJulianDay();
		if ((int)JD % 10 == 0)
			calcOrbit(30); //this place is crucial for performance - updates of orbit elipse of moon are slowing down whole program, this should be corrected
	}
	for (int i = 0; i < m_pointsOfOrbit.size(); i++)
	{
		if (i == m_pointsOfOrbit.size()-1)
		{
			ofDrawLine(m_pointsOfOrbit[i] * m_scalePosition + m_startPosition, m_pointsOfOrbit[0] * m_scalePosition + m_startPosition);
			break;
		}
		ofDrawLine(m_pointsOfOrbit[i] * m_scalePosition + m_startPosition, m_pointsOfOrbit[i+1] * m_scalePosition + m_startPosition);
	}

	ofEnableLighting();

}

glm::vec3 SolarBody::setVectorLength(glm::vec3 input, float length)
{
	return glm::normalize(input) * length;
}
