#include "FluidSimulation/FluidSimulation.h"
#include "Constants.h"

constexpr const auto TITLE = "Fluid Simulation";
constexpr const auto WINDOW_WIDTH = N * SCALE;
constexpr const auto WINDOW_HEIGHT = N * SCALE;
constexpr const auto PIXEL_WIDTH  = 2;
constexpr const auto PIXEL_HEIGHT = 2;
constexpr const auto FULL_SCREEN = false;
constexpr const auto VSYNC = false;


int main(void)
{
	std::cout << "+============== Controls ==============+\n";
	std::cout << "+ Press SPACE to change fluid color randomly\n";
	std::cout << "+ Press Arrow Keys to apply fluid velocity and R to reset it\n";
	std::cout << "+ Press ESCAPE to exit the simulation\n\n";
	std::cout << "+ Press ENTER to start...\n";
	std::cin.get();


	const auto simula = std::make_unique<FluidSimulation>(
			TITLE,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			PIXEL_WIDTH,
			PIXEL_HEIGHT,
			FULL_SCREEN,
			VSYNC);

	simula->Start();

	return 0;
}
