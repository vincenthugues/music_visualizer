#include "FreqBarsVisualization.h"


FreqBarsVisualization::FreqBarsVisualization()
: Visualization()
{
}


FreqBarsVisualization::~FreqBarsVisualization()
{
}


void FreqBarsVisualization::update(const float spectrum[], int windowWidth, int windowHeight, float graphStep)
{
	int value = 0, x = 0;
	int y, i;
	float ratio = windowWidth / 255.0f;

	mPixels.clear();

	for (i = 0; x < windowWidth && i < mSpectrumSize; ++i)
	{
		value = static_cast<int>(spectrum[i] * 20.0f * windowHeight);
		if (value >= windowHeight)
			value = windowHeight - 1;
		if (value < 0)
			value = 0;

		for (y = windowHeight - value; y < windowHeight; y++)
		{
			Color color;
			Pixel pixel;
			
			color.r = 255 - (y / ratio);
			color.g = y / ratio;
			color.b = 0;
			pixel.x = x;
			pixel.y = y;
			pixel.color = color;

			mPixels.push_back(pixel);
		}

		x += static_cast<int>(graphStep);
	}
}
