#include <iostream>

#include "SoundManager.h"


SoundManager::SoundManager() :
	mSystem(0),
	mMusic(0),
	mChannel(0),
	mResult(),
	mMusicPath(std::string()),
	mSpectrum(0)
{
}


SoundManager::~SoundManager()
{
	if (mSpectrum != 0)
		delete mSpectrum;

	FMOD_Sound_Release(mMusic);
	FMOD_System_Close(mSystem);
	FMOD_System_Release(mSystem);
}


const float* SoundManager::getSpectrum()
{
	return mSpectrum;
}


bool SoundManager::initialize(int spectrumSize)
{
	mSpectrumSize = spectrumSize;
	if (mSpectrum != 0)
		delete mSpectrum;
	mSpectrum = new float[spectrumSize];
	
	FMOD_System_Create(&mSystem);
	FMOD_System_Init(mSystem, 1, FMOD_INIT_NORMAL, 0);

	return true;
}


bool SoundManager::loadMusic(std::string& musicPath)
{
	mMusicPath = musicPath;

	mResult = FMOD_System_CreateSound(mSystem, mMusicPath.c_str(), FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &mMusic);
	if (mResult != FMOD_OK)
	{
		std::cerr << "Unable to load " << mMusicPath.c_str() << std::endl;
		return false;
	}

	return true;
}


void SoundManager::playMusic()
{
	FMOD_System_PlaySound(mSystem, FMOD_CHANNEL_FREE, mMusic, 0, NULL);
	FMOD_System_GetChannel(mSystem, 0, &mChannel);
}


void SoundManager::update()
{
	FMOD_Channel_GetSpectrum(mChannel, mSpectrum, mSpectrumSize, 0, FMOD_DSP_FFT_WINDOW_RECT);
}
