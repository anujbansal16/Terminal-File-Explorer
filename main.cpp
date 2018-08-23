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
using namespace std;


int main(){	
	int c;
	bool isCommand=false;
	unsigned long totalfiles,currLine=1;
	clearConsole();
	totalfiles=initialLS(); //this is loading the directory from current root
    printf("Press Q to quit.\n");
    cursorMove(1,1);
    while (true) {
		c=getch();
		/////////////////////////////Normal Mode////////////////////////////////
		if(!isCommand){
			if (c == ESC) { // if the first value is esc
				//cout<<"Inside ESc";
			    getch(); // skip the [
			    switch(getch()) { // the real value
			        case 'A':
			            if(currLine<=1) continue;
			            cursorUp(1);
			            currLine--;
			            break;
			        case 'B':
			        	if(currLine>=totalfiles) continue;
			            cursorDown(1);
			            currLine++;
			            break;
			        case 'C':
			            cursorForward(1);
			            break;
			        case 'D':
			            cursorBackward(1);
			            break;
			    }
			}

		else if( c==':'){
			isCommand=true;
			cursorMove(totalfiles+2,1);
		}
		else if ( c == 'Q' || c == 'q'){
	    	cursorMove(totalfiles+1,1);
	        break;
	    }
	}
	///////////////////////////Normal Mode End////////////////////////////
	else{
		if(c==ESC){
	    	isCommand=!isCommand;
	    	currLine=1;
	    	cursorMove(1,1);
	    }
	    else{
        	printf("%c",c );
	    }
	}	
	    
    }
 
    //printf("Done.\n");
	return 0;
}



