#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	Config::loadConfig("config.xml");
	Config::loadVideoPlaylist("videoPlaylist.xml");

	//Basic configuration:
	ofSetWindowPosition(Config::window.x, Config::window.y);
	ofSetWindowShape(Config::window.width, Config::window.height);
	ofSetFullscreen(Config::windowFullscreen);
	ofSetFrameRate(60);

	numOfVideos = Config::videoPlaylist.size();

	videoLoaded = false;
	videoIndex = 0;

	reloadPlaylistCounter = 0;

	ofTrueTypeFont::setGlobalDpi(72);

	noVideosMsg.load("fonts/verdana.ttf", 48, true, true);

	if (numOfVideos > 0) {
		selectVideo();
	}

	else {
		if (Config::debugMode) {
			cout << "--> Video playlist empty. The list will be reloaded in 10 seconds..." << endl;
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	video.update();

	if (numOfVideos == 0) {
		reloadPlaylistCounter++;

		if (reloadPlaylistCounter >= 600) {
			if (Config::debugMode) {
				cout << "--> Trying to load video playlist again..." << endl;
			}
			Config::videoPlaylist.clear();
			Config::loadVideoPlaylist("videoPlaylist.xml");
			numOfVideos = Config::videoPlaylist.size();
			reloadPlaylistCounter = 0;
			if (numOfVideos > 0) {
				selectVideo();
			}
		}
	}

	else {
		if (videoLoaded && video.getIsMovieDone()) {
			if (Config::debugMode) {
				cout << "--> Loading video playlist..." << endl;
			}

			Config::videoPlaylist.clear();
			videoLoaded = false;

			Config::loadVideoPlaylist("videoPlaylist.xml");

			if (numOfVideos != Config::videoPlaylist.size()) {
				numOfVideos = Config::videoPlaylist.size();
				videoIndex = Config::videoPlaylist.size() - 1;

				if (Config::debugMode) {
					cout << "--> New video in playlist, playing it next: videoIndex = " << videoIndex << endl;
				}
			}

			if (videoIndex >= Config::videoPlaylist.size()) {
				videoIndex = 0;

				if (Config::debugMode) {
					cout << "--> Video playlist was fully played, returning videoIndex to " << videoIndex << endl;
				}
			}

			selectVideo();
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(0);

	if (videoLoaded) {
		video.draw(0, 0, Config::video.width, Config::video.height);
	}
	
	if (Config::debugMode) {
		ofSetColor(255);
		ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate()), 20, 20);
		ofDrawBitmapStringHighlight("Frame: " + ofToString(video.getCurrentFrame()) + "/" + ofToString(video.getTotalNumFrames()), 20, 40);
		ofDrawBitmapStringHighlight("Duration: " + ofToString(video.getPosition()*video.getDuration(), 2) + "/" + ofToString(video.getDuration(), 2), 20, 60);
	}

	if (numOfVideos == 0) {
		string msg = "Playlist de videos vazia. Aguardando a gravacao de novos videos...";
		noVideosMsg.drawString(msg,
			Config::window.width/2 - noVideosMsg.getStringBoundingBox(msg, 0, 0).width/2, 
			Config::window.height / 2 - noVideosMsg.getStringBoundingBox(msg, 0, 0).height / 2);
	}
}

//--------------------------------------------------------------
void ofApp::selectVideo() {
	if (Config::debugMode) {
		cout << "--> Selecting video with index " << videoIndex << ": " << Config::videoPlaylist.at(videoIndex) << endl;
	}
	selectedVideoPath = Config::videoPlaylist.at(videoIndex);
	video.load("videos/" + selectedVideoPath);
	video.setLoopState(OF_LOOP_NONE);
	video.play();
	videoLoaded = true;
	videoIndex++;
}

//--------------------------------------------------------------
void ofApp::exit() {
	if (video.isPlaying()) {
		video.close();
	}
}
