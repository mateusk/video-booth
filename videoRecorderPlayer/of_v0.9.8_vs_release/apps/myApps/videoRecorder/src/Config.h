#ifndef _CONFIG
#define _CONFIG
#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

class Config {
public:
	static void loadConfig(string configXml);

	//from xml:
	static bool log;
	static ofRectangle window;
	static ofRectangle video;
	static bool windowFullscreen;
	static bool debugMode;
	static float initVolume;
	static string videoPlaylistXmlPath;
	static string FFMPEGLocation;
	static string videoFileExtension;
	static string videoCodec;
	static string videoBitrate;
	static string cameraDeviceName;
	static string videosFolderPath;
	static string networkVideosFolderPath;
	static string audiosFolderPath;
	static string registeredPeopleXmlPath;
	static int videoFrameRate;
};
#endif