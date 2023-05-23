#pragma once
#include "ofMain.h"

class ofxTimer {

private:


	// timer
	bool		bLoop;
	bool		bPauseTimer;
	bool		bStartTimer;
	bool		bTimerFinished;
	float		delay;
	float		timer;
	float		timerStart;
	// Modifs
	float       pauseStartTime;
	float       pauseTime;
	bool        paused;
	bool        resumed;

	void update(ofEventArgs &e);

public:

	unsigned int	count;
	ofEvent <unsigned int> TIMER_REACHED;

	ofxTimer();
	~ofxTimer();

	// ---------------------------------------
	void reset();
	void setup(float millSeconds, bool loopTimer);
	//void draw();

	// ---------------------------------------
	void setTimer(float millSeconds);
	float getTimeLeftInSeconds();
	float getTimeLeftInMillis();
	void startTimer();
	void stopTimer();
	void pauseTimer();
	void resumeTimer();
	void loop(bool b);
	bool isTimerFinished();
};