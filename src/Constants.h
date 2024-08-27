#pragma once

#include "Engine/olcPixelGameEngine.h"
#include <algorithm> // std::clamp
#include <cassert>
#include <memory>
#include <utility>
#include <random>

namespace {
  static constexpr const int SCALE = 2;
  static constexpr const int N = 480 / SCALE;
  static constexpr const int ITERATIONS = 8;

  static constexpr const float VESCOSITY = 0.0000001f; // thickness of fluid
  static constexpr const float DIFFUSION = 0.0f;
//	static constexpr const float MOTION_SPEED = 1.0f / 60.0f;

  /**
  * Converts 2D coords into 1D ( x,y into index )
  */
  template<class T>
  static T IX(T x, T y) noexcept {
    x = std::clamp(x, 0, N - 1);
    y = std::clamp(y, 0, N - 1);
    return x + (y * N);
  }
}
