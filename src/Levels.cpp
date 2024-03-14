//This File will Contain all the levels for the game
#include <string>
using namespace std;

wstring pickLevel(int levelNumber)
{
    wstring map;
    switch(levelNumber)
    {
        case 1: 
            //! Initialize and Create Modifiable wide string map
            map += L".################";
            map += L".#..............#";
            map += L".#..............#";
            map += L".#..............#";
            map += L".#..........#...#";
            map += L".#..........#...#";
            map += L".#..........#...#";
            map += L".#..............#";
            map += L".#..............#";
            map += L".#..............#";
            map += L".#.#..........#.#";
            map += L".#..###....###..#";
            map += L".#....#....#....#";
            map += L".#....#....#....#";
            map += L".#....#....#....#";
            map += L"#################";
    }
    return(map);
}