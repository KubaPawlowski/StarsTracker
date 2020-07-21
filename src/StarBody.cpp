#include "StarBody.h"

StarBody::StarBody()
{

}


void StarBody::setupBody()
{    
	ofSpherePrimitive bodyMesh;
	bodyMesh.setResolution(5);
	bodyMesh.setRadius(getRadius());
	getMesh() = bodyMesh.getMesh();
}



