#include "Button.h"
#include "ofMain.h"

ofEvent<int> Button::mouseClickEvent = ofEvent<int>();

Button::Button(	string _buttonLabel, 
				string fontPath,
				int fontSize, 
				int _width, 
				int _height, 
				int _xPos, 
				int _yPos,
				int _id){

	buttonLabel = _buttonLabel;
	id = _id;
	buttonWidth = _width;
	buttonHeight = _height;
	buttonXPos = _xPos;
	buttonYPos = _yPos;

	buttonColor = ofColor(0, 159, 227);

	buttonLabelFont = new ofxTrueTypeFontUC();
	buttonLabelFont->loadFont(fontPath, fontSize, true, false, 0.0f);
	buttonLabelFont->setLetterSpacing(0.95);

	yOffset = 0;

	if (buttonLabel.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos) {
		yOffset = 3;
	}

	enabled = false;

	ofAddListener(delayTimer.TIMER_REACHED, this, &Button::notifyClick);

	buttonSound.load("sounds/button.wav");
	buttonSound.setVolume(Config::initVolume);
}

Button::~Button() {
	disableMouseEvents();
}

void Button::enableMouseEvents() {
	ofAddListener(ofEvents().mousePressed, this, &Button::isClicked);
}

void Button::disableMouseEvents() {
	ofRemoveListener(ofEvents().mousePressed, this, &Button::isClicked);
}

void Button::drawButton(){
	ofSetColor(buttonColor);
	ofFill();
	ofDrawRectangle(buttonXPos, buttonYPos, buttonWidth, buttonHeight);

	ofSetColor(255);
	buttonLabelFont->drawString( buttonLabel,
								int(buttonXPos + buttonWidth/2 - buttonLabelFont->stringWidth(buttonLabel)/2),
								int(buttonYPos + buttonHeight / 2 + buttonLabelFont->stringHeight(buttonLabel) / 2) - yOffset);
}

void Button::setButtonLabel(string _buttonLabel) {
	buttonLabel = _buttonLabel;
}

void Button::isClicked(ofMouseEventArgs &e) {
	int x = e.x;
	int y = e.y;

	if (hitTest(x, y)) {
		delayTimer.setup(1000, false);
		buttonColor = ofColor(0, 130, 180);
		buttonSound.play();
	}
}

bool Button::hitTest(int tx, int ty) {
	return ((tx > buttonXPos) && (tx < buttonXPos + buttonWidth) && (ty > buttonYPos) && (ty < buttonYPos + buttonHeight));
}

int Button::getID() {
	return id;
}

void Button::setEnabled(bool _enabled) {
	enabled = _enabled;

	if (enabled) {
		enableMouseEvents();
	}
	else {
		disableMouseEvents();
	}

}

void Button::notifyClick(unsigned int &e) {
	buttonColor = ofColor(0, 159, 227);
	buttonSound.stop();
	ofNotifyEvent(mouseClickEvent, id);
	if (Config::debugMode) {
		cout << "---> Button clicked - ID: " << ofToString(id) << endl;
	}
}