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
#include<iostream>
#include <sys/ioctl.h>
using namespace std;
//file names vector declared in listDir.cpp
extern vector<string> Flist;
struct winsize w1;		
//43 in full size terminal
unsigned long windLine1;
unsigned long lastIndex;
//added to remember the position of cursor when coming back to normal mode from command mode
unsigned long cursorPos=1;
bool isCommand=false;
unsigned long totalfiles,currLine=1,temp;
void resetCursor(unsigned long &currLine);
void scrollDown( unsigned long currLine,unsigned long totalfiles);
void scrollUp(unsigned long currLine,unsigned long totalfiles);
void normalMode();
void commandMode();

int main(){	
	ioctl(0, TIOCGWINSZ, &w1);
	windLine1=w1.ws_row;
	lastIndex=windLine1;
	//printf ("lines %d\n", w.ws_row);
	clearConsole();
	totalfiles=initialLS(); //this is loading the directory from current root
    printf("Press Q to quit.\n");
    cursorMove(1,1);
    while (true) {
		/////////////////////////////Normal Mode////////////////////////////////
		if(!isCommand){
		    normalMode();
		}
	///////////////////////////Normal Mode End////////////////////////////
		else{
			commandMode();
		}	
	    
    }
    //printf("Done.\n");
	return 0;
}

void resetCursor(unsigned long &currLine){
	currLine=1;
	cursorMove(1,1);
	cursorPos=1;

}
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

void normalMode(){
	int c;
	c=getch();
	if (c == K_ESC) { // if the first value is esc				
		 // skip the [
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
		            temp=forwardDirect();
		            //if stack is empty temp=0 then dont change the configuration
		            if(temp!=0){
		            	lastIndex=windLine1;
		            	totalfiles=temp;
		            	// currLine=1;
		            	// cursorMove(1,1);	
		            	// cursorPos=1;
		            	resetCursor(currLine);
		            }
		            
		            break;
		        case K_LEFT:
		        	//if stack is empty temp=0 dont change the configuration
					temp=backDirect();
		        	if(temp!=0){
		            	lastIndex=windLine1;
		            	totalfiles=temp;
		            	// currLine=1;
		            	// cursorMove(1,1);	
		            	// cursorPos=1;
		            	resetCursor(currLine);
		            }
		            break;
		    }

	}
	//enter
	else if(c==K_ENTER){
		//cout<<" "<<currLine;
		long returnValue=enterDirectory(currLine);
		if(returnValue!=-1){
			lastIndex=windLine1;
			// cursorMove(1,1);	
			// cursorPos=1;
			// currLine=1;	
			resetCursor(currLine);
			totalfiles=returnValue;
		}
	}
	//backspace
	else if(c==K_BACKSPACE){
		totalfiles=backspace();
        currLine=1;	
        cursorMove(1,1);
	}
	//h
	else if(c==K_H || c==K_h ){
		totalfiles=goHome();
        currLine=1;	
        cursorMove(1,1);
	}
	//:
	else if( c==K_COLON){
		isCommand=true;
		//cursorMove(totalfiles+2,1);
		cursorMove(1000,1);
		printf("COMMAND MODE:");
	}
	//q
	else if ( c == K_Q || c == K_q){
    	cursorMove(totalfiles+1,1);
        exit(1);
    }
}
void commandMode(){
	int c=getch();
	if(c==K_ESC){
		clearLine();
    	isCommand=!isCommand;
    	//currLine=1;
    	//moving cursor to position it was there in normal mode
    	cursorMove(cursorPos,1);
    }
    //backspace
	else if(c==K_BACKSPACE){
		
	}
	//enter
	else if(c==K_ENTER){
		clearLine();
    	isCommand=!isCommand;
    	//moving cursor to position it was there in normal mode
    	cursorMove(cursorPos,1);
	}

    else{
	        	printf("%c",c );
    }	
}



