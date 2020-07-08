#include "FluidSimulation.h"

constexpr const auto TITLE = "Fluid Simulation";
constexpr const auto WINDOW_WIDTH = 640;
constexpr const auto WINDOW_HEIGHT = 480;
constexpr const auto PIXEL_WIDTH = 2;
constexpr const auto PIXEL_HEIGHT = 2;

int main(void) try
{
	std::unique_ptr<FluidSimulation> demo = std::make_unique<FluidSimulation>(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, PIXEL_WIDTH, PIXEL_HEIGHT);
	demo->Start();
	return 0;
}
catch (const std::exception& e)
{
	std::cerr << "[ERROR]: " << e.what() << std::endl;
}
