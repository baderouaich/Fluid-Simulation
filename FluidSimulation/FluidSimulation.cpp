#include "FluidSimulation.h"


FluidSimulation::FluidSimulation(const char* title, int width, int height, int pixel_width, int pixel_height, bool full_screen, bool vsync)
{
	this->sAppName = std::move(title);
	this->Construct(width, height, pixel_width, pixel_height, full_screen, vsync);
}



bool FluidSimulation::OnUserCreate()
{
	fluid = new Fluid(MOTION_SPEED, DIFFUSION, VESCOSITY);
	previousMousePos = { 0.0f, 0.0f };

	Clear(CLEAR_COLOR); 

	return true;
}


void FluidSimulation::OnUserInput(const float& dt)
{
	float mouseX = (float)GetMouseX();
	float mouseY = (float)GetMouseY();

	if (GetMouse(0).bHeld)
	{
		//Add some of dye
		fluid->AddDensity((int)mouseX / SCALE, (int)mouseY / SCALE, Random::Real(100.0f, 1000.0f)); // random value of dye, more fluid

		//Apply Mouse Drag Velocity
		float amountX = (float)GetMouseX() - previousMousePos.x;
		float amountY = (float)GetMouseY() - previousMousePos.y;
		fluid->AddVelocity((int)mouseX / SCALE, (int)mouseY / SCALE, amountX, amountY);
	}

	previousMousePos = { mouseX, mouseY };

}

bool FluidSimulation::OnUserUpdate(float fElapsedTime)
{
	this->OnUserInput(fElapsedTime);


	//Update Fluid
	fluid->Step();



	//Draw Fluid
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			int x = i * SCALE;
			int y = j * SCALE;

			//Get index from x,y coords
			int index = IX(i, j);

			//Get Density 
			float density = fluid->density[index];

			//Apply Fluid velocity
			fluid->AddVelocity(x, y, VELOCITY_X * fElapsedTime, VELOCITY_Y * fElapsedTime);

			//Construct color based on density alpha
			olc::Pixel color = olc::PixelF(FLUID_COLOR.r / 255.0f, FLUID_COLOR.g / 255.0f, FLUID_COLOR.b / 255.0f, density / 255.0f);
			

			//Draw drop
			FillRect(x, y, SCALE, SCALE, color);
		}
	}


	return true;
}



bool FluidSimulation::OnUserDestroy()
{
	//Cleanup
	if (fluid)
	{
		delete fluid;
		fluid = nullptr;
	}
	return true;
}