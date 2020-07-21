

#include "ofApp.h"

#define STARS_LIMIT 500
#define COLOR_INDEX_BACKGROUND 255
#define HYGDATABASE_PATH "F:\\Kuba\\CPP\\StarsTracker\\bin\\data\\hygdata_v3magsort.csv"

//////////////////////////////////////////SETUPS/////////////////////////////////////////////////////

void ofApp::setup()
{

	ofBackground(30);
	ofSetVerticalSync(true);

	//--------------------------------------------------------------------------//
	//CELESTIAL SPHERE
	setupCelestialSphere();

	//--------------------------------------------------------------------------//
	//PLANETS AND MOONS SETUP
	setupSolarBodies();

	//--------------------------------------------------------------------------//
	//STARS BODIES LOADING
	loadStars(); //read data from csv file in parallel thread

	//--------------------------------------------------------------------------//
	//PLAYER SETUP
	setupPlayer();

	//--------------------------------------------------------------------------//
	//LIGHT SETUP
	setupLight();

	//--------------------------------------------------------------------------//
	//GUI SETUP
	setupGUI();

	//--------------------------------------------------------------------------//
	//FBO FOR COLOR INDEXING
	ColorIndexFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);

	//OTHER
	m_currentTime.tm_year = 120;
	m_currentTime.tm_mday = 25;
	m_currentTime.tm_mon = 4;
	m_currentTime.tm_hour = 22;
	m_currentTime.tm_min = 20;
	m_currentTime.tm_sec = 0;
}

