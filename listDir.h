/***********************************************************

OWNER: 	ANUJ BANSAL
ROLLNO. 2018201096
COPYRIGHT PROTECTED
***********************************************************/
#include<bits/stdc++.h>
#include<stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include"utility.h"
using namespace std;

#ifndef INC_MODULE2_H
#define INC_MODULE2_H

unsigned long initialLS();//initial loading the root directory files
struct stat getStatInfo(string rootPath, string fName);//stat info of a file given path and filename
void printHead();//print column names
DIR * openDirectory(const char* s);//open directory given the path
void getFileList(DIR * pDir);//prepare filename array of files present in directory
long enterDirectory(unsigned long indexOfFile);//enter a directory on ENTER press
void printStatInfo(struct stat info, string fName);//iteratively display information of files
void openFile(string filePath);//open a file in the default application
unsigned long backDirect();//go back to previously visited directory
unsigned long forwardDirect();//go to next visited directory
void printFilesWinDependent(unsigned long firstIndex,unsigned long lastIndex,string path);//print number of files depending upon the window size
unsigned long backspace();//go back to parent directory
unsigned long goHome();//go to Home(Root) Directory

#endif
