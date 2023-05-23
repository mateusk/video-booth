#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
  // setup the GL context
	ofSetupOpenGL(1024,768, OF_WINDOW);			
	ofRunApp( new ofApp());
}
