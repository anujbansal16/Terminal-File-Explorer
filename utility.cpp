/***********************************************************

OWNER:  ANUJ BANSAL
ROLLNO. 2018201096
COPYRIGHT PROTECTED
***********************************************************/
#include"utility.h"

int getch() {
      int c=0;
      
      struct termios org_opts, new_opts;
      int res=0;
          //-----  store old settings -----------
      res=tcgetattr(STDIN_FILENO, &org_opts);
      assert(res==0);
          //---- set new terminal parms --------
      memcpy(&new_opts, &org_opts, sizeof(new_opts));
      new_opts.c_lflag &=~(IXON | IEXTEN);
      new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL | ISIG);
      tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
      c=getchar();
          //------  restore old settings ---------
      res=tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
      assert(res==0);
      return(c);
 }

void cursorAtEnd(){
  //Clear the screen, move to (0,0):
   printf("\033[2J");
}
void cursorUp(int lines){
  //- Move the cursor up 'lines' lines:
   printf("\033[%dA",lines);
}
void cursorDown(int lines){
  //- Move the cursor down 'lines' lines:
   printf("\033[%dB",lines);
}
void cursorForward(int columns){
  //- Move the cursor forward 'columns' columns:
   printf("\033[%dC",columns);
}
void cursorBackward(int columns){
  //- Move the cursor backward 'columns' columns:
   printf("\033[%dD",columns);
}
void clearConsole(){
  //Clear the screen
   printf("\033c");
}
void cursorMove(int line, int column){
  //Position the Cursor:
   printf("\033[%d;%dH",line,column);
}

  