void ofApp::setupGUI()
{
	std::cout << "SETUP: GUI" << std::endl;

	gui_cameraMode = MainGui.addToggle(gui_cameraModeLabel, false);
	MainGui.addBreak();
	MainGui.addLabel("Position on Earth:");
	gui_latitudeInput = MainGui.addTextInput(gui_latitudeInputLabel, to_string(LocationPoint->getLatitude()));
	gui_latitudeInput->setInputType(ofxDatGuiInputType::NUMERIC);
	gui_longitudeInput = MainGui.addTextInput(gui_longitudeInputLabel, to_string(LocationPoint->getLongitude()));
	gui_longitudeInput->setInputType(ofxDatGuiInputType::NUMERIC);
	gui_updateLocationButton = MainGui.addButton("UPDATE");
	MainGui.addBreak();
	gui_julianDayLabel = MainGui.addLabel(to_string(ProgramMode::getInstance()->getJulianDay()));

	MainGui.onToggleEvent(this, &ofApp::onToggleEvent);
	MainGui.onButtonEvent(this, &ofApp::onButtonEvent);

	//labels for planets setup
	BodyNameLabel.load("proxima-nova-regular.ttf", 8);
}
void ofApp::setupCelestialSphere()
{
	// Setup celestial sphere - this shows bitmap with stars to get nice background,
	// its orientation is synchronized with stars positions that are seperate objects loaded later and are selectable

	std::cout << "SETUP: CELESTIAL SPHERE" << std::endl;
	CelestialSphere.setRadius(CELESTIAL_SPHERE_RADIUS + 2000);
	CelestialSphere.setupBody();
	CelestialSphere.setTexture("RandomizedSkymap.t4_08192x04096_m.tif");
}
void ofApp::setupLight()
{
	// This setups light in scene - it is placed of course in the centre of the scene
	// where Sun is placed
	std::cout << "SETUP: LIGHT" << std::endl;
	SunLight.setPointLight();
	SunLight.setGlobalPosition(0, 0, 0);
	SunLight.setAttenuation(0.01);
	SunLight.setScale(1);
}
void ofApp::setupSolarBodies()
{
	// Setup every solar body - sun, planets and moons
	// This is kind of manual setup 

	std::cout << "SETUP: SOLAR BODIES" << std::endl;

	m_solarBodies.push_back(&Sun);
	m_solarBodies.push_back(&Moon);
	m_solarBodies.push_back(&Mercury);
	m_solarBodies.push_back(&Venus);
	m_solarBodies.push_back(&Earth);
	m_solarBodies.push_back(&Mars);
	m_solarBodies.push_back(&Jupiter);
	m_solarBodies.push_back(&Saturn);
	m_solarBodies.push_back(&Neptune);
	m_solarBodies.push_back(&Uranus);
	m_solarBodies.push_back(&Pluto);

	// Enable orbits displaying for planets and moons
	for (int i = 2; i <= 10; i++)
		m_solarBodies[i]->setOrbitEnabled(true);

	Sun.setName("Sun");
	Sun.setRadius(SUN_RADIUS);
	Sun.setTexture("tex_sun.jpg");

	Moon.setName("Moon");
	Moon.m_planetPosFunction = ln_get_lunar_geo_posn;
	Moon.setScalarPosition(MOON_TO_EARTH_SCALAR);
	Moon.setOrbitalPeriod(MOON_ORBITAL_PERIOD);
	Moon.setRadius(MOON_RADIUS);
	Moon.setTexture("lroc_color_poles_1k.jpg");
	Moon.setShouldRotating(false);

	Mercury.setName("Mercury");
	Mercury.m_planetPosFunction = ln_get_mercury_rect_helio;
	Mercury.setScalarPosition(MERCURY_TO_SUN_SCALAR);
	Mercury.setOrbitalPeriod(MERCURY_ORBITAL_PERIOD);
	Mercury.setRadius(MERCURY_RADIUS);
	Mercury.setTexture("tex_mercury.png");

	Venus.setName("Venus");
	Venus.m_planetPosFunction = ln_get_venus_rect_helio;
	Venus.setScalarPosition(VENUS_TO_SUN_SCALAR);
	Venus.setOrbitalPeriod(VENUS_ORBITAL_PERIOD);
	Venus.setRadius(VENUS_RADIUS);
	Venus.setTexture("tex_venus.jpg");

	Earth.setName("Earth");
	Earth.m_planetPosFunction = ln_get_earth_rect_helio;
	Earth.setSideRealDay(EARTH_SRD); //for rotation calculation
	Earth.setScalarPosition(EARTH_TO_SUN_SCALAR);
	Earth.setOrbitalPeriod(EARTH_ORBITAL_PERIOD);
	Earth.setRadius(EARTH_RADIUS);
	Earth.setTexture("tex_earth.jpg");

	Mars.setName("Mars");
	Mars.m_planetPosFunction = ln_get_mars_rect_helio;
	Mars.setScalarPosition(MARS_TO_SUN_SCALAR);
	Mars.setOrbitalPeriod(MARS_ORBITAL_PERIOD);
	Mars.setRadius(MARS_RADIUS);
	Mars.setTexture("tex_mars.png");

	Jupiter.setName("Jupiter");
	Jupiter.m_planetPosFunction = ln_get_jupiter_rect_helio;
	Jupiter.setScalarPosition(JUPITER_TO_SUN_SCALAR);
	Jupiter.setOrbitalPeriod(JUPITER_ORBITAL_PERIOD);
	Jupiter.setRadius(JUPITER_RADIUS);
	Jupiter.setTexture("tex_jupiter.jpg");

	Saturn.setName("Saturn");
	Saturn.m_planetPosFunction = ln_get_saturn_rect_helio;
	Saturn.setScalarPosition(SATURN_TO_SUN_SCALAR);
	Saturn.setOrbitalPeriod(SATURN_ORBITAL_PERIOD);
	Saturn.setRadius(SATURN_RADIUS);
	Saturn.setTexture("tex_saturn.jpg");

	Neptune.setName("Neptune");
	Neptune.m_planetPosFunction = ln_get_neptune_rect_helio;
	Neptune.setScalarPosition(NEPTUNE_TO_SUN_SCALAR);
	Neptune.setOrbitalPeriod(NEPTUNE_ORBITAL_PERIOD);
	Neptune.setRadius(NEPTUNE_RADIUS);
	Neptune.setTexture("tex_neptune.jpg");

	Uranus.setName("Uranus");
	Uranus.m_planetPosFunction = ln_get_uranus_rect_helio;
	Uranus.setScalarPosition(URANUS_TO_SUN_SCALAR);
	Uranus.setOrbitalPeriod(URANUS_ORBITAL_PERIOD);
	Uranus.setRadius(URANUS_RADIUS);
	Uranus.setTexture("tex_uranus.jpg");

	Pluto.setName("Pluto");
	Pluto.m_planetPosFunction = ln_get_pluto_rect_helio;
	Pluto.setScalarPosition(PLUTO_TO_SUN_SCALAR);
	Pluto.setOrbitalPeriod(PLUTO_ORBITAL_PERIOD);
	Pluto.setRadius(PLUTO_RADIUS);
	Pluto.setTexture("tex_pluto.jpg");

	for (auto& body : m_solarBodies)
		body->setupBody();
}
void ofApp::setupStars() 
{
	// Get stars parameters from catalogue and puts it to objects then store them in vector
	std::cout << "SETUP: STARS" << std::endl;
	
	// Const values for calculating stars apparent size
	const double defSize = 20000.0;		
	const double defMag = -1.44;		// Sirius magnitude for comparsion
	const double base = 2.512;			// Const value used to convert magnitude to scale
	const double minSize{ 50.0 };
	const double maxSize{ 1000.0 };

	int counter = 0;					// For logging precentage of loading
	for (std::vector<std::string> row : m_starsData.getData())
	{
		// Log precentage of populating with stars
		counter++;
		if(counter % 20 == 0)
			std::cout << "SETUP: Populating Universe with stars | " << (counter / m_starsData.getData().size()) * 100 << "%" << std::endl;

		// Create new body
		StarBody* body = new StarBody();

		// Read coordinates from earlier loaded database
		glm::vec3 spawnLocation;
		spawnLocation.x = -std::stof(row[columns::x]);
		spawnLocation.z = std::stof(row[columns::y]);
		spawnLocation.y = std::stof(row[columns::z]);
		spawnLocation = SolarBody::setVectorLength(spawnLocation, CELESTIAL_SPHERE_RADIUS);

		// Write coordinates to body
		body->setGlobalPosition(spawnLocation);

		// Calc radius of star based on magnitude - maybe not the best idea, but visualy works
		double starApparentSize = defSize / pow(base, (std::stod(row[columns::mag]) - defMag));
		if (starApparentSize < minSize)
			starApparentSize = minSize;											// Prevents from creating stars to small to see
		else if (starApparentSize > maxSize)
			starApparentSize = maxSize + ((starApparentSize / 100.0) * 10.0);	// Prevents from creating stars to big
		body->setRadius((float)starApparentSize);

		// Write some individual data to object instance
		body->mag = std::stof(row[columns::mag]);
		body->id = std::stof(row[columns::id]);
		body->ra = std::stof(row[columns::ra]);
		body->dec = std::stof(row[columns::dec]);
		body->dist = std::stof(row[columns::dist]);
		body->mag = std::stof(row[columns::mag]);
		body->properName = row[columns::proper];

		// Setup body mesh
		body->setupBody();
		// Put reference to vector to access them when drawing
		m_starBodies.push_back(body);
		// Populate vector with star name (it is separated because small part of stars in catalogue has proper name, 
		// and it is faster to draw them iterating on seperate smaller vector
		if (!row[columns::proper].empty())
			m_starBodiesLabels.insert(std::pair<std::string, glm::vec3>(row[columns::proper], spawnLocation));

	}
}
void ofApp::setupSatellites() 
{
	// Get loaded satellites data and put refs to vector
	// It is invoked once when TLE data are downloaded from internet or are already current
	std::cout << "SETUP: ARTIFICIAL SATELLITES" << std::endl;
	if (TleData.good()) 
	{	
		for (auto& satBody : TleData.getData())
		{
			ArtificialSatellite* sat = new ArtificialSatellite();
			sat->setParentNode(&Earth);		
			sat->setTle(&satBody.second);	// Put TLE data to object, it gives object ability to calculate its position in real time
			sat->setupBody();
	
			m_satellitesBodies.push_back(sat);
		}
	}
}

