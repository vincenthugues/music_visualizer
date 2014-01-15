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


// Updates the visualization's data
void GraphicsManager::update(Visualization& visualization, const float spectrum[])
{
	visualization.update(spectrum, mWindowWidth, mWindowHeight, mGraphStep);
}


// Updates the display by clearing the screen and displaying the current visualization
void GraphicsManager::display(Visualization& visualization)
{
	SDL_LockSurface(mScreen);
	clearScreen();

	std::vector<Pixel> const & pixels = visualization.getPixels();

	for (std::vector<Pixel>::const_iterator it = pixels.begin(); it != pixels.end(); it++)
	{
		Pixel pixel = *it;

		if (pixel.x >= 0 && pixel.y >= 0
			&& pixel.x < mScreen->w && pixel.y < mScreen->h)
		{
			Uint32 color(SDL_MapRGB(mScreen->format,
									static_cast<Uint8>(pixel.color.r),
									static_cast<Uint8>(pixel.color.g),
									static_cast<Uint8>(pixel.color.b)
									));

			setPixel(mScreen, pixel.x, pixel.y, color);
		}
	}

	SDL_UnlockSurface(mScreen);
	SDL_Flip(mScreen);
}


// Clears the screen with the default color
void GraphicsManager::clearScreen()
{
	SDL_FillRect(mScreen, NULL, SDL_MapRGB(mScreen->format, 0, 0, 0));
}


// Set the pixel at (x, y) to the given value
// NOTE: The surface must be locked before calling this
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

