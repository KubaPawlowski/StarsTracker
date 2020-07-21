#include "Player.h"
#define PLAYER_START_POSITION {3000,3000,3000}

// Control keys
#define FORWARD_KEY 'w'
#define LEFT_KEY 'a'
#define RIGHT_KEY 'd'
#define BACKWARD_KEY 'd'
#define TEST_KEY 'p'
#define RESET_TIME_KEY 'r'
#define SPEED_UP_TIME_KEY ']'
#define SLOW_DOWN_TIME_KEY '['

/////////////////////////////////////////SETUP////////////////////////////////////////////////////

Player::Player()
{
	ofAddListener(ofEvents().mouseDragged, this, &Player::mouseDragged);
	ofAddListener(ofEvents().keyPressed, this, &Player::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &Player::keyReleased);

	m_cameras[0] = &EarthCamera;
	m_cameras[1] = &SpaceCamera;

	m_cameras[0]->setFarClip(600000.f);
	m_cameras[0]->setNearClip(1);
	m_cameras[1]->setFarClip(300000.f);
	m_cameras[1]->setNearClip(100);

	setGlobalPosition(PLAYER_START_POSITION);

	EarthCamera.setParent(*this, true);
	EarthCamera.setPosition({ 0,1,0 });
	SpaceCamera.setParent(*this, true);
	SpaceCamera.setPosition({ 0,1,0 });
}

/////////////////////////////////////////USER INPUT//////////////////////////////////////////////

void Player::keyPressed(ofKeyEventArgs& keyboard)
{
	// Here user keyboard input is handled 

	// Moving player on earth view
	if (ProgramMode::getInstance()->getCameraMode() == EARTH)
	{
		if (keyboard.key == FORWARD_KEY)
			dolly(-10);
		else if (keyboard.key == BACKWARD_KEY)
			dolly(10);
		else if (keyboard.key == LEFT_KEY)
			truck(-10);
		else if (keyboard.key == RIGHT_KEY)
			truck(10);
	}
	// Reseting time to current
	if (keyboard.key == RESET_TIME_KEY)
	{
		ProgramMode::getInstance()->setTimeMode(REALTIME);
		m_timeScale = 0;
	}

	// Slowing time
	if (keyboard.key == SLOW_DOWN_TIME_KEY)
	{
		m_countKeyPressed++;

		if (m_countKeyPressed < 10)
			m_timeScale -= 0.0000001;
		else if (m_countKeyPressed >= 10 && m_countKeyPressed < 50)
			m_timeScale -= 0.000001;
		else if (m_countKeyPressed >= 50)
			m_timeScale -= 0.00001;

		ProgramMode::getInstance()->setTimeMode(MANUAL, m_timeScale);
	}

	// Speed up time 
	if (keyboard.key == SPEED_UP_TIME_KEY)
	{
		m_countKeyPressed++;
		if (m_countKeyPressed < 10)
			m_timeScale += 0.0000001;
		else if (m_countKeyPressed >= 10 && m_countKeyPressed < 50)
			m_timeScale += 0.000001;
		else if (m_countKeyPressed >= 50)
			m_timeScale += 0.00001;
		ProgramMode::getInstance()->setTimeMode(MANUAL, m_timeScale);
	}

	// Enabling visibility of FBO layer - for testing
	if (keyboard.key == TEST_KEY)
		ProgramMode::getInstance()->setFboTest(!ProgramMode::getInstance()->getFboTestEnabled());

}

void Player::keyReleased(ofKeyEventArgs& keyboard)
{
	m_countKeyPressed = 0;
}

void Player::mouseDragged(ofMouseEventArgs& mouse)
{
	if (!ProgramModeRef->getIsMouseOnGui())
	{
		float vWidth = ofGetWidth();
		float vHeight = ofGetHeight();

		int x = ofGetMouseX();
		int xp = ofGetPreviousMouseX();
		int y = ofGetMouseY();
		int yp = ofGetPreviousMouseY();

		if (mouse.button == OF_MOUSE_BUTTON_LEFT)
		{
			panDeg((float)((x - xp) / 5));
			m_cameras[m_cameraIndex]->tiltDeg((float)((y - yp) / 5));
		}
	}
}

////////////////////////////////////////////CAMERA//////////////////////////////////////////////////

void Player::setCamera()
{
	// Choosing which camera is active
	if (ProgramModeRef->getCameraMode() == SPACE)
	{
		clearParent();
		setGlobalPosition(PLAYER_START_POSITION);
		SpaceCamera.setPosition(SpaceCamera.getParent()->getGlobalPosition());
		SpaceCamera.clearParent();
		SpaceCamera.setParent(*this);
		m_cameraIndex = 1;
		std::cout << "PLAYER: Space view enabled" << std::endl;
	}
	else
	{
		setParent(*getParentNode(), true);
		setGlobalPosition(getParentNode()->getGlobalPosition());
		resetPlayer();
		m_cameraIndex = 0;
		std::cout << "PLAYER: Earth view enabled" << std::endl;
	}
}


void Player::startCamera()
{
	ofEnableDepthTest();
	m_cameras[m_cameraIndex]->begin();
}
void Player::stopCamera()
{
	m_cameras[m_cameraIndex]->end();
	ofDisableDepthTest();
}
ofCamera* Player::getCamera()
{
	return m_cameras[m_cameraIndex];
}

void Player::resetPlayer()
{
	resetTransform();
	lookAt(getParentNode()->getParent()->getGlobalPosition());
	rollDeg(180);
	tiltDeg(90);
}
