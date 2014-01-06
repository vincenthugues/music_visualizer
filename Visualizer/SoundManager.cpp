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


// Returns the frequency spectrum (can be equal to 0 if SoundManager::initialize() has not been called yet)
const float* SoundManager::getSpectrum()
{
	return mSpectrum;
}


// SoundManager initialization (to be called before loadMusic() and playMusic())
void SoundManager::initialize(int spectrumSize)
{
	mSpectrumSize = spectrumSize;
	if (mSpectrum != 0)
		delete mSpectrum;
	mSpectrum = new float[spectrumSize];
	
	FMOD_System_Create(&mSystem);
	FMOD_System_Init(mSystem, 1, FMOD_INIT_NORMAL, 0);
}


// Sound file loading (to be called before playMusic())
void SoundManager::loadMusic(std::string& musicPath)
{
	mMusicPath = musicPath;

	mResult = FMOD_System_CreateSound(mSystem, mMusicPath.c_str(), FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &mMusic);
	if (mResult != FMOD_OK)
	{
		std::cerr << "Unable to load " << mMusicPath.c_str() << std::endl;
		throw new std::runtime_error(std::string("SoundManager::loadMusic() failed to load " + mMusicPath));
	}
}


// Plays the previously loaded sound file
void SoundManager::playMusic()
{
	FMOD_System_PlaySound(mSystem, FMOD_CHANNEL_FREE, mMusic, 0, NULL);
	FMOD_System_GetChannel(mSystem, 0, &mChannel);
}


// Updates the frequency spectrum to the current audio frequencies
void SoundManager::update()
{
	FMOD_Channel_GetSpectrum(mChannel, mSpectrum, mSpectrumSize, 0, FMOD_DSP_FFT_WINDOW_RECT);
}
