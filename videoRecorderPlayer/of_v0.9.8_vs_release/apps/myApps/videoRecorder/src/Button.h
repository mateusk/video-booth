#ifndef _BUTTON
#define _BUTTON
#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxTrueTypeFontUC.h"
#include "ofxTimer\src\ofxTimer.h"
#include "Config.h"

class Button {
public:
	Button( string _buttonLabel, 
			string fontPath,
			int fontSize, 
			int _width, 
			int _height, 
			int _xPos, 
			int _yPos,
			int _id);

	~Button();

	int getID();
	void drawButton();
	void setButtonLabel(string _buttonLabel);
	void setEnabled(bool _enabled);
	static ofEvent<int> mouseClickEvent;

private:
	string buttonLabel;
	
	int id;
	int buttonWidth;
	int buttonHeight;
	int buttonXPos;
	int buttonYPos;
	int yOffset;

	bool enabled;

	ofColor buttonColor;

	ofxTrueTypeFontUC * buttonLabelFont;

	ofxTimer delayTimer;

	ofSoundPlayer buttonSound;

	void enableMouseEvents();
	void disableMouseEvents();
	void isClicked(ofMouseEventArgs &e);
	void notifyClick(unsigned int &e);

	bool hitTest(int tx, int ty);
};

#endif
