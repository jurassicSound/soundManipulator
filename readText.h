#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class readText
{
public:
	readText();
	~readText();

	vector<string> & readSoundPaths();

	private:
		vector<string> m_SoundPaths;
};

