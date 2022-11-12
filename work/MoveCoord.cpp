#include "MoveCoord.h"
void gotoxy(short x, short y)
{
	HANDLE    StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD    coord = { x, y };
	SetConsoleCursorPosition(StdOut, coord);
}
short wherex()
{
	HANDLE                        StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO    csbi;
	GetConsoleScreenBufferInfo(StdOut, &csbi);
	return int(csbi.dwCursorPosition.X);
}
short wherey()
{
	HANDLE                        StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO    csbi;
	GetConsoleScreenBufferInfo(StdOut, &csbi);
	return int(csbi.dwCursorPosition.Y);
}
