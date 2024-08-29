# Vector
A simple 3D game written in C++ with graphics implemented from scratch in DirectX, OpenGL and Vulkan, inspired by X-Wing (1993), Elite (1984) and X4: Foundations

## Roadmap
🟢 - done, 🚧 - WIP, 🔴 - planned

### Graphics APIs
🚧 OpenGL 4.6 graphics backend

&nbsp;&nbsp;&nbsp;&nbsp;🔴 Win32 API window management as an alternative to GLFW

🚧 DirectX 11 graphics backend

🔴 DirectX 12 graphics backend

🔴 Vulkan graphics backend

### Game Engine
🚧 Basic engine functionality (wrapper around graphics APIs to create a window & render things)

🚧 Configuring settigns

&nbsp;&nbsp;&nbsp;&nbsp;🟢 Fullscreen

&nbsp;&nbsp;&nbsp;&nbsp;🔴 Resolution

&nbsp;&nbsp;&nbsp;&nbsp;🟢 Switch between graphics backends

&nbsp;&nbsp;&nbsp;&nbsp;🔴 Switch between GLFW and Win32 windows for OpenGL

🔴 Retro-style wireframe rendering

### Game
🔴 Spaceship movement in 3 dimensions

🔴 A sipmle GUI with a minimap

🔴 Other spaceships to fight against

🔴 Simple background objects (stars, planets, moons, asteroids)