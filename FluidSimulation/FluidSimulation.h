#pragma once
#include "../Constants.h"
#include "../Fluid/Fluid.h"



class FluidSimulation final : public olc::PixelGameEngine
{
private:
	Fluid* fluid;
	olc::vf2d previousMousePos;

public:
	FluidSimulation(const char* title,
		int width,
		int height,
		int pixel_width, 
		int pixel_height,
		bool full_screen,
		bool vsync);
	virtual ~FluidSimulation() = default;

public:
	virtual bool OnUserCreate() override;
	virtual bool OnUserUpdate(float fElapsedTime) override;
	virtual __forceinline void OnUserInput(const float& dt) noexcept;
	virtual bool OnUserDestroy() override;
};
