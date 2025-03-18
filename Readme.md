[![ci](https://github.com/emrekovanci/Template-SDL/actions/workflows/ci.yml/badge.svg)](https://github.com/emrekovanci/Template-SDL/actions/workflows/ci.yml)

Template project for developing SDL applications.

This project uses the GPU API introduced with SDL3+, and relies on [shadercross][5] to compile shaders at runtime.
Currently, it is waiting for shadercross to be fully released. It should only work on Windows for now. However, if you replace shadercross folder with the appropriate version macOS or Linux, the project should work on those platforms also as well.
You can get the binaries for shadercross from [action artifacts][6]

### CMake Presets

This project makes use of [CMake Presets][1] to simplify the process of configuring
the project.

To build the project, please make sure you have [vcpkg][2]
installed and properly set up at least with the `VCPKG_ROOT` environment variable pointing
to the right place.

### List Presets

```powershell
cmake --list-presets=all
```

### Configure & Build & Test

```powershell
cmake --preset=<configurePreset-name>
```

```powershell
cmake --build --preset=<buildPreset-name>
```

```powershell
ctest --preset=<testPreset-name>
```

### Development on Windows

- Sourcing the Environment
  - If you're building with Ninja and the Visual C++ toolset on Windows, set the environment before CMake is called to generate the build system. You can do it by calling [vcvarsall.bat][3] with the `architecture` argument.

    ```powershell
    /path/to/vcvarsall.bat x64
    ```

- [Clang/LLVM support in Visual Studio][4]

[1]: https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html
[2]: https://github.com/microsoft/vcpkg
[3]: https://learn.microsoft.com/en-us/cpp/build/cmake-presets-vs?view=msvc-170#sourcing-the-environment-when-building-with-command-line-generators-on-windows
[4]: https://learn.microsoft.com/en-us/cpp/build/clang-support-msbuild?view=msvc-170
[5]: https://github.com/libsdl-org/SDL_shadercross
[6]: https://github.com/libsdl-org/SDL_shadercross/actions
