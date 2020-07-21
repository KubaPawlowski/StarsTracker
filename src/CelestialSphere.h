// Celestial sphere is just background for scene with stars texture
// Author: Kuba Pawlowski

#pragma once
#include "BodyNode.h"

class CelestialSphere : public BodyNode
{
public:
	CelestialSphere();
	void						setupBody();
};

