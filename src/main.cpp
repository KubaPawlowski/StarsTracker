// Main is just main ;)
// Author: Kuba Pawlowski

#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"
#include "ProgramMode.h"

// Singleton declaration, instance to keep track of global stuff
ProgramMode* ProgramMode::sInstance = 0;

int main() {
	ofSetupOpenGL(1920 / 2, 1080 / 2, OF_WINDOW);

	std::cout << "Program started at: " << ofGetTimestampString() << std::endl;
	ofRunApp(new ofApp());
}
