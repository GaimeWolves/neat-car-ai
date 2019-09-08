# NEAT Car AI
A project using the NEAT algorithm to make a car drive along a track.

The implementation of the NEAT algorithm is completely independent from the rest of the code. To use the NEAT implementation in your own code copy the **src/NEAT** directory into your **src** directory.

Using SDL2_gfx rendering requires you to define **USE_SDL2_RENDERING** in the preprocessor.

## Building from source
### Linux

##### Requirements
- A C++ compiler
- CMake
- SDL2
- SDL2_gfx

##### Build Process
If you haven't installed cmake, install it with (adapt for other Linux distros):

```bash
$ sudo apt-get install cmake
```

After that use cmake to build the configuration files and build the project:
```bash
$ cmake -H. -Bbuild
$ cmake --build build
```

##### Running

The executable file should reside in the bin folder and can be executed with:
```bash
$ ./bin/CarAI
```

##### Library errors

If build errors about SDL2 and SDL2_gfx occur, install the respective libraries or change the library paths in the CMake files to your custom install locations for SDL2 or SDL2_gfx. Here are links to the libraries:
- [SDL2](https://www.libsdl.org/index.php)
- [SDL2_gfx](http://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/)

### Windows
On Windows the libraries are prebuilt and reside in the libs folder. All includes for Windows reside in the include folder.
##### Requirements
- A C++ Compiler (for example VS 2019)
- [CMake](https://cmake.org)

##### Build Process
After you downloaded the source run CMake in the command prompt with:
```shell
> cmake -H. -Bbuild
> cmake --build build
```

##### Running
The executable file should reside in the bin folder.