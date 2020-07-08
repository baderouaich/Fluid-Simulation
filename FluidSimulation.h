#pragma once
#include "Engine/olcPixelGameEngine.h"
#include <memory>
#include <cassert>

class FluidSimulation final : public olc::PixelGameEngine
{

public:
	FluidSimulation(const char* title, int width, int height, int pixel_width, int pixel_height);
	virtual ~FluidSimulation() = default;

public:
	virtual bool OnUserCreate() override;
	virtual bool OnUserUpdate(float fElapsedTime) override;
	virtual void OnUserInput(const float& dt);
	virtual bool OnUserDestroy() override;

};
