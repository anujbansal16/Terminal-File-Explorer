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
//file names vector declared in listDir.cpp
extern vector<string> Flist;
extern int vmin;
extern int vtime;
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

enum CommandState isCommandSuccess;
long n=0;
char inputBuffer[MAX_BUFFER];

int main(){	
	ioctl(0, TIOCGWINSZ, &w1);
	windLine1=w1.ws_row-1;
	lastIndex=windLine1;
	//printf ("lines %d\n", w.ws_row);
	clearConsole();
	totalfiles=initialLS(); //this is loading the directory from current root
    //printf("Press Q to quit.\n");
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
        //currLine=1;	
        //cursorMove(1,1);
        resetCursor(currLine);
	}
	//h
	else if(c==K_H || c==K_h ){
		totalfiles=goHome();
        //currLine=1;	
        //cursorMove(1,1);
        resetCursor(currLine);
	}
	//:
	else if( c==K_COLON){
		isCommand=true;
		//cursorMove(totalfiles+2,1);
		//cursorMove(1000,1);
		printCommandMode();
	}
	//q
	else if ( c == K_Q || c == K_q){
    	cursorMove(totalfiles+1,1);
        exit(1);
    }
}
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
	if(c==K_ESC&&c2==-1){
		clearLine();
    	isCommand=!isCommand;
    	//currLine=1;
    	//moving cursor to position it was there in normal mode
    	cursorMove(cursorPos,1);
    	n=0;
    	isCommandSuccess=SUCCESS_GOTO;
    }
    //backspace
	else if(c==K_BACKSPACE){
		if(n!=0){
			inputBuffer[n]='\0';
			n--;
			printCommandMode();
			printInputBuffer(inputBuffer,n);
		}

	}
	//enter
	else if(c==K_ENTER){
		printCommandMode();
		isCommandSuccess =operateCommands(inputBuffer,n);
		if(isCommandSuccess==SUCCESS_GOTO||isCommandSuccess==SUCCESS_DIR_CREATED||isCommandSuccess==SUCCESS_DIR_DELETED||isCommandSuccess==SUCCESS_FILE_DELETED||isCommandSuccess==SUCCESS_COPY||isCommandSuccess==SUCCESS_MOVE ||isCommandSuccess==SUCCESS_RENAME){
			totalfiles=Flist.size();
			resetCursor(currLine);
			printCommandMode();

		}
		memset(inputBuffer,'\0',sizeof(inputBuffer));
		n=0;
	}
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



