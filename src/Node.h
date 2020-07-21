// This is base class for every BodyNode,
// BodyNodes inherit their parameters from this
// Author: Kuba Pawlowski

#pragma once
#include "ofMain.h"

class Node : public of3dPrimitive
{
public:
	Node();
	void										setIndexColor(ofColor indexColor);
	void										setRadius(float radius);
	void										setParentNode(Node* node);
	ofColor										getIndexColor();
	float										getRadius();
	Node*										getParentNode();


public:
// HYG-database additional parameters of celestial body
// there is much more data in catalogue but that's enough for now
	int											id{ 0 };						//body id in catalogue
	float										ra{ 0.f };						//right ascension for epoch&equinox 2000.0
	float										dec{ 0.f };						//declination for epoch&equinox 2000.0
	float										dist{ 0.f };					//distance in parsec
	float										mag{ 0.f };						//apparent visual magnitude
	std::string									properName{ "Unnamed" };		//common name, not much of it

private:
	ofColor										m_indexColor;
	float										m_radius{ 100 };
	Node*										m_parentNode;
};

