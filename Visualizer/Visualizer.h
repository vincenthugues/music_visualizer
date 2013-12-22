#pragma once

#include <SDL.h>
#include <fmod.h>

#define	WINDOW_WIDTH		1024
#define	WINDOW_HEIGHT		768
#define	RATIO			(WINDOW_HEIGHT / 255.0)
#define	REFRESH_RATE		25
#define	SPECTRUM_SIZE		512
#define GRAPH_STEP		(WINDOW_WIDTH / SPECTRUM_SIZE)


class Visualizer
{
public:
	struct Data
	{
		SDL_Surface*	screen;
		FMOD_SYSTEM*	system;
		FMOD_SOUND*		music;
		FMOD_CHANNEL*	channel;
		FMOD_RESULT		result;
		char*			soundPath;
		float			spectrum[SPECTRUM_SIZE];
		bool			running;
	};
	
public:
	Visualizer();
	~Visualizer();

	bool initialize();
	void run();

private:
	void handleEvents();
	void handleTime();
	void processMusic();
	bool fmodInit();
	bool SDLInit();
	void dataInit();

	void displayPointBars(int x, int y);
	void displayBars();
	void displayPointSpread(int x, int y, Uint32 color);
	void displaySpread();
	void displayGraph();
	void clearScreen();

private:
	Data mData;
};
