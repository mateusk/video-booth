#include "Config.h"

//from xml
bool Config::log = false;
ofRectangle Config::window;
ofRectangle Config::video;
bool Config::windowFullscreen;
bool Config::debugMode = false;

float Config::initVolume = 0.5;

vector <string> Config::videoPlaylist;

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
		initVolume = xml.getValue("initVolume", initVolume);
		debugMode = xml.getValue("debugMode", 0) == 1;
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

void Config::loadVideoPlaylist(string playlistXml) {
	cout << "--> Loading XML with video paths..." << endl;
	ofxXmlSettings xml;
	if (xml.loadFile(playlistXml)) {
		cout << "--> Video playlist loaded! Reading video paths..." << endl;
		xml.pushTag("videos");
		int numVideos = xml.getNumTags("video");
		cout << "--> Found " << ofToString(numVideos) << " videos in playlist:" << endl;
		for (int i = 0; i < numVideos; i++) {
			videoPlaylist.push_back(xml.getValue("video", "", i));
		}
		xml.popTag();
		for (int i = videoPlaylist.size() - 1; i >= 0; i--) {
			cout << "--> " << videoPlaylist[i] << endl;
		}
	}
	else {
		cout << "--> Erro ao carregar o XML com a lista de videos: " << playlistXml << endl;
		cout << "--> Pressione ENTER para sair." << endl;
		int anything;
		cin >> anything;
		std::exit(1);
	}
};
