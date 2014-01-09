#pragma once

#include <vector>


struct Color
{
	char r;
	char g;
	char b;
};

struct Pixel
{
	int x;
	int y;
	Color color;
};


class Visualization
{
public:
	Visualization();
	virtual ~Visualization();

	std::vector<Pixel> const & getPixels();

	void initialize(int spectrumSize);
	virtual void update(float spectrum[]) = 0;
	
protected:
	int mSpectrumSize;
	std::vector<Pixel> mPixels;
};
