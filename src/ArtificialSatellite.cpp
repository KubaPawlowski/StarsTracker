#include "ArtificialSatellite.h"

ArtificialSatellite::ArtificialSatellite()
{

}

void ArtificialSatellite::setupBody()
{
	ofSpherePrimitive bodyMesh;
	bodyMesh.setResolution(20);
	bodyMesh.setRadius(getRadius());
	getMesh() = bodyMesh.getMesh();
}

void ArtificialSatellite::setTle(std::vector<std::string>* tleData)
{
	properName = tleData->at(0);
	m_tle = new Zeptomoby::OrbitTools::cTle(properName, tleData->at(1), tleData->at(2));
	m_satellite = new Zeptomoby::OrbitTools::cSatellite(*m_tle, &properName);
}

void ArtificialSatellite::updateBody()
{
	calcPosition();
}

void ArtificialSatellite::calcPosition()
{
	cJulian	JulianDay(ProgramMode::getInstance()->getTimeT());
	float x = -m_satellite->PositionEci(JulianDay).Position().m_x;
	float y = m_satellite->PositionEci(JulianDay).Position().m_z;
	float z = m_satellite->PositionEci(JulianDay).Position().m_y;

	glm::vec3 parentPosition = getParentNode()->getGlobalPosition();
	glm::vec3 currentPosition = (glm::normalize(glm::vec3({ x,y,z })) * 2000) + parentPosition;
	setGlobalPosition(currentPosition);
}

