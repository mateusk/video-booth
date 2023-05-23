#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	Config::loadConfig("config.xml");

	//Basic configuration:
	ofSetWindowPosition(Config::window.x, Config::window.y);
	ofSetWindowShape(Config::window.width, Config::window.height);
	ofSetFullscreen(Config::windowFullscreen);
	ofSetFrameRate(60);

	vidGrabber.setVerbose(false);

	ofHideCursor();

	if (Config::debugMode) {
		ofShowCursor();
	}

	cameraSetDeviceError = true;
	namesUpdated = false;

	//Setting up camera device and video recording settings:
	#ifdef TARGET_WIN32
	vidRecorder.setFfmpegLocation(Config::FFMPEGLocation); // custom FFMPEG location
	#endif

	vidRecorder.setVideoCodec(Config::videoCodec);
	vidRecorder.setVideoBitrate(Config::videoBitrate);

	for (int i = 0; i < vidGrabber.listDevices().size(); i++) {
		if (Config::debugMode) {
			cout << " ----> [videoDevice at " << i << "]: " << vidGrabber.listDevices().at(i).deviceName << " -- " << Config::cameraDeviceName << endl;
		}

		cameraDevices.push_back(vidGrabber.listDevices().at(i).deviceName);

		if (vidGrabber.listDevices().at(i).deviceName == Config::cameraDeviceName) {
			if (Config::debugMode) {
				cout << "" << endl;
				cout << " ----> OK! Setting up video device at [" << i << "]: " << vidGrabber.listDevices().at(i).deviceName << endl;
				cout << "" << endl;
			}
			vidGrabber.setDeviceID(i);
			vidGrabber.setDesiredFrameRate(30);
			vidGrabber.initGrabber(Config::video.width, Config::video.height);
			cameraSetDeviceError = false;

			bwVideo.allocate(Config::video.width, Config::video.height, GL_LUMINANCE);
		}
	}

    isRecording = false;
	showRecordingText = false;

	ofBackground(0);

	msgFont.load("fonts/Tw_Cen_MT_Bold.ttf", 30, true, false);
	msgFont.setLineHeight(20.0f);
	msgFont.setLetterSpacing(1.035);

	camNotFoundMsg = "Camera nao encontrada.";
	connectionErrorMsg = "Falha na conexao com player de video. Revise as conexoes e reinicie o aplicativo.";

	cameraInfoWindowHeight = (vidGrabber.listDevices().size() + 1) * 30;
	cameraInfoWindowWidth = 300;
	cameraInfoWindowXPos = 50;
	cameraInfoWindowYPos = 80;

	appStateIndex = 0;
	currLang = "port";

	bgImg.loadImage("imgs/bg.jpg");
	diagonalLinesImg.loadImage("imgs/diagonal_lines.png");
	expoTitleImg.loadImage("imgs/expo_title.png");
	rockHandImg.loadImage("imgs/rockhand.png");
	sponsorsImg.loadImage("imgs/sponsors.png");

	smallTextFont.load("fonts/Tw_Cen_MT.ttf", 20, true, false);
	bigTextFont.load("fonts/Tw_Cen_MT.ttf", 35, true, false);
	countdownTextFont.load("fonts/SamsungIF_Rg.ttf", 230, true, false);
	recordingTimeFont.load("fonts/SamsungIF_Rg.ttf", 30, true, false);

	//Creating buttons:

	btnLabelFontPath = "fonts/Tw_Cen_MT_Bold.ttf";

	portLangBtn = new Button ("PORTUGU�S", btnLabelFontPath, 20, 331, 50, 623, 688, 1);
	engLangBtn = new Button ("ENGLISH", btnLabelFontPath, 20, 331, 50, 964, 688, 2);
	simBtn = new Button("SIM", btnLabelFontPath, 20, 90, 65, 846, 678, 3);
	naoBtn = new Button("N�O", btnLabelFontPath, 20, 90, 65, 978, 678, 4);
	yesBtn = new Button("YES", btnLabelFontPath, 20, 90, 65, 846, 678, 5);
	noBtn = new Button("NO", btnLabelFontPath, 20, 90, 65, 978, 678, 6);
	lithiumBtn = new Button("LITHIUM", btnLabelFontPath, 20, 496, 65, 451, 459, 7);
	loveBuzzBtn = new Button("LOVE BUZZ", btnLabelFontPath, 20, 496, 65, 973, 459, 8);
	smellsLikeBtn = new Button("SMELLS LIKE TEEN SPIRIT", btnLabelFontPath, 20, 496, 65, 451, 558, 9);
	comeAsBtn = new Button("COME AS YOU ARE", btnLabelFontPath, 20, 496, 65, 973, 558, 10);
	gravarBtn = new Button("GRAVAR", btnLabelFontPath, 20, 446, 65, 737, 662, 11);
	recordBtn = new Button("RECORD", btnLabelFontPath, 20, 446, 65, 737, 662, 12);
	voltarBtn = new Button("VOLTAR", btnLabelFontPath, 20, 150, 65, 80, 950, 13);
	returnBtn = new Button("RETURN", btnLabelFontPath, 20, 150, 65, 80, 950, 14);

	portLangBtn->setEnabled(true);
	engLangBtn->setEnabled(true);
	simBtn->setEnabled(false);
	naoBtn->setEnabled(false);
	yesBtn->setEnabled(false);
	noBtn->setEnabled(false);
	lithiumBtn->setEnabled(false);
	loveBuzzBtn->setEnabled(false);
	smellsLikeBtn->setEnabled(false);
	comeAsBtn->setEnabled(false);
	gravarBtn->setEnabled(false);
	recordBtn->setEnabled(false);
	voltarBtn->setEnabled(false);
	returnBtn->setEnabled(false);

	currPersonName = "NOME DA PESSOA";

	fileName = "video";

	countdown = 11;
	initTime = 0;
	recordingMinutes = 0;

	ofEnableSmoothing();

	ofAddListener(Button::mouseClickEvent, this, &ofApp::buttonClicked);

	ofAddListener(returnToInitScreenTimer.TIMER_REACHED, this, &ofApp::returnToInitialScreen);

	ofAddListener(recordTimer.TIMER_REACHED, this, &ofApp::stopRecording);

	updateRegisteredVisitorList();

	connectionError = true;
	if (videoPlaylist.load(Config::videoPlaylistXmlPath)){
		connectionError = false;
	}
}

