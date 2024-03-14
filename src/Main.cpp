//This File will server as the main hub of the program
#include "FPSConsole.cpp"
#include "Levels.cpp"
int levelChoice = 0;
int main()
{
    int result = play(pickLevel(levelChoice));
    return result;
}


