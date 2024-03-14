#include <iostream>
#include <Windows.h>
#include <cmath>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;

//! Initialized Variables
//Dimensions for screen
int screenWidth = 120;
int screenHeight = 40;
//character location and angle looking
float characterXpos = 8.0f;
float characterYpos = 8.0f;
float characterAngle = 0.0f;
float characterFOV = 3.14159f / 4;
float depth = 16.0f;
//minimap dimensions
int mapWidth = 16;
int mapHeight = 16;
//Boolean for manipulating game cycle
bool isPaused = false;


int main()
{

	//create screen buffer
	wchar_t* screen = new wchar_t[screenWidth * screenHeight]; //create array of type wchar_t for unicode use
	//create a console buffer of return type handle, with permissions to read and write
	HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console); //activates the created buffer
	DWORD dwBytesWritten = 0; //set bytes written to 0

	//! Initialize and Create Modifiable wide string map
	wstring map;
	map += L"################";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..........#...#";
	map += L"#..........#...#";
	map += L"#..........#...#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#.......########";
	map += L"#..............#";
	map += L"#..............#";
	map += L"################";

		//initialize time points 1 and 2 of type auto-detect
		auto tp1 = chrono::system_clock::now();
		auto tp2 = chrono::system_clock::now();

		//Game Cycle
		while (isPaused == false)
		{
			//count system speed in order to properly time our game fps
			tp2 = chrono::system_clock::now(); //update with current time 
			chrono::duration<float> elapsedTime = tp2-tp1;//deduce elapsed time between game cycles
			tp1 = tp2;
			float fElapsedTime = elapsedTime.count(); //fElapsedTime is now our fps determiner

		//!Controls
		//Rotation - Get key state and modify characterAngle Accordingly
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			characterAngle -= 0.8 * fElapsedTime;
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			characterAngle += 0.8 * fElapsedTime;
		//Movement
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
			{
				characterXpos += sinf(characterAngle) * 5.0f * fElapsedTime;
				characterYpos += cosf(characterAngle) * 5.0f * fElapsedTime;

				//Collision Detection
				if(map[(int)characterYpos * mapWidth + (int)characterXpos] == '#')
				{
					//if collision deteched move character back
					characterXpos -= sinf(characterAngle) * 5.0f * fElapsedTime;
					characterYpos -= cosf(characterAngle) * 5.0f * fElapsedTime;
				}
				
			}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
			{
				characterXpos -= sinf(characterAngle) * 5.0f * fElapsedTime;
				characterYpos -= cosf(characterAngle) * 5.0f * fElapsedTime;

				//Collision Detection
				if(map[(int)characterYpos * mapWidth + (int)characterXpos] == '#')
				{
					//if collision deteched move character forward
					characterXpos += sinf(characterAngle) * 5.0f * fElapsedTime;
					characterYpos += cosf(characterAngle) * 5.0f * fElapsedTime;
				}
				
			}

		//! RAY TRACING AND SHADING
		for (int x = 0; x < screenWidth; x++)
		{
			//For each Column in the console we will caculate the ray angle that the character sees
			float rayAng = ((characterAngle - characterFOV / 2.0f) + ((float)x / (float)screenWidth * characterFOV));

			//check for how close we are to a wall
			float distanceToWall = 0.0f;
			bool hitWall = false;
			bool hitBoundary = false;

			//unit vectors for angle that player is looking at
			float lookingX = sinf(rayAng);
			float lookingY = cosf(rayAng);

			//!Collision Detection
			while (!hitWall && distanceToWall < depth)
			{
				distanceToWall += 0.1f;
				//Create the point we are testing for collision, using int because walls located in cells
				int testX = (int)(characterXpos + lookingX * distanceToWall);
				int testY = (int)(characterYpos + lookingY * distanceToWall);

				//Determine if the ray is out of bounds
				if (testX < 0 || testX >= mapWidth || testY < 0 || testY >= mapHeight)
				{	
					hitWall = true;
					distanceToWall = depth; //set distance to max depth
				}//if

				//Ray is inbounds so test to see if the ray cell is a wall block
				else if (map[testY * mapWidth + testX] == '#')
					{
						// Ray has hit wall
						hitWall = true;

						// To highlight tile boundaries, cast a ray from each corner
						// of the tile, to the player. The more coincident this ray
						// is to the rendering ray, the closer we are to a tile 
						// boundary, which we'll shade to add detail to the walls
						vector<pair<float, float>> p;

						// Test each corner of hit tile, storing the distance from
						// the player, and the calculated dot product of the two rays
						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++)
							{
								// Angle of corner to eye
								float vy = (float)testY + ty - characterYpos;
								float vx = (float)testX + tx - characterXpos;
								float d = sqrt(vx*vx + vy*vy); 
								float dot = (lookingX * vx / d) + (lookingY * vy / d);
								p.push_back(make_pair(d, dot));
							}

						// Sort Pairs from closest to farthest
						sort(p.begin(), p.end(), [](const pair<float, float> &left, const pair<float, float> &right) {return left.first < right.first; });
						
						// First two/three are closest (we will never see all four)
						float fBound = 0.01;
						if (acos(p.at(0).second) < fBound) hitBoundary = true;
						if (acos(p.at(1).second) < fBound) hitBoundary = true;
						//if (acos(p.at(2).second) < fBound) hitBoundary = true;

				}//else
			}//while
			//Calculate the percieved ceiling height depending on distance from wall
			//Calculation- as distance to wall becomes smaller, the ceiling bcomes bigger giving the perception we are closer
			int ceiling = (float)(screenHeight / 2.0f) - screenHeight / ((float)distanceToWall);
			int floor = screenHeight - ceiling;

			//Change Wall Shading Depending on Distance From Wall
			short wallShade = ' ';
			if (distanceToWall <= depth / 4.0f)
				wallShade = 0x2588; //very dark
			else if(distanceToWall < depth / 3.0f)
				wallShade = 0x2593; //dark
			else if(distanceToWall < depth / 2.0f)
				wallShade = 0x2592; //less dark
			else if(distanceToWall < depth)
				wallShade = 0x2591; //light
			else
				wallShade = ' '; //out of render

			if (hitBoundary == true)
				wallShade = ' ';

			//Draw to column
			for (int y = 0; y < screenHeight; y++)
			{
				if (y < ceiling)
					screen[y * screenWidth + x] = ' '; //must be ceiling
				else if (y > ceiling && y <= floor)
					screen[y * screenWidth + x] = wallShade; // must be wall
				else
				{
					
					//Change Floor Shading Depending on Distance to Wall
					short floorShade = ' ';
					float floorSeen = 1.0f - (((float)y - screenHeight / 2.0f) / ((float)screenHeight / 2.0f));
					if (floorSeen < 0.25)
						floorShade = '#';
					else if (floorSeen < 0.5)
						floorShade = 'x';
					else if (floorSeen < .75)
						floorShade = '-';
					else
						floorShade = ' ';
					screen[y * screenWidth + x] = floorShade; // must be floor
				}//else
			}//for

		}//for
		
			//! Display Stats
			swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", characterXpos, characterYpos, characterAngle, 1.0f/fElapsedTime);

			//! Display Map
			for (int nx = 1; nx < mapWidth; nx++)
				for (int ny = 0; ny < mapWidth; ny++)
				{
					screen[(ny+1)*screenWidth + nx] = map[ny * mapWidth + nx];
				}
			screen[((int)characterYpos+1) * screenWidth + (int)characterXpos] = 'P';

			// Display Frame
			screen[screenWidth * screenHeight - 1] = '\0';
			WriteConsoleOutputCharacterW(console, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);

		//!Allows for writing to screen
		screen[screenWidth * screenHeight - 1] = '\0'; // last element of array is set to escape so we can know to stop
		WriteConsoleOutputCharacterW(console, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);

	}//while

	


	return 0;
}