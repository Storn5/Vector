# Vector
A simple 3D game written in C++ with graphics implemented from scratch in DirectX, OpenGL and Vulkan, inspired by X-Wing (1993), Elite (1984) and X4: Foundations

## Roadmap
🟢 - done, 🚧 - WIP, 🔴 - planned

### Graphics APIs
🚧 DirectX 11 graphics backend (Windows-only)

&nbsp;&nbsp;&nbsp;&nbsp;🟢 Support for NVIDIA Optimus and AmdPowerXpress

&nbsp;&nbsp;&nbsp;&nbsp;🔴 Make the GPU selection happen in the window instead of the terminal

🔴 DirectX 12 graphics backend (Windows-only)

🚧 OpenGL 4.6 graphics backend (cross-platform)

&nbsp;&nbsp;&nbsp;&nbsp;🔴 GPU selection

🔴 Vulkan graphics backend (cross-platform)

### Game Engine
🚧 Basic engine functionality (wrapper around graphics APIs)

&nbsp;&nbsp;&nbsp;&nbsp;🟢 Creating/closing a window

&nbsp;&nbsp;&nbsp;&nbsp;🚧 Loading shaders

&nbsp;&nbsp;&nbsp;&nbsp;🔴 Creating/moving/destroying 3D objects

&nbsp;&nbsp;&nbsp;&nbsp;🚧 Graphics loop (Rendering all objects in 3D environment)

🟢 Configuring settings (command-line args)

&nbsp;&nbsp;&nbsp;&nbsp;🟢 Select graphics API

&nbsp;&nbsp;&nbsp;&nbsp;🔴 Switch graphics API at runtime

&nbsp;&nbsp;&nbsp;&nbsp;🟢 Resolution

&nbsp;&nbsp;&nbsp;&nbsp;🟢 V-Sync

&nbsp;&nbsp;&nbsp;&nbsp;🟢 Fullscreen/windowed mode

&nbsp;&nbsp;&nbsp;&nbsp;🔴 Borderless fullscreen support

&nbsp;&nbsp;&nbsp;&nbsp;🔴 Switch windowe/fullscreen/borderless and V-Sync at runtime

🔴 Replace GLFW and Win32 with a common cross-platform windowing API (SDL, SFML, QT?)

🔴 Retro-style wireframe rendering

### Game
🔴 Spaceship movement in 3 dimensions

🔴 A sipmle GUI with a minimap

🔴 Other spaceships to fight against

🔴 Simple background objects (stars, planets, moons, asteroids)