#pragma once

#include "Visualization.h"


// Displays vertical bars whose heights are proportional to the corresponding frequency
class FrequencyBars : public Visualization
{
public:
	FrequencyBars();
	~FrequencyBars();

	void update(const float spectrum[], int windowWidth, int windowHeight, float graphStep);
};
