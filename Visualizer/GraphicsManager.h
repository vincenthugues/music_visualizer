#pragma once

#include <SDL.h>


class GraphicsManager
{
public:
	GraphicsManager();
	~GraphicsManager();
	
	void initialize(int windowWidth, int windowHeight, int spectrumSize);
	void update(const float spectrum[]);
	void rotateVisualizations();

private:
	void clearScreen();
	void displayCenteredBars(const float spectrum[]);
	void displayBars(const float spectrum[]);
	void displaySpread(const float spectrum[]);
	void displayPointSpread(int x, int y, Uint32 color);
	void setPixel(SDL_Surface *surface, const int x, const int y, const Uint32 pixel);

private:
	SDL_Surface* mScreen;
	int mWindowWidth;
	int mWindowHeight;
	int mSpectrumSize;
	float mGraphStep;
	int mVisualization;
};
