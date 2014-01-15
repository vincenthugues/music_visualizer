#include "Visualizer.h"
#include "FrequencyBars.h"
#include "FrequencyBarsCentered.h"
#include "Spread.h"


Visualizer::Visualizer() :
	mGraphicsManager(),
	mSoundManager(),
	mVisualizations(),
	mMusicPath(),
	mIsRunning(false),
	mCurrentVisualization(-1)
{
}


Visualizer::~Visualizer()
{
	for (std::vector<Visualization *>::iterator it = mVisualizations.begin(); it != mVisualizations.end(); it++)
		delete(*it);
}


// Visualizer initialization (to be called before run())
void Visualizer::initialize()
{
	mMusicPath = "Resource/music.mp3";
	mSoundManager.initialize(SPECTRUM_SIZE);
	mGraphicsManager.initialize(WINDOW_WIDTH, WINDOW_HEIGHT, SPECTRUM_SIZE);
	
	// Initializes visualizations
	addVisualization(new FrequencyBars());
	addVisualization(new FrequencyBarsCentered());
	addVisualization(new Spread());

	if (mVisualizations.size() > 0)
		mCurrentVisualization = 0;

	mIsRunning = true;
}


// Visualizer main loop
void Visualizer::run()
{
	Uint32 startTime = SDL_GetTicks();
	Uint32 prevTime = startTime;
	Uint32 deltaTime;
	Uint32 delay = 25;
	int timer = delay;
	
	mSoundManager.loadMusic(mMusicPath);
	mSoundManager.playMusic();

	while (mIsRunning)
	{
		handleEvents();
		
		// Updates the timer and makes sure the frequencies and visualization only get updated every {delay} milliseconds.
		deltaTime = SDL_GetTicks() - prevTime;
		timer -= deltaTime;
		if (timer <= 0)
		{
			mSoundManager.update();
			mGraphicsManager.update(*mVisualizations[mCurrentVisualization], mSoundManager.getSpectrum());

			timer += delay;
		}
		
		mGraphicsManager.display(*mVisualizations[mCurrentVisualization]);

		prevTime += deltaTime;
	}
}


// Sets the current visualization to the next one
void Visualizer::cycleVisualizations()
{
	mCurrentVisualization = (mCurrentVisualization + 1) % mVisualizations.size();
}


// Adds a new visualization to the array
void Visualizer::addVisualization(Visualization* visualization)
{
	if (visualization != 0)
	{
		visualization->initialize(SPECTRUM_SIZE);
		mVisualizations.push_back(visualization);
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
		// If Space is pressed, switch to the next visualization
		case SDLK_SPACE:
			cycleVisualizations();
			break;
		// If Escape or "q" is pressed, end the program's main loop
		case SDLK_ESCAPE:
		case SDLK_q:
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
