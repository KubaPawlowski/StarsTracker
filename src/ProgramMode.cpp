#include "ProgramMode.h"

ProgramMode* ProgramMode::getInstance()
{
	if (!sInstance) 
	{
		sInstance = new ProgramMode;
		return sInstance;
	}
	else 
		return sInstance;
}

ProgramMode::ProgramMode()
{
	// Set timer for calculating JD every second, can cause low fps on fast time mode
	m_timer.setPeriodicEvent(500000);
	startThread();
}

void ProgramMode::threadedFunction()
{
	while (isThreadRunning())
	{
		m_timer.waitNext();
		calcJD();
	}
}

void ProgramMode::calcJD()
{
	// Calculate Julian Day in order what time mode is selected
	if (m_TimeMode == REALTIME)
		m_JulianDay = ln_get_julian_from_sys();
	else if (m_TimeMode == MANUAL)
		m_JulianDay += m_TimeSpeed;
	else if (m_TimeMode == CUSTOM)
		m_JulianDay = ln_get_julian_from_timet(&m_TimeT);
}

void ProgramMode::setIsMouseOnGui(bool onGui)
{
	m_isMouseOnGui = onGui;
}

void ProgramMode::setTimeMode(timeMode mode, float timeSpeed)
{
	m_TimeMode = mode;
	m_TimeSpeed = timeSpeed;
}

void ProgramMode::setTimeT(time_t t)
{
	m_TimeT = t;
}
void ProgramMode::setCamerMode(cameraMode mode)
{
	m_CameraMode = mode;
}

void ProgramMode::setFboTest(bool enable)
{
	m_fboTestEnabled = enable;
}

float ProgramMode::getSideRealTime() 
{ 
	return (float)ln_get_mean_sidereal_time(m_JulianDay);
}

const float ProgramMode::getSideRealDay() 
{ 
	return m_SideRealDay; 
}

double ProgramMode::getJulianDay() 
{ 
	return m_JulianDay; 
}

time_t ProgramMode::getTimeT()
{
	time_t time;
	ln_get_timet_from_julian(m_JulianDay, &time);
	return time;
}

int ProgramMode::getCameraMode()
{
	return m_CameraMode;
}

bool ProgramMode::getIsMouseOnGui()
{
	return m_isMouseOnGui;
}

bool ProgramMode::getFboTestEnabled()
{
	return m_fboTestEnabled;
}

void ProgramMode::addToRegister(Node* nodePtr)
{
	// Here I'm registering new body objects to map with unique index colors on FBO layer
	// that allows me to point at any shape on screen and using hidden fbo layer find reference to right object instance
	// and then it is possible to use that reference for whatever needed

	ofColor indexColor;
	unsigned int r{ 0 };
	unsigned int g{ 0 };
	unsigned int b{ 0 };

	// Check last assigned index and pick new bigger one for object
	// indexes are in rgb so it looks like this, probably this could be much better
	if (m_objectsRegister.size() != 0)
	{
		if (m_lastIndexColor.b != 255)
		{
			b = m_lastIndexColor.b+1;
			g = m_lastIndexColor.g;
			r = m_lastIndexColor.r;
		}
		else
		{
			b = 0;
			if (m_lastIndexColor.g != 255)
			{
				g = m_lastIndexColor.g+1;
				r = m_lastIndexColor.r;
			}
			else
			{
				if (m_lastIndexColor.r != 255)
					r = m_lastIndexColor.r+1;
			}
		}
	}

	indexColor.r = r;
	indexColor.g = g;
	indexColor.b = b;

	nodePtr->setIndexColor(indexColor);		// assign unique index color to object
	m_lastIndexColor = indexColor;			// this will give reference by color to last created object

	indexPair indexPairInst;
	indexPairInst.indexColor = indexColor;
	indexPairInst.nodePtr = nodePtr;

	// insert object to register map of objects to get easy access to any object (body) created
	m_objectsRegister.insert(pair<int, indexPair>(m_objectsCounter, indexPairInst));
	m_objectsCounter++;
}

int ProgramMode::getObjectsCounter()
{
	return m_objectsCounter;
}

const std::map<int, indexPair>* ProgramMode::getObjectsRegister()
{
	return &m_objectsRegister;
}

Node* ProgramMode::find(ofColor toFind)
{
	// This function returns reference to object instance assigned to provided color index
	Node* ptr = nullptr;
	if (!m_objectsRegister.empty())
		for (auto itr : m_objectsRegister)
			if (itr.second.indexColor == toFind)
				ptr = itr.second.nodePtr;

	return ptr;
}
