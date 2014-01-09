#include "Visualizer.h"
#include "FreqBarsVisualization.h"


Visualizer::Visualizer() :
	mGraphicsManager(),
	mSoundManager(),
	mVisualizations(),
	mMusicPath(),
	mIsRunning(false),
	mCurrentVisualizationIdx(-1)
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

	// Initialize visualizations
	Visualization* visualization = new FreqBarsVisualization();
	if (visualization != 0)
	{
		visualization->initialize(SPECTRUM_SIZE);
		mVisualizations.push_back(*visualization);
	}

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
		mGraphicsManager.update(mVisualizations[mCurrentVisualizationIdx], mSoundManager.getSpectrum());
	}
}


// Set the current visualization to the next one
void Visualizer::cycleVisualizations()
{
	++mCurrentVisualizationIdx;
	
	if (mCurrentVisualizationIdx == mVisualizations.size())
		mCurrentVisualizationIdx = 0;
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
		case SDLK_SPACE:
			mGraphicsManager.rotateVisualizations();
			break;
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
