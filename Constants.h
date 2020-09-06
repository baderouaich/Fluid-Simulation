#pragma once
#include "Engine/olcPixelGameEngine.h"
#include <algorithm>
#include <cassert>
#include <memory>
#include <utility>
#include <random>

namespace
{
	constexpr const float PI = 3.14159265358979323846f;

	constexpr const int SCALE = 3;
	constexpr const int N = 160;
	constexpr const int ITERATIONS = 16;

	constexpr const float VESCOSITY = 0.0000001f;
	constexpr const float DIFFUSION = 0.0f;
	constexpr const float MOTION_SPEED = 0.2f;

	static const olc::Pixel FLUID_COLOR = olc::CYAN;

	constexpr const float VELOCITY_X = 0.0f; // no velocity applied in left or right side
	constexpr const float VELOCITY_Y = 0.0f; // +y to apply velocity to bottom and simulate fluid falling down


	template<class T>
	constexpr const __forceinline T& Clamp(const T& v, const T& lo, const T& hi) noexcept
	{
		return (v < lo) ? lo : (hi < v) ? hi : v;
	}

	/*
	* Converts 2D coords into 1D ( x,y into index )
	*/
	static __forceinline int IX(int x, int y) noexcept
	{
		x = Clamp(x, 0, N - 1);
		y = Clamp(y, 0, N - 1);
		return x + (y * N);
	}
}
