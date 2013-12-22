#pragma once

#include <fmod.h>

#include "GraphicsManager.h"


#define	WINDOW_WIDTH		1024
#define	WINDOW_HEIGHT		768
#define	REFRESH_RATE		25
#define	SPECTRUM_SIZE		512


class Visualizer
{
public:
	struct Data
	{
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
	bool fmodInit();

private:
	Data mData;
	GraphicsManager mGraphicsManager;
};
