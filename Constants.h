#pragma once
#include "Engine/olcPixelGameEngine.h"
#include <algorithm>
#include <cassert>
#include <memory>
#include <utility>
#include <random>

#define PI 3.14159265358979323846f

constexpr const int N = 128; // Width & Height of square window (width, height)
constexpr const int ITERATIONS = 16; // Number of iterations
constexpr const int SCALE = 4;

constexpr const float VESCOSITY = 0.0000001f;
constexpr const float DIFFUSION = 0.0f;
constexpr const float MOTION_SPEED = 0.2f;


static const olc::Pixel CLEAR_COLOR = olc::GREY;
static const olc::Pixel FLUID_COLOR = olc::DARK_CYAN;

constexpr const float VELOCITY_X = 0.0f; // no velocity applied in left or right side
constexpr const float VELOCITY_Y = +0.5f; // +y to apply velocity to bottom and simulate fluid fall



namespace
{
	/*
	* Re-maps a number from one range to another. 
	*/
	static float map(float value, float start1, float stop1, float start2, float stop2)
	{
		return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
	}

	/*
	* Converts 2D coords into 1D ( x,y into index )
	*/
	static int IX(int x, int y)
	{
		x = std::clamp(x, 0, N - 1); // avoid array out of bounds
		y = std::clamp(y, 0, N - 1);

		return x + (y * N);
	}
}
