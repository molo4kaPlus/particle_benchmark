# Particle Simulation

A 2D particle simulation built using C++ and SFML, featuring particle interactions, gravity, collisions, and attraction forces. The simulation supports spatial grid optimization for efficient collision detection and includes an optional OpenMP implementation for parallel processing. A benchmarking system is also included to measure performance metrics like FPS, physics, and rendering times.

## Features
- **Particle Dynamics**: Simulates particles with position, velocity, acceleration, and customizable radius.
- **Collision Detection**: Uses a spatial grid for efficient collision checks, with configurable collision strength and damping.
- **Gravity and Attraction**: Applies gravity in any direction and supports mouse-based attraction forces.
- **Performance Benchmarking**: Measures and saves performance data (FPS, physics/render times) to a CSV file.
- **State Persistence**: Save and load particle states to/from binary files.
- **Parallel Processing**: Optional OpenMP support for parallelized physics updates (enabled with `#define OMP`).
- **Interactive Controls**: Add, remove, and manipulate particles in real-time using mouse and keyboard inputs.
- **Fast Square Root**: Optimized square root calculation using the Quake III algorithm for performance.

## Prerequisites
- **C++ Compiler**: Compatible with C++11 or later (e.g., GCC, MSVC, Clang).
- **SFML Library**: Version 2.x (Simple and Fast Multimedia Library).
- **OpenMP**: Optional, for parallel processing (supported by most modern compilers).
- **Arial Font**: The simulation uses `Arial.ttf` for rendering text. Ensure it's available in the project directory or replace it with another font in `engine.cpp`.

## Installation
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-username/particle-simulation.git
   cd particle-simulation
   ```

2. **Install SFML**:
   - On **Ubuntu/Debian**:
     ```bash
     sudo apt-get install libsfml-dev
     ```
   - On **macOS** (using Homebrew):
     ```bash
     brew install sfml
     ```
   - On **Windows**: Download SFML from [sfml-dev.org](https://www.sfml-dev.org/download.php) and follow the setup instructions for your compiler.

3. **Enable OpenMP (Optional)**:
   - Add `-fopenmp` to your compiler flags (e.g., for GCC/Clang).
   - Ensure `#define OMP` is set in `parameters.hpp` to use the OpenMP implementation.

4. **Build the Project**:
   - Using **CMake** (recommended):
     ```bash
     mkdir build
     cd build
     cmake ..
     make
     ```
   - Alternatively, compile manually with your compiler, linking SFML and OpenMP:
     ```bash
     g++ -std=c++11 *.cpp -o simulation -lsfml-graphics -lsfml-window -lsfml-system -fopenmp
     ```

5. **Run the Simulation**:
   ```bash
   ./simulation
   ```

## Usage
- The simulation runs in a 1600x900 window with a frame rate limit of 1200 FPS (configurable in `parameters.hpp`).
- Particles are created automatically up to a maximum of 1000 (configurable).
- Interact with the simulation using the controls below.
- Benchmark results are saved to `results.csv`, and particle states can be saved/loaded to/from `particles_state.bin`.

## Controls
- **Mouse**:
  - **Left Click + Drag**: Add new particles at the mouse position.
  - **Right Click + Drag**: Remove particles under the mouse cursor.
  - **Middle Click + Drag**: Activate attraction force, pulling particles toward the mouse.
- **Keyboard**:
  - **Space**: Pause/resume the simulation.
  - **S**: Save the current particle state to `particles_state.bin`.
  - **L**: Load a particle state from `particles_state.bin`.
  - **Up/Down/Left/Right Arrows**: Change gravity direction (up, down, left, right).
  - **B**: Run a benchmark for 1000 frames and save results to `results.csv`.

## Configuration
Key parameters are defined in `parameters.hpp`:
- `g_windowWidth`, `g_windowHeight`: Window dimensions.
- `g_fpsLimit`: Maximum frame rate.
- `g_maximumParticles`: Maximum number of particles.
- `g_particleRadius`: Particle size.
- `g_collisionCheckCount`: Number of collision detection passes per frame.
- `g_particleCollisionStrength`, `g_damp`: Collision response parameters.
- `g_attractionForce`, `g_attractionRadius`: Attraction force parameters.
- `g_benchmarkFrames`, `g_benchResultFileName`: Benchmark settings.

## Project Structure
- `main.cpp`: Entry point, initializes and runs the engine.
- `engine.hpp`, `engine.cpp`: Core simulation logic, including event handling, updates, and rendering.
- `particle.hpp`: Defines the `particle` struct with position, velocity, acceleration, radius, and color.
- `grid.hpp`: Implements a spatial grid for efficient collision detection.
- `updateOpenMP.cpp`: Physics update functions with OpenMP parallelization.
- `updateST.cpp`: Single-threaded physics update functions (used when OpenMP is disabled).
- `benchmark.cpp`: Benchmarking system to measure and save performance data.
- `qsqrt.cpp`: Fast square root implementation for performance optimization.
- `parameters.hpp`: Global configuration constants.

## Notes
- The simulation uses a spatial grid to optimize collision detection, reducing the complexity from O(n²) to O(n) in sparse scenarios.
- The OpenMP implementation (`updateOpenMP.cpp`) parallelizes gravity, collision, and attraction calculations but is disabled by default (enable with `#define OMP`).
- The fast square root algorithm (`qsqrt.cpp`) is inspired by the Quake III engine for efficient distance calculations.
- Benchmarking runs for a fixed number of frames (`g_benchmarkFrames`) and saves results in CSV format for analysis.

## Contributing
Contributions are welcome! Please submit issues or pull requests for bug fixes, performance improvements, or new features. Ensure code follows the existing style and includes appropriate comments.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments
- [SFML](https://www.sfml-dev.org/) for the graphics and windowing library.
- [OpenMP](https://www.openmp.org/) for parallel processing support.
- Quake III's fast inverse square root algorithm for inspiration.