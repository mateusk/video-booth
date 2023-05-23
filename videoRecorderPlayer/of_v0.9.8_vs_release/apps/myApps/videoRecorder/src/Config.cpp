#include "Config.h"

//from xml
bool Config::log = false;
ofRectangle Config::window;
ofRectangle Config::video;
bool Config::windowFullscreen;
bool Config::debugMode = false;

string Config::FFMPEGLocation = "C:\FFMPEG";

string Config::videoPlaylistXmlPath = "";

string Config::videoFileExtension = ".mp4";
string Config::videoCodec = "mpeg4";
string Config::videoBitrate = "5000k";

float Config::initVolume = 0.5;

int Config::videoFrameRate = 15;

string Config::cameraDeviceName = "";

string Config::videosFolderPath = "";
string Config::networkVideosFolderPath = "";
string Config::audiosFolderPath = "";

string Config::registeredPeopleXmlPath = "";

void Config::loadConfig(string configXml) {
	ofxXmlSettings xml;
	cout << "--> Loading config XML..." << endl;
	if (xml.loadFile(configXml)) {
		xml.pushTag("data", 0);
			log = xml.getValue("log", 0) == 1;
			window.x = xml.getAttribute("window", "x", 0, 0);
			window.y = xml.getAttribute("window", "y", 0, 0);
			window.width = xml.getAttribute("window", "width", 0, 0);
			window.height = xml.getAttribute("window", "height", 0, 0);
			windowFullscreen = xml.getAttribute("window", "fullscreen", "true", 0) == "true";
			video.width = xml.getAttribute("video", "width", 0, 0);
			video.height = xml.getAttribute("video", "height", 0, 0);
			cameraDeviceName = xml.getValue("cameraDeviceName", cameraDeviceName);
			FFMPEGLocation = xml.getValue("FFMPEGLocation", FFMPEGLocation);
			videoPlaylistXmlPath = xml.getValue("videoPlaylistXmlPath", videoPlaylistXmlPath);
			videoFileExtension = xml.getValue("videoFileExtension", videoFileExtension);
			videoCodec = xml.getValue("videoCodec", videoCodec);
			videoBitrate = xml.getValue("videoBitrate", videoBitrate);
			initVolume = xml.getValue("initVolume", initVolume);
			videoFrameRate = xml.getValue("videoFrameRate", videoFrameRate);
			debugMode = xml.getValue("debugMode", 0) == 1;
			videosFolderPath = xml.getValue("videosFolderPath", videosFolderPath);
			networkVideosFolderPath = xml.getValue("networkVideosFolderPath", networkVideosFolderPath);
			audiosFolderPath = xml.getValue("audiosFolderPath", audiosFolderPath);
			registeredPeopleXmlPath = xml.getValue("registeredPeopleXmlPath", registeredPeopleXmlPath);
		xml.popTag();
	}
	else {
		cout << "--> Erro ao carregar o XML de configuracao: " << configXml << endl;
		cout << "--> Pressione ENTER para sair." << endl;
		int anything;
		cin >> anything;
		std::exit(1);
	}
};