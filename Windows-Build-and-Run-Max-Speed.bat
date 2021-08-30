g++ Main.cpp FluidSimulation/FluidSimulation.cpp Fluid/Fluid.cpp Engine/olcPixelGameEngine.cpp -o FluidSimulation.exe -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++20 -Ofast -march=native

call FluidSimulation.exe