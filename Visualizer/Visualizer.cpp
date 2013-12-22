#include "Visualizer.h"


Visualizer::Visualizer() :
	mGraphicsManager(),
	mSoundManager(),
	mMusicPath(),
	mIsRunning(false)
{
	
}


Visualizer::~Visualizer()
{
}


// Visualizer initialization (to be called before run())
void Visualizer::initialize()
{
	mMusicPath = "Resource/music.mp3";
	mSoundManager.initialize(SPECTRUM_SIZE);
	mGraphicsManager.initialize(WINDOW_WIDTH, WINDOW_HEIGHT, SPECTRUM_SIZE);
	
	mIsRunning = true;
}


// Visualizer main loop
void Visualizer::run()
{
	mSoundManager.loadMusic(mMusicPath);
	mSoundManager.playMusic();

	while (mIsRunning)
	{
		handleEvents();
		mSoundManager.update();
		mGraphicsManager.update(mSoundManager.getSpectrum());
	}
}


// Handles input events
void Visualizer::handleEvents()
{
	SDL_Event event;

	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT)
		mIsRunning = false;
	else if (event.type == SDL_KEYDOWN)
		switch (event.key.keysym.sym)
	{
		case SDLK_ESCAPE:
			mIsRunning = false;
			break;
		default:
			break;
	}
}


// Handles refresh time
void Visualizer::handleTime()
{
	static int prevTime = 0;
	int currTime;

	currTime = SDL_GetTicks();
	if (currTime - prevTime < REFRESH_RATE)
		SDL_Delay(REFRESH_RATE - (currTime - prevTime));
	prevTime = SDL_GetTicks();
}
