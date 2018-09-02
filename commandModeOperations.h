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

enum CommandState execute(vector<string> words);//execute the command and return its status
enum CommandState operateCommands(char inputBuffer[],long n);//perform tokenization and call execute
enum CommandState gotoDirectory(string directory);//handle goto directory command
enum CommandState createDirectory(string dirName,string path);//handle creation of directory
enum CommandState createFile(string fileName,string path);//handle creation of file
enum CommandState deleteDir(string path, bool insideCode);//delete a empty directory
enum CommandState deleteFile(string path,bool insideCode);//delete a file
enum CommandState copyOrMove(vector<string> words,string opcode);//handle copy/move command
enum CommandState copyFilesRecursively(vector<string> filenames, string destination);//copy files recursively
void copyDir(string sourcePath,string destination);//copy directory recursively
void copyFile(string filePath,string destination);//copy a file
bool isDirectory(string path);//check if a file is directory
enum CommandState deleteFilesRecursively(vector<string> filenames,bool insideCode);//delete files recursively
void deleteDirRecursively(string filePath, bool insideCode);//delete directory recursively
enum CommandState renameF(string source,string destination);//rename a file
void snapRec(string path, string destination, string searchFile);//snapshot command
enum CommandState snapShotF(string path, string destination);//snapshot handler
enum CommandState search(string searchFile);//search command handler
void checkAndAddResult(string path,string fileName,string searchFile);//store the search results
enum CommandState printSearchResults(vector<string> searchResults, string searchFileName);//print the search results

#endif
