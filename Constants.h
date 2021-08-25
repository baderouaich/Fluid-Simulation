#pragma once
#include "Engine/olcPixelGameEngine.h"
#include <algorithm> // std::clamp
#include <cassert>
#include <memory>
#include <utility>
#include <random>

namespace
{
	static constexpr const float PI = 3.14159265358979323846f;

	static constexpr const int SCALE = 3;
	static constexpr const int N = 160;
	static constexpr const int ITERATIONS = 16;

	static constexpr const float VESCOSITY = 0.0000001f; // thickness of fluid
	static constexpr const float DIFFUSION = 0.0f;
	static constexpr const float MOTION_SPEED = 0.2f;

	/**
	* Converts 2D coords into 1D ( x,y into index )
	*/
	template<class T>
	static constexpr T IX(T x, T y) noexcept
	{
		x = std::clamp(x, 0, N - 1);
		y = std::clamp(y, 0, N - 1);
		return x + (y * N);
	}
}
