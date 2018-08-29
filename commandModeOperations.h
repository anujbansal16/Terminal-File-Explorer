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
#include "utility.h"
#include "listDir.h"
using namespace std;

#ifndef INC_COMMANDMODEOPERATIONS_H
#define INC_COMMANDMODEOPERATIONS_H
enum CommandState execute(vector<string> words);
enum CommandState operateCommands(char inputBuffer[],long n);
enum CommandState gotoDirectory(string directory);
enum CommandState createDirectory(string dirName,string path);
enum CommandState createFile(string fileName,string path);
#endif
