#include <iostream>
#include "Visualizer.h"


void Visualizer::handleEvents()
{
	SDL_Event event;

	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT)
		mData.running = false;
	else if (event.type == SDL_KEYDOWN)
		switch (event.key.keysym.sym)
	{
		case SDLK_ESCAPE:
			mData.running = false;
			break;
		default:
			break;
	}
}


void Visualizer::handleTime()
{
	static int prevTime = 0;
	int currTime;

	currTime = SDL_GetTicks();
	if (currTime - prevTime < REFRESH_RATE)
		SDL_Delay(REFRESH_RATE - (currTime - prevTime));
	prevTime = SDL_GetTicks();
}


bool Visualizer::fmodInit()
{
	FMOD_System_Create(&mData.system);
	FMOD_System_Init(mData.system, 1, FMOD_INIT_NORMAL, NULL);
	mData.result = FMOD_System_CreateSound(mData.system, mData.soundPath,
		FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &mData.music);
	if (mData.result != FMOD_OK)
	{
		std::cerr << "Unable to load " << mData.soundPath << std::endl;
		return false;
	}
	FMOD_System_PlaySound(mData.system, FMOD_CHANNEL_FREE, mData.music, 0, NULL);
	FMOD_System_GetChannel(mData.system, 0, &mData.channel);
	return true;
}


Visualizer::Visualizer() :
	mData(),
	mGraphicsManager()
{
	
}


Visualizer::~Visualizer()
{
}


bool Visualizer::initialize()
{
	mData.soundPath = "Resource/music.mp3";
	
	if (fmodInit() == true)
		mGraphicsManager.initialize(WINDOW_WIDTH, WINDOW_HEIGHT, SPECTRUM_SIZE);
	else
	{
		std::cerr << "Failed to initialize fmod." << std::endl;
		return false;
	}
	
	mData.running = true;
	return true;
}


void Visualizer::run()
{
	while (mData.running)
	{
		handleEvents();
		FMOD_Channel_GetSpectrum(mData.channel, mData.spectrum, SPECTRUM_SIZE, 0, FMOD_DSP_FFT_WINDOW_RECT);
		mGraphicsManager.update(mData.spectrum);
	}

	FMOD_Sound_Release(mData.music);
	FMOD_System_Close(mData.system);
	FMOD_System_Release(mData.system);
}
