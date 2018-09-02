/***********************************************************

OWNER: 	ANUJ BANSAL
ROLLNO. 2018201096
COPYRIGHT PROTECTED
***********************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <sys/syscall.h>
#include"listDir.h"
#include"utility.h"
#include"commandModeOperations.h"
#include<iostream>
#include <sys/ioctl.h>
using namespace std;
extern vector<string> Flist;//file names vector declared in listDir.cpp
extern int vmin;//for handling Esc key and arrow key press differentiation
extern int vtime;
struct winsize w1;		
unsigned long windLine1;//no of rows in terminal 43 in fullscreen
unsigned long lastIndex;//position of last displayed file
unsigned long cursorPos=1;//added to remember the position of cursor when coming back to normal mode from command mode
bool isCommand=false;//in commandmode or not
unsigned long totalfiles,currLine=1,temp;

void resetCursor(unsigned long &currLine);
void scrollDown( unsigned long currLine,unsigned long totalfiles);
void scrollUp(unsigned long currLine,unsigned long totalfiles);
void normalMode();
void commandMode();

enum CommandState isCommandSuccess;//status of executed command in command mode

long n=0;//index for input buffer used in command mode
char inputBuffer[MAX_BUFFER];//input buffer used in command mode

int main(){	
	ioctl(0, TIOCGWINSZ, &w1);//getting terminal dimensions
	windLine1=w1.ws_row-1;
	lastIndex=windLine1;
	clearConsole();
	totalfiles=initialLS(); //this is loading the files from current root
    //printf("Press Q to quit.\n");
    cursorMove(1,1);
    while (true) {
		/////////////////////////////Normal Mode////////////////////////////////
		if(!isCommand){
		    normalMode();
		}
		/////////////////////////////Command Mode////////////////////////////
		else{
			commandMode();
		}	
	    
    }
	return 0;
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Reset the indexing related parameters and move the cursor to first line
*/
void resetCursor(unsigned long &currLine){
	currLine=1;
	cursorMove(1,1);
	cursorPos=1;
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Handle the scroll down event on pressing down arrow key
PARAMETERS:     current line of cursor, total number of files in opened directory
*/
void scrollDown( unsigned long currLine,unsigned long totalfiles){
	if(currLine==lastIndex){
		lastIndex++;
		printFilesWinDependent(currLine-windLine1+1,currLine,"");
		cursorMove(windLine1-1,1);
		cursorPos=windLine1-1;
	}
	else{
		cursorDown(1);
		cursorPos++;
	}
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Handle the scroll down event on pressing up arrow key
PARAMETERS:     current line of cursor, total number of files in opened directory
*/
void scrollUp(unsigned long currLine,unsigned long totalfiles){
	if(currLine<lastIndex-windLine1+1){
		lastIndex--;
		printFilesWinDependent(currLine-1,currLine+windLine1-2,"");
		cursorMove(1,1);
		cursorPos=1;
	}
	else{
		  cursorUp(1);
		  cursorPos--;
	}	
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Normal mode operations function, handles all key pressed events and 
				respective function calls from various modules.ie
				UP,DOWN,LEFT,RIGHT,Enter,Backspace,H/h,:,Q/q key events
				UP=> move cursor up/scroll up
				DOWN=> move cursor down/scroll down
				LEFT=> Backward history of traversed directory
				RIGHT=>Forward history of traversed directory
				ENTER=>Enter a directory/open a file
				BACKSPACE=>Back to parent directory
				H/h->Home functionality , goto root folder(application root)
				:->Switch to command mode
				Q/q: Quit from file explorer(will work in only normal mode)
*/
void normalMode(){
	int c;
	c=getch();
	if (c == K_ESC) { // if the first value is esc				
		 // skip the [ character
	    getch();
		    switch(getch()) { // the real value
		        case K_UP:
		            if(currLine<=1) break;
		            currLine--;
		            scrollUp(currLine,totalfiles);
		            break;
		        case K_DOWN:
		        	if(currLine>=totalfiles) break;
		            currLine++;
		            scrollDown(currLine,totalfiles);
		            break;
		        case K_RIGHT:
		            temp=forwardDirect();//forward history loading
		            //if stack is empty temp=0 then dont change the configuration
		            if(temp!=0){
		            	lastIndex=windLine1;
		            	totalfiles=temp;
		            	resetCursor(currLine);
		            }
		            break;
		        case K_LEFT:
		        	//if stack is empty temp=0 dont change the configuration
					temp=backDirect();
		        	if(temp!=0){
		            	lastIndex=windLine1;
		            	totalfiles=temp;
		            	resetCursor(currLine);
		            }
		            break;
		    }

	}
	//ENTER Key Event
	else if(c==K_ENTER){
		long returnValue=enterDirectory(currLine);
		if(returnValue!=-1){
			lastIndex=windLine1;
			resetCursor(currLine);
			totalfiles=returnValue;
		}
	}
	//BACKSPACE Key Event
	else if(c==K_BACKSPACE){
		totalfiles=backspace();
        resetCursor(currLine);
	}
	//Home key Key Event
	else if(c==K_H || c==K_h ){
		totalfiles=goHome();
        resetCursor(currLine);
	}
	//: Key Event
	else if( c==K_COLON){
		isCommand=true;//isCommand flag set (switched to command mode)
		printCommandMode();
	}
	//Quit Key Event
	else if ( c == K_Q || c == K_q){
    	cursorMove(totalfiles+1,1);
        exit(1);
    }
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Command mode operations function, handles all key pressed events and 
				respective function calls from various modules.ie
				ESC=> Switch back to normal mode
				ENTER=>Execute the enetered command
				BACKSPACE=>Back to parent directory
				READ input to input buffer
*/
void commandMode(){
	int c=getch();
	vmin=0;
	vtime=0;
	int c2=getch();
	vmin=1;
	vtime=0;
	//if c2 is [ than escape it out
	if(c2==91){
		getch();
	}
	//Esc Key Event
	if(c==K_ESC&&c2==-1){
		clearLine();
    	isCommand=!isCommand;
    	//moving cursor to position it was there in normal mode
    	cursorMove(cursorPos,1);
    	n=0;//empty the input buffer
    	isCommandSuccess=SUCCESS_GOTO;
    }
    //BACKSPACE Key Event
	else if(c==K_BACKSPACE){
		if(n!=0){
			inputBuffer[n-1]='\0';//clear last read character from buffer
			n--;
			printCommandMode();
			printInputBuffer(inputBuffer,n);
		}

	}
	//ENTER Key Event
	else if(c==K_ENTER){
		printCommandMode();
		isCommandSuccess =operateCommands(inputBuffer,n);
		if(isCommandSuccess==SUCCESS_GOTO||isCommandSuccess==SUCCESS_DIR_CREATED||isCommandSuccess==SUCCESS_DIR_DELETED||isCommandSuccess==SUCCESS_FILE_DELETED||isCommandSuccess==SUCCESS_COPY||isCommandSuccess==SUCCESS_MOVE ||isCommandSuccess==SUCCESS_RENAME || isCommandSuccess==SUCCESS_SNAPSHOT || isCommandSuccess==SUCCESS_SEARCH){
			totalfiles=Flist.size();
			resetCursor(currLine);
			printCommandMode();

		}
		memset(inputBuffer,'\0',sizeof(inputBuffer));
		n=0;
	}
	//READ the other characters and store them in input buffer
    else{
    	//blank enter or blank spaces
    	if(isCommandSuccess==FAILURE){
    		printCommandMode();
    		isCommandSuccess=SUCCESS_GOTO;
    	}
    	if(c2==-1){
	    	printf("%c",c);
	    	inputBuffer[n++]=c;
    	}
	    
    }	
}



