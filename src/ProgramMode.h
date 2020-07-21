// Program keeps here all global data, e.g. objects register, current time mode, selected camera
// Author: Kuba Pawlowski

#pragma once
#include "ofMain.h"
#include "Node.h"
#include "libnova/julian_day.h"
#include "libnova/sidereal_time.h"

enum timeMode {
	REALTIME,
	CUSTOM,
	MANUAL
};

enum cameraMode {
	SPACE,
	EARTH
};

struct indexPair {
	ofColor indexColor;
	Node* nodePtr;
};

class ProgramMode : public ofThread
{
private:
	ProgramMode();
	void							threadedFunction();
	void							calcJD();
public:
	static ProgramMode*				getInstance();

	void							setIsMouseOnGui(bool onGui);
	void							setTimeMode(timeMode mode, float timeSpeed = 0.01f);
	void							setCamerMode(cameraMode mode);
	void							setTimeT(time_t t);
	void							setFboTest(bool enable);
	time_t							getTimeT();
	float							getSideRealTime();
	const float						getSideRealDay();
	double							getJulianDay();
	int								getCameraMode();
	bool							getIsMouseOnGui();
	int								getObjectsCounter();
	const std::map<int, indexPair>*	getObjectsRegister();
	bool							getFboTestEnabled();
	void							addToRegister(Node *nodePtr);
	Node*							find(ofColor toFind);

private:
	static ProgramMode*				sInstance;

	ofTimer							m_timer;

	timeMode						m_TimeMode{ REALTIME };
	float							m_TimeSpeed{ 0.01f };
	float							m_SideRealTime{ 0 };
	const float						m_SideRealDay{ 23.9344696f };
	double							m_JulianDay{ 0 };
	bool							m_CameraMode{ SPACE };
	time_t							m_TimeT{ 0 };
	std::map<int, indexPair>		m_objectsRegister;
	ofColor							m_lastIndexColor;
	int								m_objectsCounter{ 0 };
	bool							m_isMouseOnGui{ false };
	bool							m_fboTestEnabled{ false };
};

