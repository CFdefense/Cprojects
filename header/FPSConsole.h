#pragma once
#include <string>

extern int screenWidth;
extern int screenHeight;
extern float characterXpos;
extern float characterYpos;
extern float characterAngle;
extern float characterFOV;
extern float depth;
extern int mapWidth;
extern int mapHeight;
extern bool isPaused;
extern bool inMenu;

int play(std::wstring map);