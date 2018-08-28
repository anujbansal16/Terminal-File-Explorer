/***********************************************************

OWNER:  ANUJ BANSAL
ROLLNO. 2018201096
COPYRIGHT PROTECTED
***********************************************************/
#include"commandModeOperations.h"
extern vector<string> Flist;
extern vector<string> stackBackHistory; // ./|dir1/|dir2|
extern unsigned long windLine;
extern unsigned long tailOmit;
enum CommandState operateCommands(char inputBuffer[],long n){
	//buffer empty
	if(n==0){
		printf("Enter commands first");
		return FAILURE;
	}
	else{
		vector<string> words;
		words=tokenize(inputBuffer," ");
		//spaces
		if(words.empty()){
			printf("Enter commands first");
			return FAILURE;
		}
		return execute(words);		
	}
}

enum CommandState execute(vector<string> words){
	string opcode=words[0];
	if(opcode==GOTO){
		if(words.size()==2)
        	return gotoDirectory(words[1]);
        else
        	cout<<" Too many few arguments for goto";
	}
	else if(opcode==COPY){
	}
	else if(opcode==MOVE){
		
	}
	else if(opcode==RENAME){
		
	}
	return FAILURE;
}


enum CommandState gotoDirectory(string directory){
		if(directory=="./"||directory=="/"){
			initialLS();
			return SUCCESS_GOTO;
		}
		string path=("."+directory+"/");
		struct stat info1;struct stat info2;
    	stat(stackBackHistory.back().c_str(),&info1);stat(path.c_str(),&info2);
    	if(info1.st_dev==info2.st_dev&&info1.st_ino==info2.st_ino){
    		return SUCCESS_GOTO;
    	}
		DIR *pDir;
		pDir = openDirectory(path.c_str());
		if(pDir==NULL)
			return FAILURE;
        getFileList(pDir);
        stackBackHistory.push_back(path);
        printFilesWinDependent(0,windLine-tailOmit,"");
        closedir(pDir);
        return SUCCESS_GOTO;
}