void ofApp::loadStars()
{
	// Read data from local stars catalogue file
	m_starsData.setPath(HYGDATABASE_PATH);		// Set path to sorted by mag database
	m_starsData.setRowLimit(2, STARS_LIMIT);	// Read database from down limit to upper limit (first n rows)
	m_starsData.startThread();					// Read every n-th data row of csv
}
void ofApp::setupPlayer()
{
	std::cout << "SETUP: PLAYER" << std::endl;
	LocationPoint = new PointOnBody(Earth);
	MainPlayer.setParentNode(LocationPoint);
}

//////////////////////////////////////////UPDATES////////////////////////////////////////////////////

void ofApp::update()
{
	// Update is called every frame

	//--------------------------------------------------------------------------//
	// CHECKERS
	isMouseOnGui();

	//--------------------------------------------------------------------------//
	// DRAW TO FBO LAYER
	ColorIndexFbo.begin();
	ofClear(COLOR_INDEX_BACKGROUND);
	drawScene(true);
	ColorIndexFbo.end();

	//--------------------------------------------------------------------------//
	// SKY COLOR
	// Based on angle between sun position and player position changes color of the sky
	// to simulate night and day
	if (ProgramModeRef->getCameraMode() == EARTH)
	{
		glm::vec3 nY = glm::normalize(LocationPoint->getGlobalPosition() - Earth.getGlobalPosition());
		glm::vec3 nP = glm::normalize(LocationPoint->getGlobalPosition());
		float sunAngle = glm::degrees(glm::angle(nY, nP));

		int maxBrightness = 200;
		int minBrightness = 30;
		float startDimAtAngle = 100.f;
		float endDimAtAngle = 75.f;

		if (sunAngle > 100.f)
			ofBackground(200);
		else if (sunAngle <= startDimAtAngle && sunAngle >= endDimAtAngle)
		{
			float a = startDimAtAngle - endDimAtAngle;
			float b = sunAngle - endDimAtAngle;
			float c = b / a;

			int brightness = (int)((float)(maxBrightness - minBrightness) * c) + minBrightness;
			ofBackground(brightness);
		}
		else
			ofBackground(30);
	}
	else 
		ofBackground(30);

	//--------------------------------------------------------------------------//
	// SATELLITES LOADING - with seperate thread
	// This is called once at first frame on the separate thread
	if (!TleData.isThreadRunning() && m_satellitesLoaded == false)
	{
		setupSatellites();
		m_satellitesLoaded = true;
	}

	//--------------------------------------------------------------------------//
	// STAR BODIES LOADING - with separate thread
	// This is called once at first frame on the separate thread
	if (m_starsEnabled)
	{
		if (!m_starsData.isThreadRunning() && m_starsLoaded == false)
		{
			std::cout << "First thread end" << std::endl;
			this->startThread();
			m_starsLoaded = true;
		}
		if (m_starsLoaded && !this->isThreadRunning() && m_starsInitialized == false)
		{
			std::cout << "Second thread end" << std::endl;
			m_starsInitialized = true;
			this->stopThread();
		}
	}

	//--------------------------------------------------------------------------//
	//SOLAR BODIES UPDATE
	for (auto& solBody : m_solarBodies)
		solBody->updateBody();

	Moon.setStartPosition(Earth.getGlobalPosition());

	//--------------------------------------------------------------------------//
	//SATELLITES BODIES UPDATE
	for (auto& satBody : m_satellitesBodies)
		satBody->updateBody();

	//--------------------------------------------------------------------------//
	//SELECTION
	if(!m_isMouseDragging)
		preselect();

	//--------------------------------------------------------------------------//
	//LOG
	updateGUI();
}
void ofApp::updateGUI()
{
	// Refresh data in the gui
	gui_julianDayLabelLabel = "Julian day: " + to_string(ProgramMode::getInstance()->getJulianDay());
	gui_julianDayLabel->setLabel(gui_julianDayLabelLabel);
}

