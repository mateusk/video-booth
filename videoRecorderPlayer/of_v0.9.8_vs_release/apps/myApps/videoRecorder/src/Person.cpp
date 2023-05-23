#include "Person.h"
#include "ofMain.h"

Person::Person(string _name, string _videoFileName){
	name = _name;
	videoFileName = _videoFileName;
}


Person::~Person(){
}

string Person::getName() {
	return name;
}

string Person::getVideoFileName() {
	return videoFileName;
}
