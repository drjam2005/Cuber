Cuber (Name on WIP still)
===
quick little project so i could play the cstimer.net virtual cube offline lol

# How To Play
- Moves are just the same as in cstimer.net
- Ctrl + Q -> quit


## Building and Running
> [!WARNING]
> Dependencies (for Building):
>- CMake
>- C++ Compiler (ideally gcc)
>- MinGW compiler (Windows)

-- Unix Systems:
```bash
	$ mkdir build
	$ cd build/
	$ cmake ..
	$ make
```
- Running:
```
	$ ./Cuber
```

-- Windows Systems:
```cmd
	$ cmake -S . -B build -G "MinGW Makefiles"
	$ cmake --build build
```
- Running:
```
	$ Cuber.exe
```
