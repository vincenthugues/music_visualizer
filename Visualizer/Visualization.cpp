#include "Visualization.h"


Visualization::Visualization() :
	mSpectrumSize(-1),
	mPixels()
{
}


Visualization::~Visualization()
{
}


std::vector<Pixel> const & Visualization::getPixels()
{
	return mPixels;
}


void Visualization::initialize(int spectrumSize)
{
	mSpectrumSize = spectrumSize;
}