//////////////////////////////////////////DRAWS//////////////////////////////////////////////////////

void ofApp::draw()
{
	//--------------------------------------------------------------------------//
	// SCENE
	glPushMatrix();

	// If fbo test is enabled it draws fbo color index layer
	// else it draws normal view
	if (!ProgramModeRef->getFboTestEnabled())
		drawScene(false);
	else
		ColorIndexFbo.draw(0, 0);

	glPopMatrix();

	//--------------------------------------------------------------------------//
	// GUI
	drawGUI();
}

void ofApp::drawScene(bool usingFbo)
{
	// This function draw scene both for normal view and fbo index layer
	// based on function parameter usingFbo

	MainPlayer.startCamera();

	if (!usingFbo) 
	{
		// Part for normal view
		//--------------------------------------------------------------------------//
		// DRAW CELESTIAL SPHERE 
		CelestialSphere.drawVisible();

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		//--------------------------------------------------------------------------//
		// LIGHT START	
		ofEnableLighting();
		SunLight.enable();

		//--------------------------------------------------------------------------//
		// SPAWN SOLAR SYSTEM
		for (auto& solBody : m_solarBodies)
		{
			solBody->drawVisible();
			solBody->drawOrbit();
		}

		//--------------------------------------------------------------------------//
		// SPAWN SATELLITES
		
		for (auto& satBody : m_satellitesBodies)
		{
			satBody->drawVisible();
			ofDrawLine(satBody->getGlobalPosition(), Earth.getGlobalPosition());		
		}

		//--------------------------------------------------------------------------//
		// LIGHT END
		SunLight.disable();
		ofDisableLighting();
		
		//--------------------------------------------------------------------------//
		// DRAW CELESTIAL SPHERE BODIES (STARS BODIES)
		// Light doesn't affect stars, no need for that
		if (m_starsLoaded && m_starsInitialized && m_starsEnabled)
		{
			// This draws loaded stars
			for (auto& starBody : m_starBodies)
				starBody->drawVisible();
		}
		glDisable(GL_CULL_FACE);
	}
	else
	{
		// Part for fbo color index layer
		for (auto& solBody : m_solarBodies)
			solBody->drawForFbo();
		for (auto& satBody : m_satellitesBodies)
			satBody->drawForFbo();
		if (m_starsLoaded && m_starsInitialized && m_starsEnabled)
			for (auto& starBody : m_starBodies)
				starBody->drawForFbo();
	}
	MainPlayer.stopCamera();
}
void ofApp::drawGUI()
{
	// This draws elements of interface 

	//--------------------------------------------------------------------------//
	// PRESELECTION CIRCLE
	if (m_isPreselectionActive)
	{
		glm::vec2 onScreenPosition;
		onScreenPosition.x = MainPlayer.getCamera()->worldToScreen(m_preselectedNode->getGlobalPosition()).x;
		onScreenPosition.y = MainPlayer.getCamera()->worldToScreen(m_preselectedNode->getGlobalPosition()).y;

		ofPushStyle();
		ofSetCircleResolution(50);
		ofNoFill();
		ofSetLineWidth(2);
		ofDrawCircle(onScreenPosition, 20);
		ofPopStyle();
	}

	//--------------------------------------------------------------------------//
	// SELECTION CIRCLE
	if (m_isSelectionActive)
	{
		glm::vec3 onScreenPosition;
		onScreenPosition = MainPlayer.getCamera()->worldToScreen(m_selectedNode->getGlobalPosition());

		if (onScreenPosition.z < 1)
		{
			ofPushStyle();
			ofSetCircleResolution(50);
			ofNoFill();
			ofSetLineWidth(2);
			ofSetColor(ofColor::yellow);
			ofDrawCircle(onScreenPosition.x, onScreenPosition.y, 30);
			ofPopStyle();
		}
	}

	//--------------------------------------------------------------------------//
	// COMPAS
	//if (ProgramMode::getInstance()->getCameraMode() == EARTH)
	//{
	//	float compassSize{ 20 };
	//	float offsetX{ 100 };
	//	float offsetY{ 100 };
	//	ofPoint p1{ (float)ofGetWindowWidth() - offsetX, (float)ofGetWindowHeight() - offsetY, 0 };
	//	ofPoint p2{ (float)ofGetWindowWidth() - offsetX, (float)ofGetWindowHeight() - offsetY - compassSize, 0 };
	//	ofDrawLine(p1, p2.getRotated(-Player.getOrientationEulerDeg().z, p1, { 0,0,1 }));
	//	ofNoFill();
	//	ofDrawCircle(p1, compassSize);
	//}

	//--------------------------------------------------------------------------//
	// STARS LABELS
	if (m_starsLoaded && m_starsInitialized)
		for (auto& starBody : m_starBodiesLabels)
			drawLabel(starBody.first, starBody.second);

	//--------------------------------------------------------------------------//
	// PLANETS LABELS
	for (auto& solBody : m_solarBodies)
		drawLabel(solBody->properName, solBody->getGlobalPosition());
}

