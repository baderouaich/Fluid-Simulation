#pragma once
#include "../Constants.h"
#include "../Fluid/Fluid.h"
#include "../Utility/Random.hpp"


class FluidSimulation final : public olc::PixelGameEngine
{
public:
	FluidSimulation(const std::string& title,
		int width,
		int height,
		int pixel_width, 
		int pixel_height,
		bool full_screen,
		bool vsync);
	~FluidSimulation() = default;

public:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	bool OnUserInput(float fElapsedTime) noexcept;
	bool OnUserDestroy() override;

private:
	olc::Pixel RandomColor() const noexcept
	{
		return olc::Pixel
		{
			static_cast<std::uint8_t>(Random::Int<std::uint16_t>(0, 255)),
			static_cast<std::uint8_t>(Random::Int<std::uint16_t>(0, 255)),
			static_cast<std::uint8_t>(Random::Int<std::uint16_t>(0, 255)),
			static_cast<std::uint8_t>(Random::Int<std::uint16_t>(0, 255))
		};
	}


private:
	std::unique_ptr<Fluid> m_fluid;
	olc::vf2d m_previous_mouse_pos;
	olc::Pixel m_fluid_color;
	float m_velocity_x;
	float m_velocity_y;


};
