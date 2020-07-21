// This keeps track of player position, user input and camera
// Author: Kuba Pawlowski

#pragma once
#include "ofMain.h"
#include "ProgramMode.h"

class Player : public Node
{
public:
	Player();
	void								keyPressed(ofKeyEventArgs& keyboard);
	void								keyReleased(ofKeyEventArgs& keyboard);
	void								setCamera();
	void								mouseDragged(ofMouseEventArgs& mouse);
	void								startCamera();
	void								stopCamera();

	ofCamera*							getCamera();

	void								resetPlayer();
private:
	ProgramMode*						ProgramModeRef = ProgramMode::getInstance();
	unsigned int						m_cameraIndex{ 1 };
	ofCamera*							m_cameras[2];
	ofCamera							EarthCamera;
	ofCamera							SpaceCamera;
	ofNode								PlayerPlane;

	int									m_countKeyPressed{ 0 };
	float								m_cursorSpeed{ 1.f };
	float								m_timeScale{ 0 };
};

