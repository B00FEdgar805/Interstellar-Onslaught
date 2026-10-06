# Interstellar Onslaught

## Discription
This is a 2d roguelite game I made in sdl3 and c++. It also uses SDL3_ttf and SDL3_mixer for text and audio. The Art was also made by me.

## Features
- Original ECS using a sparse set
- Grid-based Spatial Partitioning collision system
- Text, Texture and Audio managers
- OOP patern and separate game and rendering logic
- TileMap system taking in txt files
- IM::GUI debug functions
- 3 different levels
  
## Screenshot
<img width="3024" height="1742" alt="image" src="https://github.com/user-attachments/assets/8d5182b0-7990-4a55-b505-67f9c381771e" />

## Video
https://github.com/user-attachments/assets/d15b1b73-e64c-409c-8aa8-76e054b04d1b

## Requirements
- C++ 20
- CMake 3.20
- SDL3.0

## How to Run
MacOS
```
rm -rf build
cmake -S . -B build
cmake --build build --config Release
./build/GameTestSDL3.app/Contents/MacOS/GameTestSDL3
```
Windows
```
cmake -S . -B build
cmake --build build --config Release
build/Release/GameTestSDL3.exe
```
