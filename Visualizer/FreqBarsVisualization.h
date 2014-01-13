#pragma once

#include "Visualization.h"


class FreqBarsVisualization : public Visualization
{
public:
	FreqBarsVisualization();
	~FreqBarsVisualization();

	void update(const float spectrum[], int windowWidth, int windowHeight, float graphStep);
};
