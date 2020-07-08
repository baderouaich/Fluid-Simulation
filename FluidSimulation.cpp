#include "FluidSimulation.h"


FluidSimulation::FluidSimulation(const char* title, int width, int height, int pixel_width, int pixel_height)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	this->sAppName = std::move(title);
	this->Construct(width, height, pixel_width, pixel_height, false, false);
}



bool FluidSimulation::OnUserCreate()
{


	return true;
}


void FluidSimulation::OnUserInput(const float& dt)
{
	
}

bool FluidSimulation::OnUserUpdate(float fElapsedTime)
{
	this->OnUserInput(fElapsedTime);

	


	return true;
}



bool FluidSimulation::OnUserDestroy()
{
	//Cleanup


	return true;
}
