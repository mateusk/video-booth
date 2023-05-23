#pragma once

#include "ofMain.h"
#include "ofxXmlSettings\src\ofxXmlSettings.h"
#include "Config.h"

class ofApp : public ofBaseApp {

public:

	void setup();
	void update();
	void draw();
	void exit();

	void selectVideo();

	string selectedVideoPath;

	int videoIndex;
	int numOfVideos;
	int reloadPlaylistCounter;

	bool videoLoaded;

	ofVideoPlayer video;

	ofTrueTypeFont noVideosMsg;
};