void ofApp::drawLabel(std::string labelText, glm::vec3 labelPosition)
{
	glm::vec3 screenPosition = MainPlayer.getCamera()->worldToScreen(labelPosition);
	// There is need to check if z is smaller than 1 
	// to avoid displaying labels of objects that are behind observator
	if (screenPosition.z < 1)  
		BodyNameLabel.drawString(labelText, screenPosition.x, screenPosition.y);
}

//////////////////////////////////////////LISTENERS / CHECKERS//////////////////////////////////////

void ofApp::windowResized(int width, int height)
{
	// This allocates fbo index layer again if window is resized
	ColorIndexFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
}

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)
{
	if (e.target->is("VIEW MODE"))
	{
		if (e.checked == true)
			ProgramModeRef->setCamerMode(EARTH);
		else
			ProgramModeRef->setCamerMode(SPACE);

		MainPlayer.setCamera();
	}
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
	if (e.target->is("UPDATE"))
	{
		LocationPoint->setLocation(stof(gui_latitudeInput->getText()), stof(gui_longitudeInput->getText()));
		MainPlayer.resetPlayer();
	}
}

void ofApp::isMouseOnGui()
{
	if (MainGui.getVisible())
	{
		int guiLeft = (int)MainGui.getPosition().x;
		int guiRight = (int)MainGui.getPosition().x+MainGui.getWidth();		
		
		int guiTop = (int)MainGui.getPosition().y;
		int guiBottom = (int)MainGui.getPosition().x+MainGui.getHeight();

		int mouseX = ofGetMouseX();
		int mouseY = ofGetMouseY();

		if ((mouseX >= guiLeft && mouseX <= guiRight) && (mouseY >= guiTop && mouseY <= guiBottom))
			ProgramModeRef->setIsMouseOnGui(true);
		else
			ProgramModeRef->setIsMouseOnGui(false);
	}
	ProgramModeRef->setIsMouseOnGui(false);
}
bool ofApp::isMouseOnWindow()
{
	if (ofGetMouseX() > 0 && ofGetMouseX() < ofGetWidth() && ofGetMouseY() > 0 && ofGetMouseY() < ofGetHeight())
		return true;
	else
		return false;
}

