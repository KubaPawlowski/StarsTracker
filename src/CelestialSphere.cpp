#include "CelestialSphere.h"


CelestialSphere::CelestialSphere()
{
}

void CelestialSphere::setupBody()
{
	ofSpherePrimitive bodyMesh;
	bodyMesh.setResolution(200);
	bodyMesh.setRadius(getRadius());
	getMesh() = bodyMesh.getMesh();
}