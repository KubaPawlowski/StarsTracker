#include "PointOnBody.h"

PointOnBody::PointOnBody(Node& Body)
{
	setRadius(10);
	m_Body = &Body;
	setParent(*m_Body, true);

	updatePosition();
}

void PointOnBody::updatePosition()
{
	pointPosition = convertAnglesToVector(m_longitude, m_latitude, m_Body->getRadius());
	setPosition(pointPosition);
	setOrientation(m_Body->getOrientationEuler());
}

void PointOnBody::setLatitude(float latitude)
{
	m_latitude = latitude;
	updatePosition();
}

void PointOnBody::setLongitude(float longitude)
{
	m_longitude = longitude;
	updatePosition();
}

void PointOnBody::setLocation(float latitude, float longitude)
{
	setLatitude(latitude);
	setLongitude(longitude);
}

float PointOnBody::getLongitude()
{
	return m_longitude;
}

float PointOnBody::getLatitude()
{
	return m_latitude;
}


glm::vec3 PointOnBody::convertAnglesToVector(float longitude, float latitude, float length)
{
	glm::vec3 output  = { length,0,0 };
	output = glm::rotate(output, glm::radians(90.f + longitude), glm::vec3(0.0f, 1.0f, 0.0f));
	output = glm::rotate(output, glm::radians(latitude), glm::vec3(1.0f, 0.0f, 0.0f));
	return output;
}