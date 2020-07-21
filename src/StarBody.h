// This describes every star body that is created. 
// Positions of star bodies comes from HYGDATABASE - this is huge local file.
// Author: Kuba Pawlowski

#pragma once
#include "ofMain.h"
#include "ProgramMode.h"
#include "BodyNode.h"

class StarBody : public BodyNode
{
public:
	StarBody();
	void										setupBody();
};

