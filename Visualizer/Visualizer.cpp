#include <iostream>
#include "Visualizer.h"


// Set the pixel at (x, y) to the given value
// NOTE: The surface must be locked before calling this!
void setPixel(SDL_Surface *surface, const int x, const int y, const Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp)
	{
	case 1:
		*p = pixel;
		break;
	case 2:
		*(Uint16 *)p = pixel;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else
		{
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;
	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}


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


void Visualizer::displayPointBars(int x, int y)
{
	setPixel(mData.screen, x, y, SDL_MapRGB(mData.screen->format, 0, 255, 0));
	setPixel(mData.screen, x + 1, y, SDL_MapRGB(mData.screen->format, 0, 255, 0));
	setPixel(mData.screen, x, y, SDL_MapRGB(mData.screen->format, 255 - (y / RATIO), y / RATIO, 0));
	setPixel(mData.screen, x + 1, y, SDL_MapRGB(mData.screen->format, 255 - (y / WINDOW_HEIGHT), y / WINDOW_HEIGHT, 0));
}


void Visualizer::displayPointSpread(int x, int y, Uint32 color)
{
	setPixel(mData.screen, x, y, color);
	setPixel(mData.screen, x, WINDOW_HEIGHT - y, color);
	setPixel(mData.screen, WINDOW_WIDTH - x - 1, y, color);
	setPixel(mData.screen, WINDOW_WIDTH - x - 1, WINDOW_HEIGHT - y, color);
}


void Visualizer::displayBars()
{
	int value = 0, x = 0;
	int y, i;

	for (i = 0; i < WINDOW_WIDTH && i < SPECTRUM_SIZE; ++i)
	{
		value = mData.spectrum[i] * 20 * WINDOW_HEIGHT;
		if (value >= WINDOW_HEIGHT)
			value = WINDOW_HEIGHT - 1;
		if (value <= 0)
			value = 1;

		SDL_LockSurface(mData.screen);
		for (y = WINDOW_HEIGHT - value; y < WINDOW_HEIGHT; y++)
			displayPointBars(x, y);
		SDL_UnlockSurface(mData.screen);

		x += GRAPH_STEP;
	}
}


void Visualizer::displaySpread()
{
	int value = 0, x = 0;
	int y, tmp, i;
	Uint32 color;

	for (i = 0; i < WINDOW_WIDTH && i < SPECTRUM_SIZE; ++i)
	{
		value = mData.spectrum[i] * 20 * WINDOW_HEIGHT;
		if (value >= WINDOW_HEIGHT)
			value = WINDOW_HEIGHT - 1;
		if (value <= 0)
			value = 1;

		y = WINDOW_HEIGHT - value;
		if (y > 1 && y < WINDOW_HEIGHT)
		{
			color = SDL_MapRGB(mData.screen->format, 255 - (1.5 * y / RATIO), y / RATIO, 0);
			SDL_LockSurface(mData.screen);
			displayPointSpread(x, y, color);
			SDL_UnlockSurface(mData.screen);
			tmp = x;
			x = y * WINDOW_HEIGHT / WINDOW_WIDTH;
			y = tmp * WINDOW_WIDTH / WINDOW_HEIGHT;
			SDL_LockSurface(mData.screen);
			displayPointSpread(y, x, color);
			SDL_UnlockSurface(mData.screen);
		}

		x += GRAPH_STEP;
	}
}


void Visualizer::displayGraph()
{
	//displayBars();
	displaySpread();
}


void Visualizer::clearScreen()
{
	SDL_FillRect(mData.screen, NULL, SDL_MapRGB(mData.screen->format, 0, 0, 0));
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


void Visualizer::processMusic()
{
	while (mData.running)
	{
		handleEvents();
		clearScreen();
		FMOD_Channel_GetSpectrum(mData.channel, mData.spectrum, SPECTRUM_SIZE, 0, FMOD_DSP_FFT_WINDOW_RECT);
		SDL_LockSurface(mData.screen);
		displayGraph();
		SDL_UnlockSurface(mData.screen);
		SDL_Flip(mData.screen);
	}
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


bool Visualizer::SDLInit()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}
	
	mData.screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Visualizer", NULL);
	return true;
}


void Visualizer::dataInit()
{
	mData.soundPath = "Resource/music.mp3";
	mData.screen = NULL;
}


Visualizer::Visualizer()
{
}


Visualizer::~Visualizer()
{
}


bool Visualizer::initialize()
{
	dataInit();
	if (fmodInit() == true)
	{
		if (SDLInit() == true)
			mData.running = true;
		else
		{
			std::cerr << "Failed to initialize SDL." << std::endl;
			return false;
		}
	}
	else
	{
		std::cerr << "Failed to initialize fmod." << std::endl;
		return false;
	}
	
	return true;
}


void Visualizer::run()
{
	processMusic();
	FMOD_Sound_Release(mData.music);
	FMOD_System_Close(mData.system);
	FMOD_System_Release(mData.system);
	SDL_Quit();
}
