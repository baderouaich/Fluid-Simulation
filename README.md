# Fluid Simulation
  
Attempt to Simulate Fluid, using <strong>Lattice Boltzmann Method</strong> and the <strong>olc::PixelGameEngine</strong>.

## Overview
https://github.com/user-attachments/assets/8702a958-ca6d-4be8-b236-d518cc9f7d30

## Build & Run
```sh
git clone https://github.com/baderouaich/Fluid-Simulation
cd Fluid-Simulation
mkdir build && cd build
cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
make -j8
./fluid_simulation
```
#### Controls
- **Mouse** movement to apply velocity
- **SPACE** key to change fluid color randomly
- **Arrow** Keys to apply fluid velocity
- **R** to reset the velocity to default
- **ESCAPE** key to exit the simulation


## Reference
https://mikeash.com/pyblog/fluid-simulation-for-dummies.html


