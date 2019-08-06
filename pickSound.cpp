#include "pickSound.h"



pickSound::pickSound()
{
}


pickSound::~pickSound()
{
}

int pickSound::soundPicker(vector<string> soundPaths)
{
	cout << "\n Here are " << soundPaths.size() << " sounds to choose from: " << endl;

	for (int s = 0; s < soundPaths.size(); s++) {
		transferPath = soundPaths[s];
		numberOfChars = transferPath.size();
		transferPath.replace(numberOfChars - 4, 4, "");

		transferPath.replace(0, 7, "");
		replace(transferPath.begin(), transferPath.end(), '_', ' ');

		cout << " - " << "(" << s << ") " << transferPath << endl;
	}

	cout << "\n Type in the number of the sound that you would like to hear: " << endl;
	cin >> soundNumber;

	return soundNumber;
}
