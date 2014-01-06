#pragma once

#include <vector>

#include "GraphicsManager.h"
#include "SoundManager.h"
#include "Visualization.h"


#define	WINDOW_WIDTH		1024
#define	WINDOW_HEIGHT		768
#define	REFRESH_RATE		25
#define	SPECTRUM_SIZE		512


class Visualizer
{
public:
	Visualizer();
	~Visualizer();

	void initialize();
	void run();

private:
	void handleEvents();
	void handleTime();
	
private:
	GraphicsManager mGraphicsManager;
	SoundManager mSoundManager;
	std::vector<Visualization> mVisualizers;
	std::string mMusicPath;
	bool mIsRunning;
};
