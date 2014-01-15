#include "Spread.h"


Spread::Spread()
{
}


Spread::~Spread()
{
}


void Spread::addPoints(int x, int y, int windowWidth, int windowHeight, Color color)
{
	Pixel pixel;

	pixel.color = color;

	pixel.x = x;
	pixel.y = y;
	mPixels.push_back(pixel);

	pixel.x = x;
	pixel.y = windowHeight - y - 1;
	mPixels.push_back(pixel);

	pixel.x = windowWidth - x - 1;
	pixel.y = y;
	mPixels.push_back(pixel);
	
	pixel.x = windowWidth - x - 1;
	pixel.y = windowHeight - y - 1;
	mPixels.push_back(pixel);
}


void Spread::update(const float spectrum[], int windowWidth, int windowHeight, float graphStep)
{
	int value = 0, x = 0;
	int y, tmp, i;
	float ratio = windowHeight / 255.0f;
	
	mPixels.clear();
	
	for (i = 0; x < windowWidth && i < mSpectrumSize; ++i)
	{
		value = static_cast<int>(spectrum[i] * 20 * windowHeight);
		if (value >= windowHeight)
			value = windowHeight - 1;
		if (value < 0)
			value = 0;
	
		y = windowHeight - value;
		if (y >= 0 && y < windowHeight)
		{
			Color color;
			
			color.r = static_cast<char>(255 - (1.5 * y / ratio));
			color.g = static_cast<char>(y / ratio);
			color.b = 0;

			addPoints(x, y, windowWidth, windowHeight, color);

			tmp = x;
			x = y * windowHeight / windowWidth;
			y = tmp * windowHeight / windowHeight;
			addPoints(y, x, windowWidth, windowHeight, color);
		}
	
		x += static_cast<int>(graphStep);
	}
}
