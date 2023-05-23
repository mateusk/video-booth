#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){
  ofAppGlutWindow window;
  // setup the GL context
	ofSetupOpenGL(&window, 1280, 720, OF_WINDOW);			
	ofRunApp( new ofApp());
}
