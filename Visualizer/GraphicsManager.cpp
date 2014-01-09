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


// GraphicsManager initialization (to be called before any other function)
void GraphicsManager::initialize(int windowWidth, int windowHeight, int spectrumSize)
{
	mWindowWidth = windowWidth;
	mWindowHeight = windowHeight;
	mSpectrumSize = spectrumSize;
	mGraphStep = static_cast<float>(mWindowWidth) / static_cast<float>(mSpectrumSize);

	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		mScreen = SDL_SetVideoMode(mWindowWidth, mWindowHeight, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
		SDL_WM_SetCaption("Visualizer", NULL);
	}
	else
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
}


// Set the pixel at (x, y) to the given value
// NOTE: The surface must be locked before calling this
void GraphicsManager::setPixel(SDL_Surface *surface, const int x, const int y, const Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	if (x < 0 || y < 0
		|| x >= surface->w || y >= surface->h)
		return;

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


// Displays the first type of visualization: vertical bars whose heights are proportional to the corresponding frequency
void GraphicsManager::displayBars(const float spectrum[])
{
	int value = 0, x = 0;
	int y, i;
	float ratio = mWindowHeight / 255.0f;

	SDL_LockSurface(mScreen);
	
	for (i = 0; x < mWindowWidth && i < mSpectrumSize; ++i)
	{
		value = static_cast<int>(spectrum[i] * 20.0f * mWindowHeight);
		if (value >= mWindowHeight)
			value = mWindowHeight - 1;
		if (value < 0)
			value = 0;

		for (y = mWindowHeight - value; y < mWindowHeight; y++)
			setPixel(mScreen, x, y, SDL_MapRGB(mScreen->format, static_cast<Uint8>(255 - (y / ratio)), static_cast<Uint8>(y / ratio), 0));

		x += static_cast<int>(mGraphStep);
	}

	SDL_UnlockSurface(mScreen);
}


// Displays the first type of visualization: vertical bars whose heights are proportional to the corresponding frequency
void GraphicsManager::displayCenteredBars(const float spectrum[])
{
	int value = 0, x = 0;
	int y, i;
	float ratio = 0;
	
	SDL_LockSurface(mScreen);

	for (i = 0; x < mWindowWidth && i < mSpectrumSize; ++i)
	{
		value = static_cast<int>(spectrum[i] * 10 * mWindowHeight);
		if (value >= mWindowHeight)
			value = mWindowHeight - 1;
		if (value < 0)
			value = 0;

		ratio = (float)value / (float)mWindowHeight;
		for (y = - value/2; y < value / 2; y++)
			setPixel(mScreen, x, y + mWindowHeight / 2, SDL_MapRGB(mScreen->format,
																   static_cast<Uint8>(ratio * 255.0f),
																   static_cast<Uint8>((1 - ratio) * 128.0f),
																   static_cast<Uint8>((1 - ratio) * 255.0)));
		
		x += static_cast<int>(mGraphStep);
	}

	SDL_UnlockSurface(mScreen);
}


// Sets the pixels' values for the second type of visualization (see displaySpread())
void GraphicsManager::displayPointSpread(int x, int y, Uint32 color)
{
	setPixel(mScreen, x, y, color);
	setPixel(mScreen, x, mWindowHeight - y, color);
	setPixel(mScreen, mWindowWidth - x - 1, y, color);
	setPixel(mScreen, mWindowWidth - x - 1, mWindowHeight - y, color);
}


// Displays the first type of visualization: pixels that seem to be "leaking" from fixed sources, spreading in a cone-shaped area
void GraphicsManager::displaySpread(const float spectrum[])
{
	int value = 0, x = 0;
	int y, tmp, i;
	Uint32 color;
	float ratio = mWindowHeight / 255.0f;

	SDL_LockSurface(mScreen);

	for (i = 0; x < mWindowWidth && i < mSpectrumSize; ++i)
	{
		value = static_cast<int>(spectrum[i] * 20 * mWindowHeight);
		if (value >= mWindowHeight)
			value = mWindowHeight - 1;
		if (value < 0)
			value = 0;

		y = mWindowHeight - value;
		if (y >= 0 && y < mWindowHeight)
		{
			color = SDL_MapRGB(mScreen->format, static_cast<Uint8>(255 - (1.5 * y / ratio)), static_cast<Uint8>(y / ratio), 0);
			displayPointSpread(x, y, color);
			tmp = x;
			x = y * mWindowHeight / mWindowWidth;
			y = tmp * mWindowWidth / mWindowHeight;
			displayPointSpread(y, x, color);
		}

		x += static_cast<int>(mGraphStep);
	}
	
	SDL_UnlockSurface(mScreen);
}


// Updates the display by clearing the screen and displaying the current visualization
void GraphicsManager::update(Visualization& visualization, const float spectrum[])
{
	SDL_LockSurface(mScreen);
	clearScreen();


	if (mVisualization == 0)
		displayBars(spectrum);
	else if (mVisualization == 1)
		displayCenteredBars(spectrum);
	else if (mVisualization == 2)
		displaySpread(spectrum);

	SDL_UnlockSurface(mScreen);
	SDL_Flip(mScreen);
}


// Clears the screen with the default color
void GraphicsManager::clearScreen()
{
	SDL_FillRect(mScreen, NULL, SDL_MapRGB(mScreen->format, 0, 0, 0));
}
