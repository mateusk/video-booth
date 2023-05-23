#pragma once
#include "ofMain.h"

class Person{
public:
	Person(string _name, string _videoFileName);
	~Person();

	string getName();
	string getVideoFileName();

private:
	string name;
	string videoFileName;
};
