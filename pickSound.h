#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

class pickSound
{
public:
	pickSound();
	~pickSound();

	int soundPicker(vector<string> soundPaths);

	private:
		string transferPath;
		int soundNumber = 0;
		int numberOfChars = 0;
};

