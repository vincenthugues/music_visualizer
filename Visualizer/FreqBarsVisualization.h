#pragma once

#include "Visualization.h"


class FreqBarsVisualization : public Visualization
{
public:
	FreqBarsVisualization();
	~FreqBarsVisualization();

	void update(float spectrum[]);
};
