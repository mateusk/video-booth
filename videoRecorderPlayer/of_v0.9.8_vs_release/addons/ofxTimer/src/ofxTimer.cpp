#include "ofxTimer.h"

ofxTimer::ofxTimer() {
}

ofxTimer::~ofxTimer() {
	ofRemoveListener(ofEvents().update, this, &ofxTimer::update);
	ofLogVerbose("Timer Destroyed\n");
}

// ---------------------------------------

void ofxTimer::setup(float millSeconds, bool loopTimer) {

	count = 0;
	bLoop = loopTimer;
	bPauseTimer = false;

	//timer
	bStartTimer = true;
	bTimerFinished = false;
	delay = millSeconds;	// mill seconds
	timer = 0;
	timerStart = 0;

	paused = false;
	resumed = false;

	//events
	ofAddListener(ofEvents().update, this, &ofxTimer::update);

}

// ---------------------------------------
void ofxTimer::pauseTimer() {
	paused = true;
	pauseStartTime = ofGetElapsedTimef();
}

// ---------------------------------------
void ofxTimer::resumeTimer() {
	resumed = true;
	paused = false;
	pauseTime = ofGetElapsedTimef() - pauseStartTime;
}

// ---------------------------------------
void ofxTimer::reset() {
	count = 0;
	timer = 0;
	timerStart = 0;
	bStartTimer = true;
	bTimerFinished = false;
	resumed = false;
	paused = false;
}

// ---------------------------------------
void ofxTimer::loop(bool b) {
	bLoop = b;
}

// ---------------------------------------
void ofxTimer::update(ofEventArgs &e) {

	if (paused)
		return;

	if (!bPauseTimer) {
		if (bStartTimer) {
			bStartTimer = false;
			timerStart = ofGetElapsedTimef();
		}

		float time = ofGetElapsedTimef() - timerStart;

		if (resumed) {
			time -= pauseTime;
		}

		time *= 1000.0;
		if (time >= delay) {
			count++;
			if (!bLoop) {
				bPauseTimer = true;
				bTimerFinished = true;	//TODO noch kein unterschied zu bPaused;
			}
			paused = false;
			resumed = false;

			bStartTimer = true;
			ofNotifyEvent(TIMER_REACHED, count, this);

			ofLogNotice() << "TIMER_REACHED";
		}
	}
}

float ofxTimer::getTimeLeftInSeconds() {

	if (bTimerFinished)
		return 0;

	if (bStartTimer) {
		return delay / 1000.f;
	}

	float time = ofGetElapsedTimef() - timerStart;

	if (resumed) {
		time = (ofGetElapsedTimef() - timerStart - pauseTime);
	}

	if (paused) {
		time = (ofGetElapsedTimef() - timerStart - ofGetElapsedTimef()
			+ pauseStartTime);
	}

	return (delay / 1000.0) - time;
}

float ofxTimer::getTimeLeftInMillis() {

	if (bTimerFinished)
		return 0;

	if (bStartTimer) {
		return delay;
	}

	float time = ofGetElapsedTimef() - timerStart;

	if (resumed) {
		time = (ofGetElapsedTimef() - timerStart - pauseTime);
	}

	if (paused) {
		time = (ofGetElapsedTimef() - timerStart - ofGetElapsedTimef()
			+ pauseStartTime);
	}

	return delay - (time * 1000.0);
}

// ---------------------------------------
void ofxTimer::setTimer(float millSeconds) {
	delay = millSeconds;
}

void ofxTimer::startTimer() {
	bPauseTimer = false;
}

void ofxTimer::stopTimer() {
	bPauseTimer = true;
}

bool ofxTimer::isTimerFinished() {
	return bTimerFinished;
}