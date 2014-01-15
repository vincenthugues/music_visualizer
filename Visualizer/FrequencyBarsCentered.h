#pragma once

#include "Visualization.h"


// Displays vertically centered vertical bars whose heights are proportional to the corresponding frequency
class FrequencyBarsCentered : public Visualization
{
public:
	FrequencyBarsCentered();
	~FrequencyBarsCentered();

	void update(const float spectrum[], int windowWidth, int windowHeight, float graphStep);
};
