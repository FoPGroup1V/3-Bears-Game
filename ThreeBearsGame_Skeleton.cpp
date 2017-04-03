//---------------------------------------------------------------------------
//Program: Skeleton for Task 1c – group assignment
//Author: Pascale Vacher
//Last updated: 24 February 2017
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//----- include libraries
//---------------------------------------------------------------------------

//include standard libraries
#include <iostream>	
#include <iomanip> 
#include <conio.h> 
#include <cassert> 
#include <string>
#include <sstream>
#include <stdlib.h>
#include <ctime>
#include <stdio.h>
using namespace std;

//include our own libraries
#include "ConsoleUtils.h"	//for Clrscr, Gotoxy, etc.

//---------------------------------------------------------------------------
//----- define constants
//---------------------------------------------------------------------------

//defining the size of the grid
const int  SIZEX(16);    	//horizontal dimension
const int  SIZEY(11);		//vertical dimension
//defining symbols used for display of the grid and content
const char BEAR('@');   	//bear
const char TUNNEL(' ');    	//tunnel
const char WALL('#');    	//border
const char BOMB('0');		//Bomb
const char TRIGGER('T');	//Trigger
const char EXIT('X');		//Exit
//defining the command letters to move the bear on the maze
const int  UP(72);			//up arrow
const int  DOWN(80); 		//down arrow
const int  RIGHT(77);		//right arrow
const int  LEFT(75);		//left arrow
//defining the other command letters
const char QUIT('Q');		//to end the game

struct Item {
	int x, y;
	char symbol;
};

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

int main()
{
	string playerName;

	void mainMenu(string&);

	mainMenu(playerName);


	//function declarations (prototypes)
	void initialiseGame(char g[SIZEY][SIZEX], char m[SIZEY][SIZEX], Item& bear);
	void paintGame(const char g[SIZEY][SIZEX], string mess, string& playerName, int actualScore);
	bool wantsToQuit(const int key);
	bool isArrowKey(const int k);
	int  getKeyPress();
	void updateGameData(const char g[SIZEY][SIZEX], Item& bear, const int key, string& mess);
	void updateGrid(char g[SIZEY][SIZEX], const char m[SIZEY][SIZEX], const Item bear);
	void score();
	void endProgram();

	//local variable declarations 
	int actualScore;
	char grid[SIZEY][SIZEX];	//grid for display
	char maze[SIZEY][SIZEX];	//structure of the maze
	Item bear = { 0, 0, BEAR }; 			//bear's position and symbol
	string message("LET'S START...");	//current message to player

	//action...
	initialiseGame(grid, maze, bear);	//initialise grid (incl. walls & bear)
	paintGame(grid, message, playerName, actualScore);			//display game info, modified grid & messages
	int key(getKeyPress()); 			//read in  selected key: arrow or letter command
	while (!wantsToQuit(key))			//while user does not want to quit
	{
		if (isArrowKey(key))
		{
			updateGameData(grid, bear, key, message);		//move bear in that direction
			updateGrid(grid, maze, bear);			//update grid information
		}
		else
		{
			message = "INVALID KEY!";	//set 'Invalid key' message
			paintGame(grid, message, playerName, actualScore);		//display game info, modified grid & messages
			key = getKeyPress(); 			//display menu & read in next option
		}
	}

	endProgram();						//display final message
	return 0;
}
void mainMenu(string& playerName)
{
	void showMessage(WORD backColour, WORD textColour, int x, int y, const string message);
	showMessage(clRed, clYellow, 40, 10, "3 Bears Project 2016-17");
	showMessage(clRed, clYellow, 40, 11, "Team DAB 2.0");
	showMessage(clBlue, clWhite, 40, 12, "Player Name: ");
	cin >> playerName;
		SelectBackColour(clBlack);
	Clrscr();
	
}

void score()
{

}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void initialiseGame(char grid[SIZEY][SIZEX], char maze[SIZEY][SIZEX], Item& bear)
{ //initialise grid & place bear in middle
	void setInitialMazeStructure(char maze[SIZEY][SIZEX]);
	void setInitialDataFromMaze(char maze[SIZEY][SIZEX], Item& b);
	void updateGrid(char g[SIZEY][SIZEX], const char m[SIZEY][SIZEX], Item b);

	setInitialMazeStructure(maze);		//initialise maze
	setInitialDataFromMaze(maze, bear);	//initialise bear's position
	updateGrid(grid, maze, bear);		//prepare grid
}

void setInitialMazeStructure(char maze[SIZEY][SIZEX])
{ //set the position of the walls in the maze
	//initialise maze configuration
	int initialMaze[SIZEY][SIZEX] 	//local array to store the maze structure
		= { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 2, 3, 0, 3, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 1 },
		{ 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 1, 0, 1, 5, 0, 0, 0, 3, 0, 1, 0, 1, 0, 1 },
		{ 1, 2, 1, 0, 1, 3, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1 },
		{ 1, 2, 1, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
		{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } };
	// with 1 for wall, 0 for tunnel, etc. 
	//copy into maze structure
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			switch (initialMaze[row][col])
		{
			case 0: maze[row][col] = TUNNEL; break;
			case 1: maze[row][col] = WALL; break;
			case 2: maze[row][col] = BEAR; break;
			case 3: maze[row][col] = BOMB; break;
			case 4: maze[row][col] = TRIGGER; break;
			case 5: maze[row][col] = EXIT; break;
		}
}
void setInitialDataFromMaze(char maze[SIZEY][SIZEX], Item& bear)
{ //extract bear's coordinates from initial maze info
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			switch (maze[row][col])
		{
			case BEAR:
			{
				bear.x = col;
				bear.y = row;
				maze[row][col] = TUNNEL;
			}
			break;
			//will work for other items too
		}
}

