#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "xaudio2.h"

#include <x3daudio.h>

#pragma comment(lib, "Xaudio2.lib")

using namespace std;

#ifdef _XBOX
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

#define ISFP INVALID_SET_FILE_POINTER
#define HFW HRESULT_FROM_WIN32(GetLastError());

class playVoice
{
public:
	playVoice();
	~playVoice();
	
	HRESULT playAudio(const TCHAR * strFileName, float volumeVariable, float pan, float pitchVariable);

private:
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
	{
		HRESULT hr;
		hr = S_OK;

		int IsSetFilePointerStatus;
		IsSetFilePointerStatus = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
		if (ISFP == IsSetFilePointerStatus) {
			return HFW;
		}

		DWORD dwChunkType;
		DWORD dwChunkDataSize;

		DWORD dwRIFFDataSize;
		dwRIFFDataSize = 0;

		DWORD dwFileType;

		DWORD bytesRead;
		bytesRead = 0;

		DWORD dwOffset;
		dwOffset = 0;

		while (hr == S_OK)
		{
			DWORD dwRead;

			int chunkType;
			chunkType = ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL);
			if (0 == chunkType) {
				hr = HFW;
			}

			int chunkDataSize;
			chunkDataSize = ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL);
			if (0 == chunkDataSize) {
				hr = HFW;
			}

			switch (dwChunkType)
			{
			case fourccRIFF:
				dwRIFFDataSize = dwChunkDataSize;
				dwChunkDataSize = 4;

				int fileType;
				fileType = ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL);
				if (0 == fileType) {
					hr = HFW;
				}

				break;

			default:
				if (ISFP == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT)) {
					return HFW;
				}
			}

			dwOffset = dwOffset + sizeof(DWORD) * 2;

			if (dwChunkType == fourcc)
			{
				dwChunkSize = dwChunkDataSize;
				dwChunkDataPosition = dwOffset;
				return S_OK;
			}

			dwOffset = dwOffset + dwChunkDataSize;

			if (bytesRead >= dwRIFFDataSize) {
				return S_FALSE;
			}

		}

		return S_OK;
	}

	HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
	{
		HRESULT hr;
		hr = S_OK;

		if (ISFP == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN)) {
			return HFW;
		}

		DWORD dwRead;

		int funcReadFile;
		funcReadFile = ReadFile(hFile, buffer, buffersize, &dwRead, NULL);

		if (0 == funcReadFile) {
			hr = HFW;
		}

		return hr;
	}
};

