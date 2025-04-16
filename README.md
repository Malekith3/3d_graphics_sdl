# CPU 3D Renderer – Pikuma Course (C++23)

This project is based on the excellent [**Pikuma 3D Graphics Programming**](https://pikuma.com/courses/learn-3d-computer-graphics-programming) course.  
It implements a full **3D graphics pipeline from scratch** on the **CPU**, with no reliance on GPU acceleration.  
All rendering is done manually, step by step, to gain a deep understanding of how real-time 3D rendering works under the hood.

---

## Goals

- Learn how a 3D rendering pipeline works by implementing it manually.
- Reinforce graphics fundamentals like transformation matrices, clipping, rasterization, and depth buffering.
- Gain experience with **modern C++ (C++23)** while porting from the original ANSI C course.

---

##  Libraries

- **C++23** – Modern C++ to explore newer language features.
- **SDL2** – Used only for window creation and framebuffer swapping and event handling.
- **GLM** – Math library for vector/matrix operations.

> The project is fully self-contained. You only need CMake and a compiler that supports C++23.  
> SDL2 and GLM are downloaded automatically during the build process.

---

##  Current Status

**Still under development** – I haven't completed the full course yet, so this project is a work in progress.  
Features and rendering stages will be added as I progress through the lessons.

---

##  How to Build

All you need is **CMake ≥ 3.15** and a GNU compiler that supports **C++23**.

> ⚠️ **Disclaimer**:  
> This project currently relies on internal headers from GCC’s `libstdc++` (specifically `<bits/ranges_algobase.h>`), which are **not part of the C++ standard**.  
> As a result, **it will not compile** with **MSVC** or **Clang/libc++** at this point.  
> This is a temporary situation — the goal is to **refactor** the code to use only standard-compliant headers and make the project **compiler-agnostic**.
### Steps to Build:

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/Malekith3/3d_graphics_sdl.git
   ```
2. **Build**
   ### Windows
    ````bash
    cmake -B build -G "MinGW Makefiles"
    cmake --build build
    ````
   ### Linux
    ````bash
   cmake -B build
   cmake --build build
    ````
3. **Run**
   ````bash
   #Linux
    ./build/MinimalSDL2App
   #Windows
   build\MinimalSDL2App.exe
    ````
   

> On the first run, the following dependencies will be cloned automatically:
> * SDL2 – for window/context and event handling
> * GLM – for math (matrices, vectors, transforms)

---
## Notes
This goes without saying, but this is a learning project — not something intended for production use.
Feel free to explore the code, fork the repository, or reach out if you have any questions or suggestions!