#pragma once

#include <SDL.h>

#include "Visualization.h"


class GraphicsManager
{
public:
	GraphicsManager();
	~GraphicsManager();
	
	void initialize(int windowWidth, int windowHeight, int spectrumSize);
	void update(Visualization& visualization, const float spectrum[]);
	void display(Visualization& visualization);
	
private:
	void clearScreen();
	void setPixel(SDL_Surface *surface, const int x, const int y, const Uint32 pixel);

private:
	SDL_Surface* mScreen;
	int mWindowWidth;
	int mWindowHeight;
	int mSpectrumSize;
	float mGraphStep;
};
