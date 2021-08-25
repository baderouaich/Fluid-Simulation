#pragma once
#include "../Constants.h"
#include "../Fluid/Fluid.h"
#include <random>


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
		static std::random_device seed{};
		static std::default_random_engine engine{ seed() };
		std::uniform_int_distribution<int> dist(0, 255);
		return olc::Pixel(static_cast<std::uint8_t>(dist(engine)), static_cast<std::uint8_t>(dist(engine)), static_cast<std::uint8_t>(dist(engine)), static_cast<std::uint8_t>(dist(engine)));
	}


private:
	std::unique_ptr<Fluid> m_fluid;
	olc::vf2d m_previous_mouse_pos;
	olc::Pixel m_fluid_color;
	float m_velocity_x;
	float m_velocity_y;


};
