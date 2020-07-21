// This describes objects that can be placed on spherical objects,
// with longitude and latitude.
// This is used to make location point on Earth to place player there.
// Author: Kuba Pawlowski

#pragma once
#include "SolarBody.h"
#include "Node.h"

class PointOnBody : public Node
{
public:
	PointOnBody(Node &Body);
	void							setLatitude(float latitude);
	void							setLongitude(float longitude);
	void							setLocation(float latitude, float longitude);
	float							getLongitude();
	float							getLatitude();
	glm::vec3						convertAnglesToVector(float longitude, float latitude, float length = 100.f);
private:
	void							updatePosition();
	glm::vec3						pointPosition{ 0,0,0 };
	float							m_latitude{ 51.780852 };	// Lodz, PL
	float							m_longitude{ 19.458438 };	// Lodz, PL
	Node*							m_Body;


};

