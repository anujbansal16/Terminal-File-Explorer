/***********************************************************

OWNER: 	ANUJ BANSAL
ROLLNO. 2018201096
COPYRIGHT PROTECTED
***********************************************************/
#include<bits/stdc++.h>
#include <termios.h>
#include <signal.h>
#include <termios.h>
#include <signal.h>
#define ESC '\033'
#define K_UP 'A'
#define K_DOWN 'B'
#define K_RIGHT 'C'
#define K_LEFT 'D'

#ifndef INC_UTILITY_H
#define INC_UTILITY_H

int getch();
void cursorAtEnd();
void cursorUp(int lines);
void cursorDown(int lines);
void cursorForward(int columns);
void cursorBackward(int columns);
void clearConsole();
void cursorMove(int line, int column);
void clearLine();
#endif 

