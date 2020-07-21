// This is base class for every body created,
// objects that inherit from this can be drawn and selected
// Author: Kuba Pawlowski

#pragma once
#include "ProgramMode.h"
#include "Node.h"

class BodyNode : public Node
{
public:
	BodyNode();
	void										drawVisible();
	void										drawForFbo();
	void										setTexture(std::string path);
	void										setMaterialEnabled(bool enabled);

private:
	bool										m_materialEnabled{ false };
	std::string									m_TexturePath;
	ofTexture									BodyTexture;
	ofMaterial									BodyMaterial;
};