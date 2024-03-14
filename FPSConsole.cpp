#include <iostream>
#include <Windows.h>

//Dimensions for screen
int screenWidth = 120;
int screenHeight = 40;
using namespace std;

int main()
{
	//Boolean for manipulating game cycle
	boolean isPaused = false;

	
	//create screen buffer
	wchar_t* screen = new wchar_t[screenWidth * screenHeight]; //create array of type wchar_t for unicode use
	//create a console buffer of return type handle, with permissions to read and write
	HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console); //activates the created buffer
	DWORD dwBytesWritten = 0; //set bytes written to 0

	//Game Cycke
	while (isPaused == false)
	{

	}

	screen[screenWidth * screenHeight - 1] = '\0'; // last element of array is set to escape so we can know to stop
	//Allows for writing to screen
	WriteConsoleOutputCharacter(console, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);


	return 0;
}