#include "FluidSimulation.h"
#include "../Utility/Random.hpp"


FluidSimulation::FluidSimulation(const std::string& title, int width, int height, int pixel_width, int pixel_height, bool full_screen, bool vsync)
{
	this->sAppName = title;
	
	this->Construct(width, height, pixel_width, pixel_height, full_screen, vsync);
}



bool FluidSimulation::OnUserCreate()
{
	m_fluid = std::make_unique<Fluid>();
	m_previous_mouse_pos = olc::vf2d{ 0.0f, 0.0f };
	m_fluid_color = olc::CYAN;
	m_velocity_x = m_velocity_y = 0.0f;

	return true;
}


bool FluidSimulation::OnUserInput(float fElapsedTime) noexcept
{
	// Handle exit on Escape pressed
	if (GetKey(olc::Key::ESCAPE).bPressed)
	{
		return false;
	}

	// Change color randomly on space pressed
	if (GetKey(olc::Key::SPACE).bPressed)
	{
		m_fluid_color = this->RandomColor();
	}
	

	// Handle mouse drag effect
	const float mouseX = static_cast<float>(GetMouseX());
	const float mouseY = static_cast<float>(GetMouseY());

	if (GetMouse(0).bHeld)
	{
		// Add some of dye
		m_fluid->AddDensity((int)mouseX / SCALE, (int)mouseY / SCALE, Random::Real(100.0f, 1000.0f));

		// Apply Mouse Drag Velocity
		const float amountX = (float)GetMouseX() - m_previous_mouse_pos.x;
		const float amountY = (float)GetMouseY() - m_previous_mouse_pos.y;
		m_fluid->AddVelocity(static_cast<int>(mouseX / SCALE), static_cast<int>(mouseY / SCALE), amountX, amountY);
	}


	// Apply velocity on arrow keys pressed
	//static constexpr const float VELOCITY_X = 0.0f; // no velocity applied in left or right side
	//static constexpr const float VELOCITY_Y = 0.0f; // +y to apply velocity to bottom and simulate fluid falling down
	static constexpr float VELOCITY = 0.03f;
	if (GetKey(olc::Key::UP).bHeld)
	{
		m_velocity_y += -VELOCITY;
	}
	else if (GetKey(olc::Key::DOWN).bHeld)
	{
		m_velocity_y += VELOCITY;
	}
	if (GetKey(olc::Key::RIGHT).bHeld)
	{
		m_velocity_x += VELOCITY;
	}
	else if (GetKey(olc::Key::LEFT).bHeld)
	{
		m_velocity_x += -VELOCITY;
	}
	if (GetKey(olc::Key::R).bPressed) // reset velocity
		m_velocity_x = m_velocity_y = 0.0f;






	// set previous mouse position
	m_previous_mouse_pos = { mouseX, mouseY };
	return true;
}

bool FluidSimulation::OnUserUpdate(float fElapsedTime)
{
	// Update Fluid
	m_fluid->Update();

	// Draw Fluid
	for (int j = 0; j < N; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			const int x = i * SCALE;
			const int y = j * SCALE;

			// Get index from x,y coords
			const int index = IX(i, j);

			// Get Density 0 -> 255 alpha
			const float density = m_fluid->density[index];
			
			// Apply Fluid velocity
			m_fluid->AddVelocity(x, y, m_velocity_x * fElapsedTime, m_velocity_y * fElapsedTime);

			// Construct color based on density alpha
			//olc::Pixel color{ m_fluid_color.r, m_fluid_color.g, m_fluid_color.b,  };
			m_fluid_color.a = static_cast<std::uint8_t>(density);

			// Draw drop
			FillRect(x, y, SCALE, SCALE, m_fluid_color);
		}
	}

	// Clamp velocity
	m_velocity_x = std::clamp(m_velocity_x, -0.15f, 0.15f);
	m_velocity_y = std::clamp(m_velocity_y, -0.15f, 0.15f);

	return this->OnUserInput(fElapsedTime); // Handle user input & check for escape press to exit
}



bool FluidSimulation::OnUserDestroy()
{
	
	return true;
}