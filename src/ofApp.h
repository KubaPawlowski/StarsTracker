// This is heart of application, 
// it collects all setups, draws, updates
// Author: Kuba Pawlowski

#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "gl/ofGLUtils.h"

#include "Player.h"

#include "ProgramMode.h"
#include "getTle.h"
#include "CelestialSphere.h"
#include "SolarBody.h"
#include "StarBody.h"
#include "ArtificialSatellite.h"

#include "PointOnBody.h"
#include "ImportData.h"

#include "libnova/libnova.h"
#include <vector>

class ofApp : public ofBaseApp, public ofThread
{
public:
	void								setup();
	void								draw();
	void								update();

	void								windowResized(int width, int height);
	void								onToggleEvent(ofxDatGuiToggleEvent e);
	void								onButtonEvent(ofxDatGuiButtonEvent e);

	void								isMouseOnGui();
	bool								isMouseOnWindow();
	void								mouseReleased(int x, int y, int button);
	void								mouseDragged(int x, int y, int button);

private:
	void								setupCelestialSphere();
	void								setupGUI();
	void								setupSolarBodies();
	void								setupStars();
	void								setupSatellites();
	void								setupPlayer();
	void								setupLight();
	void								loadStars();

	void								select();
	void								preselect();
	void								updateGUI();
	void								drawScene(bool usingFbo = false);
	void								drawGUI();
	void								drawLabel(std::string labelText, glm::vec3 labelPosition);
	void								threadedFunction();

private:
	ProgramMode*						ProgramModeRef = ProgramMode::getInstance();

	//PLAYER
	Player								MainPlayer;
	PointOnBody							*LocationPoint;

	//LIGHTS
	ofLight								SunLight;

	//CELESTIAL SPHERE
	CelestialSphere						CelestialSphere;

	//STARS
	ImportData							m_starsData;
	std::vector<StarBody*>				m_starBodies;
	std::map<std::string, glm::vec3>	m_starBodiesLabels;
	bool								m_starsEnabled{ true };
	bool								m_starsLoading;
	bool								m_starsLoaded{ false };
	bool								m_starsInitialized{ false };

	//ARTIFICIAL SATELLITES
	getTle								TleData;
	std::vector<ArtificialSatellite*>	m_satellitesBodies;

	//SOLAR SYSTEM
	std::vector<SolarBody*>				m_solarBodies;
	//planets
	SolarBody							Sun;
	SolarBody							Mercury;
	SolarBody							Venus;
	SolarBody							Earth;
	SolarBody							Mars;
	SolarBody							Jupiter;
	SolarBody							Saturn;
	SolarBody							Neptune;
	SolarBody							Uranus;
	SolarBody							Pluto;
	//moons
	SolarBody							Moon;

	//GUI
	ofxDatGui							MainGui;
	
	ofxDatGuiToggle*					gui_cameraMode;
	ofxDatGuiTextInput*					gui_latitudeInput;
	ofxDatGuiTextInput*					gui_longitudeInput;
	ofxDatGuiLabel*						gui_julianDayLabel;
	ofxDatGuiButton*					gui_updateLocationButton;
	std::string							gui_cameraModeLabel{ "VIEW MODE" };
	std::string							gui_latitudeInputLabel{ "LATITUDE" };
	std::string							gui_longitudeInputLabel{ "LONGITUDE" };
	std::string							gui_julianDayLabelLabel{ "Julian Day: " };

	ofTrueTypeFont						BodyNameLabel;

	//SELECTING
	ofFbo								ColorIndexFbo;
	bool								m_isPreselectionActive{ false };
	bool								m_isSelectionActive{ false };
	Node*								m_preselectedNode;
	Node*								m_selectedNode;

	//EVENTS
	bool								m_isMouseDragging{ false };
	bool								m_satellitesLoaded{ false };
	glm::vec3							m_preselectionPosition{ 0,0,0 };
	glm::vec3							m_selectionPosition{ 0,0,0 };

	//OTHER
	struct tm							m_currentTime{ 0 };
};