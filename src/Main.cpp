//This File will server as the main hub of the program
#include "../header/FPSConsole.h"
#include "../header/Levels.h"
int levelChoice = 1;

//works as intended
int main()
{
    play(pickLevel(levelChoice));
    return 0;
}