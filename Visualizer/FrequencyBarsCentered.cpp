#include "FrequencyBarsCentered.h"


FrequencyBarsCentered::FrequencyBarsCentered()
: Visualization()
{
}


FrequencyBarsCentered::~FrequencyBarsCentered()
{
}


void FrequencyBarsCentered::update(const float spectrum[], int windowWidth, int windowHeight, float graphStep)
{
	int value = 0, x = 0;
	int y, i;
	float ratio = 0;

	mPixels.clear();
	
	for (i = 0; x < windowWidth && i < mSpectrumSize; ++i)
	{
		value = static_cast<int>(spectrum[i] * 10 * windowHeight);
		if (value >= windowHeight)
			value = windowHeight - 1;
		if (value < 0)
			value = 0;
	
		ratio = (float)value / (float)windowHeight;
		for (y = -value / 2; y < value / 2; y++)
		{
			Color color;
			Pixel pixel;

			color.r = ratio * 255.0f;
			color.g = (1 - ratio) * 128.0f;
			color.b = (1 - ratio) * 255.0;
			pixel.x = x;
			pixel.y = y + windowHeight / 2;
			pixel.color = color;

			mPixels.push_back(pixel);
		}
		
		x += static_cast<int>(graphStep);
	}
}
