
#include <iostream>
#include <string>
#include <tchar.h>
#include <vector>

#include "readText.h"
#include "pickSound.h"
#include "playVoice.h"

using namespace std;

int main() {

	/*
	Sound program ToDo List
	- MAKE IT SO THAT YOU STOP A SPECIFIC SOUND ON COMMAND
	- MAKE IT SO THAT IT KEEPS A LIST OF WHICH SOUNDS WERE STARTED IN WHAT ORDER
	- MAKE IT SO THAT IT CAN OUTPUT THE SOUNDS THAT PLAYING IN ORDER
	- MAKE IT SO THAT YOU CAN GO BACK TO CHOOSING SOUNDS IF YOU TYPED IN THE WRONG SOUND
	- MAKE IT SO THAT THE PROGRAM ALLOWS YOU TO CHOOSE A SOUND THAT IS PLAYING, AND THEN IT PRINTS OUT THE
	SETTINGS OF THAT SOUND (VOLUME, PITCH, ECHO, etc.)
	- MAKE IT SO THAT YOU CAN ACTIVELY CHANGE A SETTING (i.e. VOLUME) WHILE A SOUND IS PLAYING
	- MAKE IT SO THAT YOU HAVE CATEGORIES OF SOUND (i.e. AMBIENCE, ANIMALS, MACHINES, etc.)
	*/

 	CoInitialize(nullptr);

	readText readPaths;
	pickSound selectVoice;
	playVoice transmitAudio;

	int choice = 0;
	int soundNumber = 0;
	vector<string> soundPaths = readPaths.readSoundPaths();
	float volumeVariable = 0.0;
	float pan = 0.0f;
	float pitchVariable = 0.0;

	while (true) {
		soundNumber = selectVoice.soundPicker(soundPaths);
		system("CLS");

		const TCHAR * strFileName = _TEXT(soundPaths[soundNumber].c_str());

		cout << "\n Type in the volume that would like this sound to be;" << endl;
		cout << " make sure the number is to the tenth's place: " << endl;
		cin >> volumeVariable;

		cout << "\n Type in a value within the values of -1.0 and 1.0. A number closer to -1.0" << endl;
		cout << " will make the sound more towards the left speaker; a number" << endl;
		cout << " closer to 1.0 will make the sound more towards the right speaker;" << endl;
		cout << " 0.0 will make the sound be 'split' between the left and right speakers: " << endl;
		cin >> pan;

		cout << "\n Type in the value for the change in pitch of the sound;" << endl;
		cout << " again, make sure the number is to the tenth's place: " << endl;
		cin >> pitchVariable;

		transmitAudio.playAudio(strFileName, volumeVariable, pan, pitchVariable);

		cout << "\n Would you like to play another sound; yes, 1, or no, 2: " << endl;
		cin >> choice;


		system("CLS");

		if (choice == 1) {}
		else if (choice == 2) {
			break;
		}
	}

	cout << "\nThank you for using this work-in-progress audio playing system.";
	//soundNumber = selectVoice.soundPicker(soundPaths);
	//
	//const TCHAR * strFileName = _TEXT(soundPaths[soundNumber].c_str());
	//
	//transmitAudio.playAudio(strFileName);
	
	cout << "\n-------------------------------\n";

	system("PAUSE");
	return 0;
}

/*IXAudio2MasteringVoice* pMasterVoice = NULL;
	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice))) {
		return hr;
	}

	cout << "\n Succeed!" << endl << endl;*/