void ofApp::mouseReleased(int x, int y, int button)
{
	if (button == OF_MOUSE_BUTTON_LEFT && !m_isMouseDragging && !ProgramModeRef->getIsMouseOnGui())
		select();

	m_isMouseDragging = false;
}

void ofApp::mouseDragged(int x, int y, int button)
{
	m_isMouseDragging = true;
}
//////////////////////////////////////////THREADS////////////////////////////////////////////////////

void ofApp::threadedFunction()
{
	setupStars();
}

//////////////////////////////////////////SELECTING//////////////////////////////////////////////////

void ofApp::select()
{
	if (m_isPreselectionActive)
	{
		m_isSelectionActive = true;
		m_selectedNode = m_preselectedNode;
	}
	else
	{
		m_isSelectionActive = false;
		m_selectedNode = nullptr;
	}
}

void ofApp::preselect()
{
	// This is invoked constantly in update to check if cursor is on selectable object
	m_isPreselectionActive = false;						// Reset preselection state
	m_preselectedNode = nullptr;						// Reset preselection state

	ofPixels indexFboPixels;
	ColorIndexFbo.readToPixels(indexFboPixels);			// Read fbo layer to pixels to make possible to check color under cursor
	ofColor indexColorToFind = COLOR_INDEX_BACKGROUND;	// Reset color to find to background color
	
	// Get pixel color under cursor on index fbo layer to compareit later with global objects register
	if(isMouseOnWindow())
		indexColorToFind = indexFboPixels.getColor(ofGetMouseX(), ofGetMouseY());
	
	// Find index color in register and get ptr to right object instance
	if (indexColorToFind != COLOR_INDEX_BACKGROUND)
	{
		m_preselectedNode = ProgramMode::getInstance()->find(indexColorToFind);
		if ((m_preselectedNode != nullptr && m_selectedNode == nullptr) || 
			(m_preselectedNode != nullptr && m_preselectedNode->getIndexColor() != m_selectedNode->getIndexColor()))
		{
				m_isPreselectionActive = true;
				m_preselectionPosition = m_preselectedNode->getGlobalPosition();
		}
	}
}