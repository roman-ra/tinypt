# tinypt

Simple multi-threaded path tracer with support for rendering sphere primitives. Written in C++11 without any dependencies except the standard library.

![Render](out.png?raw=true "Render")

Currently suported material types:
* Diffuse (Lambertian BRDF)
* Mirror
* Emission

Todo:
* Refactor
* Support for textures
* Support for more material types
* Support for triangulated meshes (requires an accelaration structure like BVH)

Building instructions:
```
mkdir build
cd build
cmake ..
make
```