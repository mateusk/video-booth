#pragma once

#include "ofMain.h"
#include "ofxVideoRecorder.h"
#include "ofxXmlSettings.h"
#include "ofxTrueTypeFontUC.h"
#include "ofxTimer\src\ofxTimer.h"
#include "Config.h"
#include "Button.h"
#include "Person.h"

class ofApp : public ofBaseApp{
    
public:
  void setup();
  void update();
  void draw();
  void exit();
  
  void keyReleased(int key);
  void audioIn(float * input, int bufferSize, int nChannels);

	void buttonClicked(int &buttonId);

	void returnToInitialScreen(unsigned int &e);

	void stopRecording(unsigned int &e);

	void updateRegisteredVisitorList();
    
	bool isRecording;
	bool cameraSetDeviceError;
	bool connectionError;
	bool showRecordingText;
	bool namesUpdated;

	int appStateIndex;

	int cameraInfoWindowHeight;
	int cameraInfoWindowWidth;

	int cameraInfoWindowXPos;
	int cameraInfoWindowYPos;

	int countdown;
	int recordingMinutes;
	
	float initTime;

	string camNotFoundMsg;
	string connectionErrorMsg;

	string currentFPS;

	string currLang;
	string btnLabelFontPath;

	string currPersonName;
	string currVideoFileName;
	string currAudioFileName;
	string fileName;

	string recMinutesStr;
	string recSecondsStr;
    
  ofFbo recordFbo;
  ofPixels recordPixels;

	ofImage bgImg;
	ofImage diagonalLinesImg;
	ofImage expoTitleImg;
	ofImage rockHandImg;
	ofImage sponsorsImg;

	ofxTrueTypeFontUC buttonLabelFont;
	ofxTrueTypeFontUC bigTextFont;
	ofxTrueTypeFontUC smallTextFont;
	ofTrueTypeFont countdownTextFont;
	ofTrueTypeFont recordingTimeFont;
	ofTrueTypeFont msgFont;

	ofVideoGrabber vidGrabber;

	ofTexture bwVideo;
	ofImage bwVideoImage;

	ofxVideoRecorder vidRecorder;

	ofSoundPlayer  soundPlayer;

	ofxTimer returnToInitScreenTimer;
	ofxTimer recordTimer;

	ofXml registeredPeopleXml;

	ofXml videoPlaylist;

	Button * portLangBtn;
	Button * engLangBtn;
	Button * simBtn;
	Button * naoBtn;
	Button * yesBtn;
	Button * noBtn;
	Button * lithiumBtn;
	Button * loveBuzzBtn;
	Button * smellsLikeBtn;
	Button * comeAsBtn;
	Button * gravarBtn;
	Button * recordBtn;
	Button * voltarBtn;
	Button * returnBtn;

	vector <Button *> peopleNamesBtns;
	vector <Person *> lastRegisteredPeople;

	vector <string> cameraDevices;
};
