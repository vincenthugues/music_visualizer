#pragma once


class Visualization
{
public:
	Visualization();
	virtual ~Visualization();

	void initialize(int spectrumSize);
	virtual void update(float spectrum[]) = 0;

private:
	int mSpectrumSize;
};
