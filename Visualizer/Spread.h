#pragma once

#include "Visualization.h"


// Displays pixels that seem to be spreading from fixed sources in a cone-shaped area
class Spread : public Visualization
{
public:
	Spread();
	~Spread();
	
	void addPoints(int x, int y, int windowWidth, int windowHeight, Color color);
	void update(const float spectrum[], int windowWidth, int windowHeight, float graphStep);
};
