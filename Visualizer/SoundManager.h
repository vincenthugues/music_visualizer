#pragma once

#include <string>
#include <fmod.h>


class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	const float* getSpectrum();

	void initialize(int spectrumSize);
	void loadMusic(std::string & musicPath);
	void playMusic();
	void update();

private:
	FMOD_SYSTEM* mSystem;
	FMOD_SOUND* mMusic;
	FMOD_CHANNEL* mChannel;
	FMOD_RESULT mResult;
	std::string mMusicPath;
	float* mSpectrum;
	int mSpectrumSize;
};
