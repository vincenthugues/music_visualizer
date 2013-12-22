#include <iostream>

#include "GraphicsManager.h"


GraphicsManager::GraphicsManager() :
	mScreen(0)
{
}


GraphicsManager::~GraphicsManager()
{
	SDL_Quit();
}


void GraphicsManager::initialize(int windowWidth, int windowHeight, int spectrumSize)
{
	mWindowWidth = windowWidth;
	mWindowHeight = windowHeight;
	mSpectrumSize = spectrumSize;
	mGraphStep = mWindowWidth / mSpectrumSize;

	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		mScreen = SDL_SetVideoMode(mWindowWidth, mWindowHeight, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
		SDL_WM_SetCaption("Visualizer", NULL);
	}
	else
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
}


// Set the pixel at (x, y) to the given value
// NOTE: The surface must be locked before calling this!
void GraphicsManager::setPixel(SDL_Surface *surface, const int x, const int y, const Uint32 pixel)
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


void GraphicsManager::displayPointBars(int x, int y, float ratio)
{
	setPixel(mScreen, x, y, SDL_MapRGB(mScreen->format, 0, 255, 0));
	setPixel(mScreen, x + 1, y, SDL_MapRGB(mScreen->format, 0, 255, 0));
	setPixel(mScreen, x, y, SDL_MapRGB(mScreen->format, 255 - (y / ratio), y / ratio, 0));
	setPixel(mScreen, x + 1, y, SDL_MapRGB(mScreen->format, 255 - (y / mWindowHeight), y / mWindowHeight, 0));
}


void GraphicsManager::displayPointSpread(int x, int y, Uint32 color)
{
	setPixel(mScreen, x, y, color);
	setPixel(mScreen, x, mWindowHeight - y, color);
	setPixel(mScreen, mWindowWidth - x - 1, y, color);
	setPixel(mScreen, mWindowWidth - x - 1, mWindowHeight - y, color);
}


void GraphicsManager::displayBars(float spectrum[])
{
	int value = 0, x = 0;
	int y, i;
	float ratio = mWindowHeight / 255.0;

	for (i = 0; x < mWindowWidth && i < mSpectrumSize; ++i)
	{
		value = spectrum[i] * 20 * mWindowHeight;
		if (value >= mWindowHeight)
			value = mWindowHeight - 1;
		if (value < 0)
			value = 0;

		SDL_LockSurface(mScreen);
		for (y = mWindowHeight - value; y < mWindowHeight; y++)
			displayPointBars(x, y, ratio);
		SDL_UnlockSurface(mScreen);

		x += mGraphStep;
	}
}


void GraphicsManager::displaySpread(float spectrum[])
{
	int value = 0, x = 0;
	int y, tmp, i;
	Uint32 color;
	float ratio = mWindowHeight / 255.0;

	for (i = 0; x < mWindowWidth && i < mSpectrumSize; ++i)
	{
		value = spectrum[i] * 20 * mWindowHeight;
		if (value >= mWindowHeight)
			value = mWindowHeight - 1;
		if (value < 0)
			value = 0;

		y = mWindowHeight - value;
		if (y >= 0 && y < mWindowHeight)
		{
			color = SDL_MapRGB(mScreen->format, 255 - (1.5 * y / ratio), y / ratio, 0);
			SDL_LockSurface(mScreen);
			displayPointSpread(x, y, color);
			SDL_UnlockSurface(mScreen);
			tmp = x;
			x = y * mWindowHeight / mWindowWidth;
			y = tmp * mWindowWidth / mWindowHeight;
			SDL_LockSurface(mScreen);
			displayPointSpread(y, x, color);
			SDL_UnlockSurface(mScreen);
		}

		x += mGraphStep;
	}
}


void GraphicsManager::update(float spectrum[])
{
	SDL_LockSurface(mScreen);
	clearScreen();

	displayBars(spectrum);
	//displaySpread(spectrum);

	SDL_UnlockSurface(mScreen);
	SDL_Flip(mScreen);
}


void GraphicsManager::clearScreen()
{
	SDL_FillRect(mScreen, NULL, SDL_MapRGB(mScreen->format, 0, 0, 0));
}


