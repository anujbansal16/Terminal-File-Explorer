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

extern vector<string> Flist;
struct winsize w1;		
unsigned long windLine1;
unsigned long lastIndex;

void scrollDown( unsigned long currLine,unsigned long totalfiles){
	if(currLine==lastIndex){
		lastIndex++;
		printFilesWinDependent(currLine-windLine1+1,currLine,"");
		cursorMove(windLine1-1,1);
	}
	else
		cursorDown(1);
}
void scrollUp(unsigned long currLine,unsigned long totalfiles){
	if(currLine<lastIndex-windLine1+1){
		lastIndex--;
		printFilesWinDependent(currLine-1,currLine+windLine1-2,"");
		cursorMove(1,1);
	}
	else
		  cursorUp(1);
}

int main(){	
	int c;
    //struct winsize w;	
	ioctl(0, TIOCGWINSZ, &w1);
	windLine1=w1.ws_row;
	lastIndex=windLine1;
	//printf ("lines %d\n", w.ws_row);
	bool isCommand=false;
	unsigned long totalfiles,currLine=1,temp;
	clearConsole();
	totalfiles=initialLS(); //this is loading the directory from current root
    printf("Press Q to quit.\n");
    cursorMove(1,1);
    while (true) {
    	c=getch();
		/////////////////////////////Normal Mode////////////////////////////////
		if(!isCommand){
			if (c == ESC) { // if the first value is esc				
				 // skip the [
			    getch();
				    switch(getch()) { // the real value
				        case 'A':
				            if(currLine<=1) continue;
				            currLine--;
				            scrollUp(currLine,totalfiles);
				            break;
				        case 'B':
				        	if(currLine>=totalfiles) continue;
				            currLine++;
				            scrollDown(currLine,totalfiles);
				            break;
				        case 'C':
				            temp=forwardDirect();
				            //if stack is empty temp=0 then dont change the configuration
				            if(temp!=0){
				            	lastIndex=windLine1;
				            	totalfiles=temp;
				            	currLine=1;
				            	cursorMove(1,1);	
				            }
				            
				            break;
				        case 'D':
				        	//if stack is empty temp=0 dont change the configuration
							temp=backDirect();
				        	if(temp!=0){
				            	lastIndex=windLine1;
				            	totalfiles=temp;
				            	currLine=1;
				            	cursorMove(1,1);	
				            }
				            break;
				    }

			}
			//enter
			else if(c==10){
				//cout<<" "<<currLine;
				long returnValue=enterDirectory(currLine);
				if(returnValue!=-1){
					lastIndex=windLine1;
					cursorMove(1,1);	
					currLine=1;	
					totalfiles=returnValue;
				}
			}
			//backspace
			else if(c==127){
				totalfiles=backspace();
	            currLine=1;	
	            cursorMove(1,1);
			}
			//h
			else if(c==104){
				totalfiles=goHome();
	            currLine=1;	
	            cursorMove(1,1);
			}
			else if( c==':'){
				isCommand=true;
				//cursorMove(totalfiles+2,1);
				cursorMove(1000,1);
			}
			else if ( c == 'Q' || c == 'q'){
		    	cursorMove(totalfiles+1,1);
		        break;
		    }
		    
		    //cout<<totalfiles;
		}
	///////////////////////////Normal Mode End////////////////////////////
		else{
			//char c2=getchar();
			if(c==ESC){
				clearLine();
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




