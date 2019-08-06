#include "playVoice.h"


playVoice::playVoice()
{
}


playVoice::~playVoice()
{
}

HRESULT playVoice::playAudio(const TCHAR * strFileName, float volumeVariable, float pan, float pitchVariable)
{
		IXAudio2* pXAudio2;
		pXAudio2 = NULL;

		HRESULT hr;
		hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

		bool DidItFail = FAILED(hr);

		if (DidItFail) {
			return hr;
		}

		IXAudio2MasteringVoice* pMasterVoice;
		pMasterVoice = NULL;
		hr = pXAudio2->CreateMasteringVoice(&pMasterVoice);

		DidItFail = FAILED(hr);

		if (DidItFail)
			return hr;

		WAVEFORMATEXTENSIBLE wfx = { 0 };
		XAUDIO2_BUFFER buffer = { 0 };

#ifdef _XBOX	//THE COMMENTED OUT CODE IN THIS SECTION WERE SOME EARLIER TESTS OF THE PROGRAM, DON'T UNCOMMENT THEM!
	
		//char * strFileName = "C:\modules\Paul\sounds\daisy.wav";
#else
		//	const TCHAR * strFileName = _TEXT("C:\\modules\\Paul\\sounds\\daisy.wav");
		//	const TCHAR * strFileName = _TEXT("C:\\modules\\Paul\\sounds\\B17_Engine_Startup.wav");
#endif
		HANDLE hFile = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

		if (INVALID_HANDLE_VALUE == hFile) {
			cout << "\n Audio Playback: TERMINATED ";
			cout << "\n ERROR. Could not find file: " << strFileName << endl << endl;

			int i = 0;
			cin >> i;

			return HRESULT_FROM_WIN32(GetLastError());
		}

		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN)) {
			return HRESULT_FROM_WIN32(GetLastError());
		}

		DWORD dwChunkSize;
		DWORD dwChunkPosition;

		FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
		DWORD filetype;
		ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		if (filetype != fourccWAVE) {
			return S_FALSE;
		}

		FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

		FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);


		BYTE * pDataBuffer = new BYTE[dwChunkSize];
		ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

		buffer.AudioBytes = dwChunkSize;
		buffer.pAudioData = pDataBuffer;
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		
		IXAudio2SubmixVoice * pSFXSubmixVoice;
		pXAudio2->CreateSubmixVoice(&pSFXSubmixVoice, 1, 44100, 0, 0, 0, 0);

		XAUDIO2_SEND_DESCRIPTOR SFXSend = { 0, pSFXSubmixVoice };	

		XAUDIO2_VOICE_SENDS SFXSendList = { 1, &SFXSend };


		DWORD dwChannelMask;
		pMasterVoice->GetChannelMask(&dwChannelMask);

		float outputMatrix[8];
		for (int i = 0; i < 8; i++) 
		{
			outputMatrix[i] = 0;
		}

		// pan of -1.0 indicates all left speaker
		//1.0 is all right speaker, 0.0 is split between left and right

		float left = 0.5f - pan / 2;
		float right = 0.5f + pan / 2;

		switch (dwChannelMask)
		{
		case SPEAKER_MONO:
			outputMatrix[0] = 1.0;
			break;
		case SPEAKER_STEREO:
		case SPEAKER_2POINT1:
		case SPEAKER_SURROUND:
			outputMatrix[0] = left;
			outputMatrix[1] = right;
			break;
		case SPEAKER_QUAD:
			outputMatrix[0] = outputMatrix[2] = left;
			outputMatrix[1] = outputMatrix[3] = right;
			break;
		case SPEAKER_4POINT1:
			outputMatrix[0] = outputMatrix[3] = left;
			outputMatrix[1] = outputMatrix[4] = right;
			break;
		case SPEAKER_5POINT1:
		case SPEAKER_7POINT1:
		case SPEAKER_5POINT1_SURROUND:
			outputMatrix[0] = outputMatrix[4] = left;
			outputMatrix[1] = outputMatrix[5] = right;
			break;
		case SPEAKER_7POINT1_SURROUND:
			outputMatrix[0] = outputMatrix[4] = outputMatrix[6] = left;
			outputMatrix[1] = outputMatrix[5] = outputMatrix[7] = right;
			break;
		}

		XAUDIO2_VOICE_DETAILS VoiceDetails;
		pSFXSubmixVoice->GetVoiceDetails(&VoiceDetails);

		XAUDIO2_VOICE_DETAILS MasterVoiceDetails;
		pMasterVoice->GetVoiceDetails(&MasterVoiceDetails);

		pSFXSubmixVoice->SetOutputMatrix(NULL, VoiceDetails.InputChannels, MasterVoiceDetails.InputChannels, outputMatrix);

		// Create Source Voice 
		IXAudio2SourceVoice* pSFXSourceVoice;
		if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSFXSourceVoice, (WAVEFORMATEX*)&wfx,
		0, XAUDIO2_DEFAULT_FREQ_RATIO, (NULL), &SFXSendList, NULL))) {
			return hr;
		}

		pSFXSubmixVoice->SetVolume(volumeVariable);
		pSFXSourceVoice->SetFrequencyRatio(pitchVariable);		

		if (FAILED(hr = pSFXSourceVoice->SubmitSourceBuffer(&buffer))) {
			return hr;
		}

		if (FAILED(hr = pSFXSourceVoice->Start(0))) {
			return hr;
		}

		cout << "\n Audio Playback: SUCCESS " << endl;
}

