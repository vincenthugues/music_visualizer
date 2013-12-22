#pragma once

#include <SDL.h>


class GraphicsManager
{
public:
	GraphicsManager();
	~GraphicsManager();
	
	void initialize(int windowWidth, int windowHeight, int spectrumSize);
	void update(float spectrum[]);

private:
	void clearScreen();
	void displayBars(float spectrum[]);
	void displayPointBars(int x, int y, float ratio);
	void displaySpread(float spectrum[]);
	void displayPointSpread(int x, int y, Uint32 color);
	void setPixel(SDL_Surface *surface, const int x, const int y, const Uint32 pixel);

private:
	SDL_Surface* mScreen;
	int mWindowWidth;
	int mWindowHeight;
	int mSpectrumSize;
	float mGraphStep;
};
