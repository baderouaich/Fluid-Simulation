sudo apt update

sudo apt install build-essential libglu1-mesa-dev libpng-dev

g++ Main.cpp FluidSimulation/FluidSimulation.cpp Fluid/Fluid.cpp Engine/olcPixelGameEngine.cpp -o FluidSimulation lX11 -lpthread -lpng -lstdc++fs -static -std=c++20 -Ofast -march=native

FluidSimulation