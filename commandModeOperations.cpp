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
        	cout<<" Too many few arguments for "<<GOTO;
	}

	else if(opcode==CREATE_DIR){
		if(words.size()==3)
        	return createDirectory(words[1],words[2]);
        else
        	cout<<" Too many few arguments for "<<CREATE_DIR;
	}

	else if(opcode==CREATE_FILE){
		if(words.size()==3)
        	return createFile(words[1],words[2]);
        else
        	cout<<" Too many few arguments for "<<CREATE_FILE;
	}

	else if(opcode==DELETE_DIR){
		if(words.size()==2)
        	return deleteDir(words[1]);
        else
        	cout<<" Too many few arguments for "<<DELETE_DIR;
	}

	else if(opcode==DELETE_FILE){
		if(words.size()==2)
        	return deleteFile(words[1]);
        else
        	cout<<" Too many few arguments for "<<DELETE_FILE;
	}
	
	return FAILURE;
}


enum CommandState gotoDirectory(string directory){
		/*if(directory=="./"||directory=="/"){
			initialLS();
			return SUCCESS_GOTO;
		}*/
		if(directory[0]=='.'){
			cout<<"Path must start from / not .";
			return FAILURE;
		}
		string path=("."+directory+"/");
		/*struct stat info1;struct stat info2;
    	stat(stackBackHistory.back().c_str(),&info1);stat(path.c_str(),&info2);
    	if(info1.st_dev==info2.st_dev&&info1.st_ino==info2.st_ino){
    		return SUCCESS_GOTO;
    	}*/
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

enum CommandState createDirectory(string dirName,string path){
    int isNotCreated;
    DIR *pDir;
    //create directory in pwd
    if(path==".")
    	path=stackBackHistory.back()+dirName;
    else//directory in given path
    	path="."+path.substr(1,path.size())+"/"+dirName;
    struct stat info;
    //if directory already exist?
    if (stat(path.c_str(), &info) != -1) {
   		if (S_ISDIR(info.st_mode)) {
  			cout<<" Directory name already exist ";
        	return FAILURE;
   		}
	}
    isNotCreated = mkdir(path.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    //created
    if (!isNotCreated){
		pDir = openDirectory(stackBackHistory.back().c_str());
		//load current directory again
		if(pDir==NULL)
			return FAILURE;
		getFileList(pDir);
		printFilesWinDependent(0,windLine-tailOmit,"");
		closedir(pDir);
		return SUCCESS_DIR_CREATED;
    }
    //not created
    else {
        cout<<" Unable to create directory "<<strerror(errno);
        return FAILURE;
    }
 
}
enum CommandState createFile(string fileName,string path){
    DIR *pDir;
    if(path==".")
    	path=stackBackHistory.back();
    else//file in given path
    	path="."+path.substr(1,path.size())+"/";
    struct stat info;
    //if directory already exist?
    if (stat(path.c_str(), &info) != -1) {
   		if (S_ISDIR(info.st_mode)) {
			fopen ((path+fileName).c_str(),"w+");
			//load current directory again
			pDir = openDirectory(stackBackHistory.back().c_str());
			if(pDir==NULL)
				return FAILURE;
			getFileList(pDir);
			printFilesWinDependent(0,windLine-tailOmit,"");
			closedir(pDir);
   		}
   		return SUCCESS_DIR_CREATED;
	}
	else{
		cout<<"Can't create the file.Directory doesn't exist ";
		return FAILURE;
	}
 
}

enum CommandState deleteDir(string path){
	int isNotDeleted;
    DIR *pDir;    
    path=stackBackHistory.back()+path;
    //check if its current working directory
    struct stat info1;struct stat info2;
    stat(path.c_str(),&info1);stat((stackBackHistory.back()).c_str(),&info2);
    if(info1.st_dev==info2.st_dev&&info1.st_ino==info2.st_ino){
        cout<<" Error: Cant remove working directory ";
        return FAILURE;
    }
    //deleted
    isNotDeleted = rmdir(path.c_str());
   	if (!isNotDeleted){
   		pDir = openDirectory(stackBackHistory.back().c_str());
		//load current directory again
		if(pDir==NULL)
			return FAILURE;
		getFileList(pDir);
		printFilesWinDependent(0,windLine-tailOmit,"");
		closedir(pDir);
		return SUCCESS_DIR_DELETED;
   	}
   	//not deleted if not a directory or directory not found
   	else
   	{   
   		cout<<" Error: "<<strerror(errno);
        return FAILURE;
   	}
    
}

enum CommandState deleteFile(string path){
	int isNotDeleted;
    DIR *pDir;    
    path=stackBackHistory.back()+path;
    //deleted
    isNotDeleted = unlink(path.c_str());
   	if (!isNotDeleted){
   		pDir = openDirectory(stackBackHistory.back().c_str());
		//load current directory again
		if(pDir==NULL)
			return FAILURE;
		getFileList(pDir);
		printFilesWinDependent(0,windLine-tailOmit,"");
		closedir(pDir);
		return SUCCESS_FILE_DELETED;
   	}
   	
   	else
   	{   
   		cout<<" Error: "<<strerror(errno);
        return FAILURE;
   	}
    
}