//---------------------------------------------------------------------------
//----- update grid state
//---------------------------------------------------------------------------

void updateGrid(char grid[SIZEY][SIZEX], const char maze[SIZEY][SIZEX], const Item bear)
{ //update grid configuration after each move
	void setMaze(char g[SIZEY][SIZEX], const char b[SIZEY][SIZEX]);
	void placeBear(char g[SIZEY][SIZEX], const Item bear);

	setMaze(grid, maze);	//reset the empty maze configuration into grid
	placeBear(grid, bear);	//set bear in grid
}

void setMaze(char grid[SIZEY][SIZEX], const char maze[SIZEY][SIZEX])
{ //reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];
}

void placeBear(char g[SIZEY][SIZEX], const Item bear)
{ //place bear at its new position in grid
	g[bear.y][bear.x] = bear.symbol;
}

//---------------------------------------------------------------------------
//----- move the bear
//---------------------------------------------------------------------------
void updateGameData(const char g[SIZEY][SIZEX], Item& bear, const int key, string& mess)
{ //move bear in required direction
	bool isArrowKey(const int k);
	void setKeyDirection(int k, int& dx, int& dy);
	assert(isArrowKey(key));

	//reset message to blank
	mess = "                                         ";		//reset message to blank

	//calculate direction of movement for given key
	int dx(0), dy(0);
	setKeyDirection(key, dx, dy);

	//check new target position in grid and update game data (incl. bear coordinates) if move is possible
	switch (g[bear.y + dy][bear.x + dx])
	{			//...depending on what's on the target position in grid...
	case TUNNEL:		//can move
		bear.y += dy;	//go in that Y direction
		bear.x += dx;	//go in that X direction
		break;
	case WALL:  		//hit a wall and stay there
		cout << '\a';	//beep the alarm
		mess = "CANNOT GO THERE!";
		break;
	}
}
//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------
void setKeyDirection(const int key, int& dx, int& dy)
{ //calculate direction indicated by key
	bool isArrowKey(const int k);
	int i(0);
	assert(isArrowKey(key));
	switch (key)	//...depending on the selected key...
	{
	case LEFT:  	//when LEFT arrow pressed...
		dx = -1;	//decrease the X coordinate
		dy = 0;
		break;
	case RIGHT: 	//when RIGHT arrow pressed...
		dx = +1;	//increase the X coordinate
		dy = 0;
		break;
	case UP: 		//when UP arrow pressed...
		dx = 0;
		dy = -1;	//decrease the Y coordinate
		break;
	case DOWN: 		//when DOWN arrow pressed...
		dx = 0;
		dy = +1;	//increase the Y coordinate
		break;
	}
}

int getKeyPress()
{ //get key or command (in uppercase) selected by user
	//KEEP THIS FUNCTION AS GIVEN
	int keyPressed;
	keyPressed = _getch();			//read in the selected arrow key or command letter
	while (keyPressed == 224) 		//ignore symbol following cursor key
		keyPressed = _getch();
	return toupper(keyPressed);		//return it in uppercase 
}

bool isArrowKey(const int key)
{	//check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')
	return (key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN);
}
bool wantsToQuit(const int key)
{	//check if the user wants to quit (when key is 'Q' or 'q')
	return toupper(key) == QUIT;
}

//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------

string tostring(char x) {
	std::ostringstream os;
	os << x;
	return os.str();
}
void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message)
{
	Gotoxy(x, y);
	SelectBackColour(backColour);
	SelectTextColour(textColour);
	cout << message;
}
void paintGame(const char g[SIZEY][SIZEX], string mess, string& playerName, int actualScore)
{ //display game title, messages, maze, bear and other items on screen
	string tostring(char x);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	void paintGrid(const char g[SIZEY][SIZEX]);

	//display Player Name
	showMessage(clWhite, clRed, 40, 0, playerName);

	//display game title
	showMessage(clBlack, clYellow, 0, 0, "___GAME___");
	showMessage(clWhite, clRed, 40, 3, "FoP Task 1c: February 2017");

	//display menu options available
	showMessage(clRed, clYellow, 40, 6, "TO MOVE USE KEYBOARD ARROWS ");
	showMessage(clRed, clYellow, 40, 7, "TO QUIT ENTER 'Q'           ");

	cout << endl << actualScore;

	//print auxiliary messages if any
	showMessage(clBlack, clWhite, 40, 11, mess);	//display current message

	// display grid contents
	paintGrid(g);
}

void paintGrid(const char g[SIZEY][SIZEX])
{ //display grid content on screen
	SelectBackColour(clBlack);
	SelectTextColour(clWhite);
	Gotoxy(0, 2);
	for (int row(0); row < SIZEY; ++row)
	{
		for (int col(0); col < SIZEX; ++col)
			cout << g[row][col];	//output cell content
		cout << endl;
	}
}

void endProgram()
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	showMessage(clRed, clYellow, 40, 9, "");	//hold output screen until a keyboard key is hit
	system("pause");
}
