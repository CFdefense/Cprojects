#!/bin/bash

# Compiling Folder
echo "Compiling..."

# Compile the three C++ files into game.exe
x86_64-w64-mingw32-g++ -o dist/game.exe src/main.cpp src/FPSConsole.cpp src/Levels.cpp

