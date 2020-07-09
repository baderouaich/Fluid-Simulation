#include "FluidSimulation/FluidSimulation.h"

constexpr const auto TITLE = "Fluid Simulation";
constexpr const auto WINDOW_WIDTH = N * SCALE;
constexpr const auto WINDOW_HEIGHT = N * SCALE;
constexpr const auto PIXEL_WIDTH = 2;
constexpr const auto PIXEL_HEIGHT = 2;
constexpr const auto FULL_SCREEN = false;
constexpr const auto VSYNC = false;

int main(void) try
{
	std::unique_ptr<FluidSimulation> demo = std::make_unique<FluidSimulation>(
			TITLE, 
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			PIXEL_WIDTH,
			PIXEL_HEIGHT,
			FULL_SCREEN,
			VSYNC);

	demo->Start();

	return 0;
}
catch (const std::exception& e)
{
	std::cerr << "[ERROR]: " << e.what() << std::endl;
}
