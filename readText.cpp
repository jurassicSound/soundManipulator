#include "readText.h"

readText::readText()
{
}


readText::~readText()
{
}

vector<string> & readText::readSoundPaths()
{
	string transferPath;

	ifstream reader("soundsArchive.txt");

	if (reader.is_open()) {

		while (getline(reader, transferPath)) {

			m_SoundPaths.push_back(transferPath);
		}
	}
	else {
		cout << "\n ERROR " << endl;
		return m_SoundPaths;
	}
	reader.close();

	return m_SoundPaths;
}
