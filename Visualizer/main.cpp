#include "Visualizer.h"


int main(int argc, char *argv[])
{
	Visualizer visualizer;

	if (visualizer.initialize() == true)
		visualizer.run();

	return 0;
}