//----------------------------------------------------------------------------------------------------------------
void ofApp::exit() {
	if (vidRecorder.isRecording()) {
			if (Config::debugMode) {
		cout << "---> Closing the ffmpeg recorder..." << endl;
	}
	vidRecorder.close();

	appStateIndex = 0;
	portLangBtn->setEnabled(true);
	engLangBtn->setEnabled(true);

	soundPlayer.stop();
	soundPlayer.unloadSound();

	string fullVideoFilePath = ofFilePath::getAbsolutePath(ofToDataPath(Config::videosFolderPath + currVideoFileName + Config::videoFileExtension));
	string fullAudioFilePath = ofFilePath::getAbsolutePath(ofToDataPath(Config::audiosFolderPath + currAudioFileName));
	string pathToTransferVideo = Config::networkVideosFolderPath + currVideoFileName + Config::videoFileExtension;

	cout << "--->" << fullVideoFilePath << endl;
	cout << "--->" << fullAudioFilePath << endl;
	cout << "--->" << pathToTransferVideo << endl;

	if (Config::debugMode) {
		cout << "---> Muxing recorded video with chosen audio..." << endl;
	}
	string cmd = "ffmpeg -i " + 
		fullVideoFilePath +
		" -i " + fullAudioFilePath +
		" -c:v copy -c:a aac " +
		ofToDataPath(Config::videosFolderPath + "output.mp4");
	system(cmd.c_str());

	if (Config::debugMode) {
		cout << "---> Success! Deleting old video file..." << endl;
	}
	cmd = "del " + Config::videosFolderPath + currVideoFileName + Config::videoFileExtension;
	system(cmd.c_str());

	if (Config::debugMode) {
		cout << "---> Success! Renaming new muxed file to " + currVideoFileName + Config::videoFileExtension << endl;
	}
	cmd = "rename " + Config::videosFolderPath + "output.mp4 " + currVideoFileName + Config::videoFileExtension;
	system(cmd.c_str());

	if (Config::debugMode) {
		cout << "---> Success! Now copying file to " + Config::networkVideosFolderPath << endl;
	}
	
	std::ifstream  src(fullVideoFilePath, std::ios::binary);
	std::ofstream  dst(pathToTransferVideo, std::ios::binary);
	dst << src.rdbuf();

	videoPlaylist.setTo("videos");
	videoPlaylist.addChild("video");
	videoPlaylist.setValue("video[" + ofToString(videoPlaylist.getNumChildren() - 1) + "]", currVideoFileName + Config::videoFileExtension);
	videoPlaylist.save(Config::videoPlaylistXmlPath);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	currentFPS = ofToString(ofGetFrameRate());
	vidGrabber.update();

	if (soundPlayer.getIsPlaying()) {
		ofSoundUpdate();
	}

	if (!connectionError) {

		if (appStateIndex == 2 && !namesUpdated) {
			peopleNamesBtns.clear();
			//Update button labels with last 10 names from XML:
			for (int i = 0; i < lastRegisteredPeople.size(); i++) {
				int x = 0;
				int y = 0;

				if (i < 5) {
					x = 432;
				}
				if (i >= 5) {
					x = 992;
				}
				if (i == 0 || i == 5) {
					y = 405;
				}
				if (i == 1 || i == 6) {
					y = 503;
				}
				if (i == 2 || i == 7) {
					y = 602;
				}
				if (i == 3 || i == 8) {
					y = 701;
				}
				if (i == 4 || i == 9) {
					y = 800;
				}


				Button * btn = new Button(lastRegisteredPeople.at(i)->getName(), btnLabelFontPath, 20, 496, 65, x, y, i + 15);
				btn->setEnabled(true);
				peopleNamesBtns.push_back(btn);
			}

			namesUpdated = true;
		}

		if (appStateIndex == 5 && !cameraSetDeviceError && !isRecording) {
			bwVideo.loadData(vidGrabber.getPixelsRef());
			ofPixels bwVideoPixels;
			bwVideo.readToPixels(bwVideoPixels);
			bwVideoImage.setFromPixels(bwVideoPixels);

			if (ofGetElapsedTimeMillis() - initTime >= 1000) {
				countdown--;
				initTime = ofGetElapsedTimeMillis();
				if (countdown == 0) {
					fileName = Config::videosFolderPath + currVideoFileName;
					vidRecorder.setup(fileName, Config::video.width, Config::video.height, 30, 0, 0, true);

					// Start recording
					vidRecorder.start();

					initTime = ofGetElapsedTimeMillis();

					isRecording = true;
					countdown = 11;

					soundPlayer.play();

					recordTimer.setup(120000, false);
				}
			}
		}

		if (vidRecorder.isRecording()) {
			vidRecorder.setClock(12000 - recordTimer.getTimeLeftInMillis());

			if (vidGrabber.isFrameNew()) {
				bool success = vidRecorder.addFrame(vidGrabber.getPixelsRef());
				if (!success) {
					ofLogWarning("This frame was not added!");
				}
			}
		}

		if (isRecording) {
			int minutes = 0;
			int seconds = 0;

			if (recordTimer.getTimeLeftInSeconds() >= 60) {
				minutes = 1;
				seconds = recordTimer.getTimeLeftInSeconds() - 60;
			}

			if (recordTimer.getTimeLeftInSeconds() < 60) {
				minutes = 0;
				seconds = recordTimer.getTimeLeftInSeconds();
			}

			recMinutesStr = ofToString(minutes);
			recSecondsStr = ofToString(seconds);

			if (seconds < 10) {
				recSecondsStr = "0" + ofToString(seconds);
			}
		}

		if (appStateIndex == 6 && !cameraSetDeviceError && !isRecording) {
			bwVideo.loadData(vidGrabber.getPixelsRef());
			ofPixels bwVideoPixels;
			bwVideo.readToPixels(bwVideoPixels);
			bwVideoImage.setFromPixels(bwVideoPixels);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);

	//--------------------------------------- START SCREEN -------------------------------------------

	if (appStateIndex == 0 && !cameraSetDeviceError && !connectionError) {
		ofSetColor(255);
		bgImg.draw(0, 0, bgImg.getWidth(), bgImg.getHeight());
		expoTitleImg.draw(835, 101, expoTitleImg.getWidth(), expoTitleImg.getHeight());
		sponsorsImg.draw(617, 914, sponsorsImg.getWidth(), sponsorsImg.getHeight());

		portLangBtn->drawButton();
		engLangBtn->drawButton();
	}

	//------------------------------------ CONFIRM NAME SCREEN ----------------------------------------

	if (appStateIndex == 1 && currLang == "port" && !cameraSetDeviceError && !connectionError) {
		ofSetColor(255);
		bgImg.draw(0, 0, bgImg.getWidth(), bgImg.getHeight());
		expoTitleImg.draw(864, 100, 190, 343);
		diagonalLinesImg.draw(-213, 689, diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());
		diagonalLinesImg.draw(2133, 689, -diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());
		sponsorsImg.draw(617, 914, sponsorsImg.getWidth(), sponsorsImg.getHeight());

		bigTextFont.drawString("VOC� �",
								Config::window.width/2 - bigTextFont.getStringBoundingBox("VOC� �", 0, 0).width/2,
								580);
		bigTextFont.drawString(currPersonName + "?",
								Config::window.width / 2 - bigTextFont.getStringBoundingBox(currPersonName +"?", 0, 0).width / 2,
								630);

		simBtn->drawButton();
		naoBtn->drawButton();

		voltarBtn->drawButton();
	}

	if (appStateIndex == 1 && currLang == "eng" && !cameraSetDeviceError && !connectionError) {
		ofSetColor(255);
		bgImg.draw(0, 0, bgImg.getWidth(), bgImg.getHeight());
		expoTitleImg.draw(864, 100, 190, 343);
		diagonalLinesImg.draw(-213, 689, diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());
		diagonalLinesImg.draw(2133, 689, -diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());
		sponsorsImg.draw(617, 914, sponsorsImg.getWidth(), sponsorsImg.getHeight());

		bigTextFont.drawString("ARE YOU",
			Config::window.width / 2 - bigTextFont.getStringBoundingBox("ARE YOU", 0, 0).width / 2,
			580);
		bigTextFont.drawString(currPersonName + "?",
			Config::window.width / 2 - bigTextFont.getStringBoundingBox(currPersonName + "?", 0, 0).width / 2,
			630);

		yesBtn->drawButton();
		noBtn->drawButton();

		returnBtn->drawButton();
	}

	//--------------------------------- SELECT CORRECT NAME SCREEN -------------------------------------

	if (appStateIndex == 2 && currLang == "port" && !cameraSetDeviceError && !connectionError) {
		ofSetColor(255);
		bgImg.draw(0, 0, bgImg.getWidth(), bgImg.getHeight());
		diagonalLinesImg.draw(-380, 170, diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());
		diagonalLinesImg.draw(2300, 170, -diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());

		bigTextFont.drawString("QUAL",
								Config::window.width / 2 - bigTextFont.getStringBoundingBox("QUAL", 0, 0).width / 2,
								175);
		bigTextFont.drawString("O SEU NOME?",
								Config::window.width / 2 - bigTextFont.getStringBoundingBox("O SEU NOME?", 0, 0).width / 2,
								230);

		if (peopleNamesBtns.size() > 0 && namesUpdated) {
			for (int i = 0; i < peopleNamesBtns.size(); i++) {
				peopleNamesBtns.at(i)->drawButton();
			}
		}

		voltarBtn->drawButton();
	}

	if (appStateIndex == 2 && currLang == "eng" && !cameraSetDeviceError && !connectionError) {
		ofSetColor(255);
		bgImg.draw(0, 0, bgImg.getWidth(), bgImg.getHeight());
		diagonalLinesImg.draw(-380, 170, diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());
		diagonalLinesImg.draw(2300, 170, -diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());

		bigTextFont.drawString("WHAT",
			Config::window.width / 2 - bigTextFont.getStringBoundingBox("WHAT", 0, 0).width / 2,
			175);
		bigTextFont.drawString("IS YOUR NAME?",
			Config::window.width / 2 - bigTextFont.getStringBoundingBox("IS YOUR NAME?", 0, 0).width / 2,
			230);

		if (peopleNamesBtns.size() > 0 && namesUpdated) {
			for (int i = 0; i < peopleNamesBtns.size(); i++) {
				peopleNamesBtns.at(i)->drawButton();
			}
		}

		returnBtn->drawButton();
	}

	//----------------------------------- SELECT SONG SCREEN ---------------------------------------

	if (appStateIndex == 3 && currLang == "port" && !cameraSetDeviceError && !connectionError) {
		ofSetColor(255);
		bgImg.draw(0, 0, bgImg.getWidth(), bgImg.getHeight());
		diagonalLinesImg.draw(-380, 170, diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());
		diagonalLinesImg.draw(2300, 170, -diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());

		bigTextFont.drawString("ESCOLHA",
								Config::window.width / 2 - bigTextFont.getStringBoundingBox("ESCOLHA", 0, 0).width / 2,
								175);
		bigTextFont.drawString("SUA M�SICA",
								Config::window.width / 2 - bigTextFont.getStringBoundingBox("SUA M�SICA", 0, 0).width / 2,
								230);

		lithiumBtn->drawButton();
		smellsLikeBtn->drawButton();
		loveBuzzBtn->drawButton();
		comeAsBtn->drawButton();

		voltarBtn->drawButton();
	}

	if (appStateIndex == 3 && currLang == "eng" && !cameraSetDeviceError && !connectionError) {
		ofSetColor(255);
		bgImg.draw(0, 0, bgImg.getWidth(), bgImg.getHeight());
		diagonalLinesImg.draw(-380, 170, diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());
		diagonalLinesImg.draw(2300, 170, -diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());

		bigTextFont.drawString("PICK",
			Config::window.width / 2 - bigTextFont.getStringBoundingBox("PICK", 0, 0).width / 2,
			175);
		bigTextFont.drawString("YOUR SONG",
			Config::window.width / 2 - bigTextFont.getStringBoundingBox("YOUR SONG", 0, 0).width / 2,
			230);

		lithiumBtn->drawButton();
		smellsLikeBtn->drawButton();
		loveBuzzBtn->drawButton();
		comeAsBtn->drawButton();

		returnBtn->drawButton();
	}

	//----------------------------------- START RECORD SCREEN ----------------------------------------

	if (appStateIndex == 4 && currLang == "port" && !cameraSetDeviceError && !connectionError) {
		ofSetColor(255);
		bgImg.draw(0, 0, bgImg.getWidth(), bgImg.getHeight());
		diagonalLinesImg.draw(-380, 170, diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());
		diagonalLinesImg.draw(2300, 170, -diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());

		bigTextFont.drawString("VOC� TEM DOIS MINUTOS PARA FAZER",
								Config::window.width / 2 - bigTextFont.getStringBoundingBox("VOC� TEM DOIS MINUTOS PARA FAZER", 0, 0).width / 2,
								150);
		bigTextFont.drawString("SEU V�DEO. FIQUE ATENTO",
								Config::window.width / 2 - bigTextFont.getStringBoundingBox("SEU V�DEO. FIQUE ATENTO", 0, 0).width / 2,
								200);
		bigTextFont.drawString("AO CRON�METRO!",
								Config::window.width / 2 - bigTextFont.getStringBoundingBox("AO CRON�METRO!", 0, 0).width / 2,
								250);

		smallTextFont.drawString("CASO QUEIRA FAZER OUTRO V�DEO � NECESS�RIO FAZER",
								Config::window.width / 2 - smallTextFont.getStringBoundingBox("CASO QUEIRA FAZER OUTRO V�DEO � NECESS�RIO FAZER", 0, 0).width / 2,
								350);

		smallTextFont.drawString("UM NOVO CADASTRO. TOQUE NO BOT�O GRAVAR QUANDO ESTIVER",
								Config::window.width / 2 - smallTextFont.getStringBoundingBox("UM NOVO CADASTRO. TOQUE NO BOT�O GRAVAR QUANDO ESTIVER", 0, 0).width / 2,
								 385);

		smallTextFont.drawString("PRONTO E AGUARDE A CONTAGEM DE 10 SEGUNDOS.",
								Config::window.width / 2 - smallTextFont.getStringBoundingBox("PRONTO E AGUARDE A CONTAGEM DE 10 SEGUNDOS.", 0, 0).width / 2,
								 420);

		rockHandImg.draw(785, 535, rockHandImg.getWidth(), rockHandImg.getHeight());

		gravarBtn->drawButton();

		voltarBtn->drawButton();
	}

	if (appStateIndex == 4 && currLang == "eng" && !cameraSetDeviceError && !connectionError) {
		ofSetColor(255);
		bgImg.draw(0, 0, bgImg.getWidth(), bgImg.getHeight());
		diagonalLinesImg.draw(-380, 170, diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());
		diagonalLinesImg.draw(2300, 170, -diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());

		bigTextFont.drawString("YOU HAVE TWO MINUTES",
			Config::window.width / 2 - bigTextFont.getStringBoundingBox("YOU HAVE TWO MINUTES", 0, 0).width / 2,
			150);
		bigTextFont.drawString("TO RECORD YOUR VIDEO.",
			Config::window.width / 2 - bigTextFont.getStringBoundingBox("TO RECORD YOUR VIDEO.", 0, 0).width / 2,
			200);
		bigTextFont.drawString("MANAGE YOUR TIME!",
			Config::window.width / 2 - bigTextFont.getStringBoundingBox("MANAGE YOUR TIME!", 0, 0).width / 2,
			250);

		smallTextFont.drawString("IF YOU WANT TO RECORD ANOTHER VIDEO, YOU MUST REGISTER AGAIN.",
			int(Config::window.width / 2 - smallTextFont.getStringBoundingBox("IF YOU WANT TO RECORD ANOTHER VIDEO, YOU MUST REGISTER AGAIN.", 0, 0).width / 2),
			350);

		smallTextFont.drawString("TOUCH THE RECORD BUTTON WHEN YOU ARE READY",
			int(Config::window.width / 2 - smallTextFont.getStringBoundingBox("TOUCH THE RECORD BUTTON WHEN YOU ARE READY", 0, 0).width / 2),
			385);

		smallTextFont.drawString("AND WAIT FOR THE 10 SECOND COUNTDOWN.",
			int(Config::window.width / 2 - smallTextFont.getStringBoundingBox("AND WAIT FOR THE 10 SECOND COUNTDOWN.", 0, 0).width / 2),
			420);

		rockHandImg.draw(785, 535, rockHandImg.getWidth(), rockHandImg.getHeight());

		recordBtn->drawButton();

		returnBtn->drawButton();
	}

	//----------------------------------- COUNTDOWN SCREEN & RECORD ----------------------------------------

	if (appStateIndex == 5 && !cameraSetDeviceError && !connectionError) {
		if (!isRecording) {
			ofSetColor(255);
			bwVideoImage.draw(0, 0, Config::window.width, Config::window.height);

			ofSetColor(0, 0, 0, 100);
			ofDrawRectangle(0, 0, Config::window.width, Config::window.height);

			ofSetColor(255);
			diagonalLinesImg.draw(-380, 517, diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());
			diagonalLinesImg.draw(2300, 517, -diagonalLinesImg.getWidth(), diagonalLinesImg.getHeight());

			ofSetColor(0, 159, 227);

			if (countdown < 10) {
				countdownTextFont.drawString("00:0" + ofToString(countdown), 600, 630);
			}

			if (countdown >= 10) {
				countdownTextFont.drawString("00:" + ofToString(countdown), 600, 630);
			}
		}

		if (isRecording) {
			ofSetColor(255);
			vidGrabber.draw(0, 0, Config::window.width, Config::window.height);

			ofSetColor(255, 0, 0);
			ofCircle(Config::window.width / 2 - 130, Config::window.height - 140, 20);
			
			ofSetColor(0);
			recordingTimeFont.drawString("0" + recMinutesStr + ":" + recSecondsStr,
				Config::window.width / 2 - recordingTimeFont.getStringBoundingBox("0" + recMinutesStr + ":" + recSecondsStr, 0, 0).width / 2 - 2,
				Config::window.height - 73);

			ofSetColor(255);
			recordingTimeFont.drawString("0" + recMinutesStr + ":" + recSecondsStr,
				Config::window.width / 2 - recordingTimeFont.getStringBoundingBox("0" + recMinutesStr + ":" + recSecondsStr, 0, 0).width / 2,
				Config::window.height - 75);


			if (ofGetFrameNum() % 29 == 0) {
				showRecordingText = !showRecordingText;
			}

			if (showRecordingText) {
				if (currLang == "port") {
					ofSetColor(0);
					msgFont.drawString("GRAVANDO", Config::window.width / 2 - 87, Config::window.height - 126);
					ofSetColor(255);
					msgFont.drawString("GRAVANDO", Config::window.width / 2 - 85, Config::window.height - 128);
				}

				if (currLang == "eng") {
					ofSetColor(0);
					msgFont.drawString("RECORDING", Config::window.width / 2 - 87, Config::window.height - 126);
					ofSetColor(255);
					msgFont.drawString("RECORDING", Config::window.width / 2 - 85, Config::window.height - 128);
				}
			}
		}
	}

	if (appStateIndex == 6) {
		ofSetColor(255);
		bwVideoImage.draw(0, 0, Config::window.width, Config::window.height);

		ofSetColor(0, 0, 0, 100);
		ofDrawRectangle(0, 0, Config::window.width, Config::window.height);

		ofSetColor(255);
		rockHandImg.draw(Config::window.width/2 - rockHandImg.getWidth()/2, 345, rockHandImg.getWidth(), rockHandImg.getHeight());

		ofSetColor(0, 159, 227);
		ofSetLineWidth(2.0f);
		ofDrawLine(625, 478, 1304, 478);

		bigTextFont.drawString("WOW! THAT'S",
			Config::window.width / 2 - bigTextFont.getStringBoundingBox("WOW! THAT'S", 0, 0).width / 2,
			540);

		bigTextFont.drawString("NIRVANA!",
			Config::window.width / 2 - bigTextFont.getStringBoundingBox("NIRVANA!", 0, 0).width / 2,
			590);

		if (currLang == "port") {
			ofSetColor(0, 159, 227);
			bigTextFont.drawString("VALEU!",
				Config::window.width / 2 - bigTextFont.getStringBoundingBox("VALEU!", 0, 0).width / 2,
				640);

			ofSetColor(255);
			bigTextFont.drawString("ASSISTA SUA PERFORMANCE NA TV AO LADO",
				Config::window.width / 2 - bigTextFont.getStringBoundingBox("ASSISTA SUA PERFORMANCE NA TV AO LADO", 0, 0).width / 2,
				900);
		}

		if (currLang == "eng") {
			ofSetColor(0, 159, 227);
			bigTextFont.drawString("THANK YOU!",
				Config::window.width / 2 - bigTextFont.getStringBoundingBox("THANK YOU!", 0, 0).width / 2,
				640);

			ofSetColor(255);
			bigTextFont.drawString("WATCH YOUR PERFORMANCE",
				Config::window.width / 2 - bigTextFont.getStringBoundingBox("WATCH YOUR PERFORMANCE", 0, 0).width / 2,
				900);

			bigTextFont.drawString("ON THE SCREEN IN THE NEXT ROOM",
				Config::window.width / 2 - bigTextFont.getStringBoundingBox("ON THE SCREEN IN THE NEXT ROOM", 0, 0).width / 2,
				950);
		}
	}

	if (cameraSetDeviceError) {
		ofSetColor(255, 255, 255);
		msgFont.drawString(camNotFoundMsg,
							Config::window.width/2 - msgFont.getStringBoundingBox(camNotFoundMsg, 0, 0).width/2,
							Config::window.height / 2 + 100);
	}

	if (connectionError) {
		ofSetColor(255, 255, 255);
		msgFont.drawString(connectionErrorMsg,
			Config::window.width / 2 - msgFont.getStringBoundingBox(connectionErrorMsg, 0, 0).width / 2,
			Config::window.height / 2 + 100);
	}

	if (Config::debugMode) {
		ofDrawBitmapStringHighlight("FPS: " + currentFPS , 70, 70);

		ofDrawBitmapStringHighlight("Camera devices found:",
							cameraInfoWindowXPos + 20,
							cameraInfoWindowYPos + 20);
		for (int i = 0; i < cameraDevices.size(); i++) {
			ofDrawBitmapStringHighlight(	"- " + cameraDevices.at(i),
								cameraInfoWindowXPos + 20,
								cameraInfoWindowYPos + 20 + (i + 1) * 20);
		}
	}
}

//--------------------------------------------------------------
void ofApp::audioIn(float *input, int bufferSize, int nChannels){
	if (isRecording) {
		vidRecorder.addAudioSamples(input, bufferSize, nChannels);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == OF_KEY_RIGHT) {
		appStateIndex++;
		if (appStateIndex >= 6) {
			appStateIndex = 6;
		}
		if (appStateIndex == 5) {
			initTime = ofGetElapsedTimeMillis();
		}
	}

	if (key == OF_KEY_LEFT) {
		appStateIndex--;
		if (appStateIndex <= 0) {
			appStateIndex = 0;
		}
	}

	if (key == OF_KEY_BACKSPACE) {
		if (vidRecorder.isRecording()) {
			isRecording = false;

			appStateIndex = 6;

			returnToInitScreenTimer.setup(10000, false);
			recordTimer.stopTimer();
		}
	}

	if (key == 'n' && appStateIndex != 2) {
		namesUpdated = false;
	}
}

//--------------------------------------------------------------
void ofApp::buttonClicked(int &buttonId) {
	switch (buttonId) {
		//portLangBtn
		case 1:
			currLang = "port";
			updateRegisteredVisitorList();
			appStateIndex = 1;
			portLangBtn->setEnabled(false);
			engLangBtn->setEnabled(false);
			simBtn->setEnabled(true);
			naoBtn->setEnabled(true);

			voltarBtn->setEnabled(true);

			if (Config::debugMode) {
				cout << "---> App state index is: " << ofToString(appStateIndex) << endl;
			}
			break;

		//engLangBtn
		case 2:
			currLang = "eng";
			updateRegisteredVisitorList();
			appStateIndex = 1;
			portLangBtn->setEnabled(false);
			engLangBtn->setEnabled(false);
			yesBtn->setEnabled(true);
			noBtn->setEnabled(true);

			returnBtn->setEnabled(true);

			if (Config::debugMode) {
				cout << "---> App state index is: " << ofToString(appStateIndex) << endl;
			}
			break;

		//simBtn
		case 3:
			appStateIndex = 3;
			namesUpdated = false;
			simBtn->setEnabled(false);
			naoBtn->setEnabled(false);
			lithiumBtn->setEnabled(true);
			smellsLikeBtn->setEnabled(true);
			loveBuzzBtn->setEnabled(true);
			comeAsBtn->setEnabled(true);

			if (peopleNamesBtns.size() > 0) {
				for (int i = 0; i < peopleNamesBtns.size(); i++) {
					peopleNamesBtns.at(i)->setEnabled(false);
				}
			}

			if (Config::debugMode) {
				cout << "---> App state index is: " << ofToString(appStateIndex) << endl;
			}
			break;

		//naoBtn
		case 4:
			appStateIndex = 2;
			if (Config::debugMode) {
				cout << "---> App state index is: " << ofToString(appStateIndex) << endl;
			}
			break;

		//yesBtn
		case 5:
			appStateIndex = 3;
			namesUpdated = false;
			yesBtn->setEnabled(false);
			noBtn->setEnabled(false);
			lithiumBtn->setEnabled(true);
			smellsLikeBtn->setEnabled(true);
			loveBuzzBtn->setEnabled(true);
			comeAsBtn->setEnabled(true);

			if (peopleNamesBtns.size() > 0) {
				for (int i = 0; i < peopleNamesBtns.size(); i++) {
					peopleNamesBtns.at(i)->setEnabled(false);
				}
			}

			if (Config::debugMode) {
				cout << "---> App state index is: " << ofToString(appStateIndex) << endl;
			}
			break;

		//noBtn
		case 6:
			appStateIndex = 2;
			if (Config::debugMode) {
				cout << "---> App state index is: " << ofToString(appStateIndex) << endl;
			}
			break;

		//lithiumBtn
		case 7:
			appStateIndex = 4;

			lithiumBtn->setEnabled(false);
			smellsLikeBtn->setEnabled(false);
			loveBuzzBtn->setEnabled(false);
			comeAsBtn->setEnabled(false);

			if (currLang == "port") {
				gravarBtn->setEnabled(true);
			}
			if (currLang == "eng") {
				recordBtn->setEnabled(true);
			}

      // Sound files are NOT provided in this repository due to licensing issues. Use your own files if you want to run this code, or comment out the following 3 lines below.
			soundPlayer.load(Config::audiosFolderPath + "lithium.wav");
			soundPlayer.setVolume(Config::initVolume);
			currAudioFileName = "lithium.wav";

			if (Config::debugMode) {
				cout << "---> App state index is: " << ofToString(appStateIndex) << endl;
			}
			break;

		//loveBuzzBtn
		case 8:
			appStateIndex = 4;

			lithiumBtn->setEnabled(false);
			smellsLikeBtn->setEnabled(false);
			loveBuzzBtn->setEnabled(false);
			comeAsBtn->setEnabled(false);

			if (currLang == "port") {
				gravarBtn->setEnabled(true);
			}
			if (currLang == "eng") {
				recordBtn->setEnabled(true);
			}

      // The sound files are NOT provided in this repository due to licensing issues. Use your own files if you want to run this code, or comment out the following 3 lines below.
			soundPlayer.load(Config::audiosFolderPath + "lovebuzz.wav");
			soundPlayer.setVolume(Config::initVolume);
			currAudioFileName = "lovebuzz.wav";

			if (Config::debugMode) {
				cout << "---> App state index is: " << ofToString(appStateIndex) << endl;
			}
			break;

		//smellsLikeBtn
		case 9:
			appStateIndex = 4;

			lithiumBtn->setEnabled(false);
			smellsLikeBtn->setEnabled(false);
			loveBuzzBtn->setEnabled(false);
			comeAsBtn->setEnabled(false);

			if (currLang == "port") {
				gravarBtn->setEnabled(true);
			}
			if (currLang == "eng") {
				recordBtn->setEnabled(true);
			}

      // Sound files are NOT provided in this repository due to licensing issues. Use your own files if you want to run this code, or comment out the following 3 lines below.
			soundPlayer.load(Config::audiosFolderPath + "smellslike.wav");
			soundPlayer.setVolume(Config::initVolume);
			currAudioFileName = "smellslike.wav";

			if (Config::debugMode) {
				cout << "---> App state index is: " << ofToString(appStateIndex) << endl;
			}
			break;

		//comeAsBtn
		case 10:
			appStateIndex = 4;

			lithiumBtn->setEnabled(false);
			smellsLikeBtn->setEnabled(false);
			loveBuzzBtn->setEnabled(false);
			comeAsBtn->setEnabled(false);

			if (currLang == "port") {
				gravarBtn->setEnabled(true);
			}
			if (currLang == "eng") {
				recordBtn->setEnabled(true);
			}

      // Sound files are NOT provided in this repository due to licensing issues. Use your own files if you want to run this code, or comment out the following 3 lines below.
			soundPlayer.load(Config::audiosFolderPath + "comeasyouare.wav");
			soundPlayer.setVolume(Config::initVolume);
			currAudioFileName = "comeasyouare.wav";

			if (Config::debugMode) {
				cout << "---> App state index is: " << ofToString(appStateIndex) << endl;
			}
			break;

		//gravarBtn
		case 11:
			appStateIndex = 5;
			gravarBtn->setEnabled(false);
			voltarBtn->setEnabled(false);
			if (Config::debugMode) {
				cout << "---> App state index is: " << ofToString(appStateIndex) << endl;
			}
			break;

		//recordBtn
		case 12:
			appStateIndex = 5;
			recordBtn->setEnabled(false);
			returnBtn->setEnabled(false);
			if (Config::debugMode) {
				cout << "---> App state index is: " << ofToString(appStateIndex) << endl;
			}
			break;

		//voltarBtn
		case 13:
			if (appStateIndex == 1 && currLang == "port") {
				portLangBtn->setEnabled(true);
				engLangBtn->setEnabled(true);
				simBtn->setEnabled(false);
				naoBtn->setEnabled(false);

				voltarBtn->setEnabled(false);

				appStateIndex = 0;
			}

			if (appStateIndex == 1 && currLang == "eng") {
				portLangBtn->setEnabled(true);
				engLangBtn->setEnabled(true);
				yesBtn->setEnabled(false);
				noBtn->setEnabled(false);

				returnBtn->setEnabled(false);

				appStateIndex = 0;
			}

			if (appStateIndex == 2 && currLang == "port") {
				if (peopleNamesBtns.size() > 0) {
					for (int i = 0; i < peopleNamesBtns.size(); i++) {
						peopleNamesBtns.at(i)->setEnabled(false);
					}
				}

				simBtn->setEnabled(true);
				naoBtn->setEnabled(true);

				appStateIndex = 1;
			}

			if (appStateIndex == 2 && currLang == "eng") {
				if (peopleNamesBtns.size() > 0) {
					for (int i = 0; i < peopleNamesBtns.size(); i++) {
						peopleNamesBtns.at(i)->setEnabled(false);
					}
				}

				yesBtn->setEnabled(true);
				noBtn->setEnabled(true);

				appStateIndex = 1;
			}

			if (appStateIndex == 3 && currLang == "port") {
				namesUpdated = false;
				simBtn->setEnabled(true);
				naoBtn->setEnabled(true);
				lithiumBtn->setEnabled(false);
				smellsLikeBtn->setEnabled(false);
				loveBuzzBtn->setEnabled(false);
				comeAsBtn->setEnabled(false);

				appStateIndex = 1;
			}

			if (appStateIndex == 3 && currLang == "eng") {
				namesUpdated = false;
				yesBtn->setEnabled(true);
				noBtn->setEnabled(true);
				lithiumBtn->setEnabled(false);
				smellsLikeBtn->setEnabled(false);
				loveBuzzBtn->setEnabled(false);
				comeAsBtn->setEnabled(false);

				appStateIndex = 1;
			}

			if (appStateIndex == 4 && currLang == "port") {
				lithiumBtn->setEnabled(true);
				smellsLikeBtn->setEnabled(true);
				loveBuzzBtn->setEnabled(true);
				comeAsBtn->setEnabled(true);

				gravarBtn->setEnabled(false);

				appStateIndex = 3;
			}

			if (appStateIndex == 4 && currLang == "eng") {
				lithiumBtn->setEnabled(true);
				smellsLikeBtn->setEnabled(true);
				loveBuzzBtn->setEnabled(true);
				comeAsBtn->setEnabled(true);

				recordBtn->setEnabled(false);

				appStateIndex = 3;
			}
			
			if (Config::debugMode) {
				cout << "---> App state index is: " << ofToString(appStateIndex) << endl;
			}
			break;

		//returnBtn
		case 14:
			if (appStateIndex == 1 && currLang == "port") {
				portLangBtn->setEnabled(true);
				engLangBtn->setEnabled(true);
				simBtn->setEnabled(false);
				naoBtn->setEnabled(false);

				voltarBtn->setEnabled(false);

				appStateIndex = 0;
			}

			if (appStateIndex == 1 && currLang == "eng") {
				portLangBtn->setEnabled(true);
				engLangBtn->setEnabled(true);
				yesBtn->setEnabled(false);
				noBtn->setEnabled(false);

				returnBtn->setEnabled(false);

				appStateIndex = 0;
			}

			if (appStateIndex == 2 && currLang == "port") {
				if (peopleNamesBtns.size() > 0) {
					for (int i = 0; i < peopleNamesBtns.size(); i++) {
						peopleNamesBtns.at(i)->setEnabled(false);
					}
				}

				simBtn->setEnabled(true);
				naoBtn->setEnabled(true);

				appStateIndex = 1;
			}

			if (appStateIndex == 2 && currLang == "eng") {
				if (peopleNamesBtns.size() > 0) {
					for (int i = 0; i < peopleNamesBtns.size(); i++) {
						peopleNamesBtns.at(i)->setEnabled(false);
					}
				}

				yesBtn->setEnabled(true);
				noBtn->setEnabled(true);

				appStateIndex = 1;
			}

			if (appStateIndex == 3 && currLang == "port") {
				namesUpdated = false;
				simBtn->setEnabled(true);
				naoBtn->setEnabled(true);
				lithiumBtn->setEnabled(false);
				smellsLikeBtn->setEnabled(false);
				loveBuzzBtn->setEnabled(false);
				comeAsBtn->setEnabled(false);

				appStateIndex = 1;
			}

			if (appStateIndex == 3 && currLang == "eng") {
				namesUpdated = false;
				yesBtn->setEnabled(true);
				noBtn->setEnabled(true);
				lithiumBtn->setEnabled(false);
				smellsLikeBtn->setEnabled(false);
				loveBuzzBtn->setEnabled(false);
				comeAsBtn->setEnabled(false);

				appStateIndex = 1;
			}

			if (appStateIndex == 4 && currLang == "port") {
				lithiumBtn->setEnabled(true);
				smellsLikeBtn->setEnabled(true);
				loveBuzzBtn->setEnabled(true);
				comeAsBtn->setEnabled(true);

				gravarBtn->setEnabled(false);

				appStateIndex = 3;
			}

			if (appStateIndex == 4 && currLang == "eng") {
				lithiumBtn->setEnabled(true);
				smellsLikeBtn->setEnabled(true);
				loveBuzzBtn->setEnabled(true);
				comeAsBtn->setEnabled(true);

				recordBtn->setEnabled(false);

				appStateIndex = 3;
			}

			if (Config::debugMode) {
				cout << "---> App state index is: " << ofToString(appStateIndex) << endl;
			}
			break;

		//people names buttons:
		default:
			currPersonName = lastRegisteredPeople.at(buttonId - 15)->getName();
			currVideoFileName = lastRegisteredPeople.at(buttonId - 15)->getVideoFileName();
			if (Config::debugMode) {
				cout << "Changed person name: " << currPersonName << endl;
				cout << "Changed video file: " << currVideoFileName << endl;
			}
			
			for (int i = 0; i < peopleNamesBtns.size(); i++) {
				peopleNamesBtns.at(i)->setEnabled(false);
			}

			lithiumBtn->setEnabled(true);
			smellsLikeBtn->setEnabled(true);
			loveBuzzBtn->setEnabled(true);
			comeAsBtn->setEnabled(true);

			appStateIndex = 3;

			break;
	}
}

//--------------------------------------------------------------
void ofApp::returnToInitialScreen(unsigned int &e) {
	if (Config::debugMode) {
		cout << "---> Closing the ffmpeg recorder..." << endl;
	}
	vidRecorder.close();

	appStateIndex = 0;
	portLangBtn->setEnabled(true);
	engLangBtn->setEnabled(true);

	soundPlayer.stop();
	soundPlayer.unloadSound();

	if (Config::debugMode) {
		cout << "---> Muxing recorded video with chosen audio..." << endl;
	}
	string cmd = "ffmpeg -i " + 
		Config::videosFolderPath + currVideoFileName + Config::videoFileExtension +
		" -i " + Config::audiosFolderPath + currAudioFileName +
		" -c:v copy -c:a aac " +
		Config::videosFolderPath + "output.mp4";
	system(cmd.c_str());

	if (Config::debugMode) {
		cout << "---> Deleting old video file..." << endl;
	}
	cmd = "del " + Config::videosFolderPath + currVideoFileName + Config::videoFileExtension;
	system(cmd.c_str());

	if (Config::debugMode) {
		cout << "---> Renaming new muxed file to " + currVideoFileName + Config::videoFileExtension << endl;
	}
	cmd = "rename " + Config::videosFolderPath + "output.mp4" + " " + currVideoFileName + Config::videoFileExtension;
	system(cmd.c_str());

	if (Config::debugMode) {
		cout << "---> Now copying file to " + Config::networkVideosFolderPath << endl;
	}
	
	std::ifstream  src(Config::videosFolderPath + currVideoFileName + Config::videoFileExtension, std::ios::binary);
	std::ofstream  dst(Config::networkVideosFolderPath + currVideoFileName + Config::videoFileExtension, std::ios::binary);
	dst << src.rdbuf();

	videoPlaylist.setTo("videos");
	videoPlaylist.addChild("video");
	videoPlaylist.setValue("video[" + ofToString(videoPlaylist.getNumChildren() - 1) + "]", currVideoFileName + Config::videoFileExtension);
	videoPlaylist.save(Config::videoPlaylistXmlPath);
}

//--------------------------------------------------------------
void ofApp::updateRegisteredVisitorList() {
	//Loading the xml with data from people who have registred:
	lastRegisteredPeople.clear();
	if (registeredPeopleXml.load(Config::registeredPeopleXmlPath)) {
		registeredPeopleXml.setTo("registros");
		if (registeredPeopleXml.getNumChildren() >= 10) {
			for (int i = registeredPeopleXml.getNumChildren() - 1; i >= registeredPeopleXml.getNumChildren() - 10; i--) {
				string name = registeredPeopleXml.getValue("pessoa[" + ofToString(i) + "]/nome");
				string videoFileName = registeredPeopleXml.getValue("pessoa[" + ofToString(i) + "]/videoFileName");
				Person * p = new Person(name, videoFileName);
				lastRegisteredPeople.push_back(p);
			}
		}
	
		if (registeredPeopleXml.getNumChildren() < 10 && registeredPeopleXml.getNumChildren() > 0) {
			for (int i = registeredPeopleXml.getNumChildren() - 1; i > -1; i--) {
				string name = registeredPeopleXml.getValue("pessoa[" + ofToString(i) + "]/nome");
				string videoFileName = registeredPeopleXml.getValue("pessoa[" + ofToString(i) + "]/videoFileName");
				Person * p = new Person(name, videoFileName);
				lastRegisteredPeople.push_back(p);
			}
		}

		currPersonName = lastRegisteredPeople.at(0)->getName();
		currVideoFileName = lastRegisteredPeople.at(0)->getVideoFileName();

		if (Config::debugMode && lastRegisteredPeople.size() > 0) {
			cout << "---> These are the last registered people's names saved in registros.xml:" << endl;
			for (int i = 0; i < lastRegisteredPeople.size(); i++) {
				cout << "---> " << lastRegisteredPeople.at(i)->getName() << endl;
				cout << "---> " << lastRegisteredPeople.at(i)->getVideoFileName() << endl;
			}
			cout << "---> Current person name is: " << currPersonName << endl;
			cout << "---> Current videoFileName is: " << currVideoFileName << endl;
		}
	}
	else {
		cout << "---> Erro ao carregar arquivo XML com registro de visitantes!" << endl;
	}
}

//--------------------------------------------------------------
void ofApp::stopRecording(unsigned int &e) {
	cout << "---> Stop recording in 10 seconds..." << endl;

	isRecording = false;

	appStateIndex = 6;

	returnToInitScreenTimer.setup(10000, false);
}