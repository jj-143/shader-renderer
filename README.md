![screenshot][]

# Shader Renderer

Tiny app for GLSL shader coding

## Features

### Compositor Rack

- 2 shader nodes: `ComputeShaderNode`, `FragShaderNode`
- Activate / Deactivate node
- Add / Edit uniforms

### Project

- Project file (.json) for save / load
- Load a single compute shader file, imported as a single node setup

### Render to file

- Current frame / Range of frames to PNG

### Misc

- FPS style viewport navigation

  - `<Tab>` to switch between camera modes

- Partial [Shadertoy][] compatibility

  - `iTime`, `iResolution`, `mainImage()`

- Automatically compiles shaders on file changes

## CLI Usage

```
shader-renderer [<width>] [<height>] [<file>]

Options:
  width       viewport width (default: 768)
  height      viewport height (default: 768)
  file        project file (.json) / shader file
```

## Development

### Requirements

- CMake
- GLFW
- \[Linux\] gtk3, glib2 for [nativefiledialog][]

### Build

Build artifacts will be in `build/output`

#### Linux

```bash
mkdir build
cd build
cmake ..
make
```

#### Windows, tested on MinGW (UCRT)

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make.exe
```

Or, use VS Code with CMake Tools extension (MinGW Kit).

<!-- Links -->

[screenshot]: docs/screenshot.png
[Shadertoy]: https://shadertoy.com
[nativefiledialog]: https://github.com/mlabbe/nativefiledialog
