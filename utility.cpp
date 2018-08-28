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
void clearLine(){
  //clear current line
  printf("%c[2K", 27);
}
void printHumanReadableSize(long size)
{
  long Kb = 1  * 1024;
  long Mb = Kb * 1024;
  long Gb = Mb * 1024;
  long Tb = Gb * 1024;
  long Pb = Tb * 1024;
  long Eb = Pb * 1024;
  if (size <  Kb)                 
    printf("  %7.2fb",1.0*size);
  if (size >= Kb && size < Mb)
    printf("  %7.2fKb",1.0*size/Kb);
  if (size >= Mb && size < Gb)
    printf("  %7.2fMb",1.0*size/Mb);
  if (size >= Gb && size < Tb)
    printf("  %7.2fGb",1.0*size/Gb);
  if (size >= Tb && size < Pb)
    printf("  %7.2fTb",1.0*size/Tb);
  if (size >= Pb && size < Eb)
    printf("  %7.2fPb",1.0*size/Pb);  
}
void printInputBuffer(char inputBuffer[], long n){
  for (long i=0;i<n;i++)
      printf("%c", inputBuffer[i]);
}
void printCommandMode(){
  cursorMove(1000,1);
  clearLine();    
  printf("COMMAND MODE :");
}
vector<string> tokenize(char inputBuffer[], string token){
  vector<string> words;
    char* word = strtok(inputBuffer, token.c_str());
    while (word != NULL) {
        words.push_back(word);
        word = strtok(NULL, token.c_str());
    }
    return words;
}
  


