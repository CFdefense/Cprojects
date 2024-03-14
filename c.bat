REM Compiling Folder
ECHO Compiling...
REM Compiles the three c++ files into game.exe
CALL g++ -o dist/game.exe src/main.cpp src/FPSConsole.cpp src/Levels.cpp