# Ray Tracer
This is an offline ray tracer that runs entirely on the CPU (GPU support might be added at some point). The UI is currently very limited, so most things need to be configured through code, but it supports importing OBJ meshes, mesh instance translation and rotation, diffuse, reflective, and emissive materials, multi threading, and various performance optimisaions including per-object bounding volume hierarchies.

All ray-tracing/mathematics related functionality has been implemented from scratch, with SDL2 being used just to draw pixels to a window and ImGui for the UI.

## Example Render ([Stanford Dragon](https://graphics.stanford.edu/data/3Dscanrep/))
<img width="1182" height="832" alt="dragon" src="https://github.com/user-attachments/assets/8ba31b3b-da49-48e4-b2bb-fc16385187ef" />

## Compiling
Install SDL2 and ImGui, for example using a package manager like vcpkg or compiling them from source, and then build the project with CMake.
