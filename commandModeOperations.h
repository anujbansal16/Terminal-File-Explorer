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
#include <fcntl.h>
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
enum CommandState deleteDir(string path, bool insideCode);
enum CommandState deleteFile(string path,bool insideCode);
enum CommandState copyOrMove(vector<string> words,string opcode);
enum CommandState copyFilesRecursively(vector<string> filenames, string destination);
void copyDir(string sourcePath,string destination);
void copyFile(string filePath,string destination);
bool isDirectory(string path);
enum CommandState deleteFilesRecursively(vector<string> filenames,bool insideCode);
void deleteDirRecursively(string filePath, bool insideCode);
enum CommandState renameF(string source,string destination);
void snapRec(string path, string destination, string searchFile);
enum CommandState snapShotF(string path, string destination);
enum CommandState search(string searchFile);
void checkAndAddResult(string path,string fileName,string searchFile);
enum CommandState search(string searchFile);
enum CommandState printSearchResults(vector<string> searchResults, string searchFileName);
#endif
