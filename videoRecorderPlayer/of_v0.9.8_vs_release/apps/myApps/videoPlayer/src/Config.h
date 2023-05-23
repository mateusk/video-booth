#pragma once

#include "ofMain.h"
#include "ofxXmlSettings\src\ofxXmlSettings.h"

class Config {
public:
	static void loadConfig(string configXml);
	static void loadVideoPlaylist(string audioDescXml);

	//from xml:
	static bool log;
	static ofRectangle window;
	static ofRectangle video;
	static bool windowFullscreen;
	static bool debugMode;

	static float initVolume;

	static vector <string> videoPlaylist;
};
