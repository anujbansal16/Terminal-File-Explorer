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
        	cout<<" Too many few arguments for "<<GOTO<<" expecting exactly 2";;
	}

	else if(opcode==CREATE_DIR){
		if(words.size()==3)
        	return createDirectory(words[1],words[2]);
        else
        	cout<<" Too many few arguments for "<<CREATE_DIR<<" expecting exactly 3";;
	}

	else if(opcode==CREATE_FILE){
		if(words.size()==3)
        	return createFile(words[1],words[2]);
        else
        	cout<<" Too many few arguments for "<<CREATE_FILE<<" expecting exactly 3";;
	}

	else if(opcode==DELETE_DIR){
		if(words.size()==2)
        	return deleteDir(words[1]);
        else
        	cout<<" Too many few arguments for "<<DELETE_DIR<<" expecting exactly 2";
	}

	else if(opcode==DELETE_FILE){
		if(words.size()==2)
        	return deleteFile(words[1]);
        else
        	cout<<" Too many few arguments for "<<DELETE_FILE<<" expecting exactly 2";
	}

	else if(opcode==COPY){
		if(words.size()>=3)
        	return copy(words);
        else
        	cout<<" Very few arguments "<<COPY<<" expecting atleast 3";
	}
	else{
		cout<<" Please enter a valid command";
	}
	
	return FAILURE;
}


/*
AUTHOR:         ANUJ
DESCRIPTION:    Go to a particular directory given a absolute path must start from /
RETURN:         status of commands SUCCESS_GOTO or Failure
*/
enum CommandState gotoDirectory(string directory){
		/*if(directory=="./"||directory=="/"){
			initialLS();
			return SUCCESS_GOTO;
		}*/
		string path;
		if(path[0]=='.'){
			cout<<"Path must start from / or ~ not .";
			return FAILURE;
		}
		if(directory[0]=='~'){
			path=("."+directory.substr(1,directory.size()-1)+"/");
		}
		else if(directory[0]=='/'){
			if(directory.size()==1)
				path="./";
			else
				path=("."+directory+"/");
		}
		//relative path starting from directory name
		else{
			path=stackBackHistory.back()+directory+"/";
		}
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
	if((path[0]=='.'&&path[1]=='.') || (path[0]=='.'&&path[1]=='/') ){
		cout<<"Path must start from / or ~ or directoryName not .";
		return FAILURE;
	}
	if(path[0]=='.'){
		path=stackBackHistory.back()+dirName;
	}
	else if(path[0]=='~'){
		path=("."+path.substr(1,path.size()-1)+"/"+dirName);
	}
	else if(path[0]=='/'){
		if(path.size()==1)
			path="."+path+dirName;
		else
			path=("."+path+"/"+dirName);
	}
	else{
		path=stackBackHistory.back()+path+"/"+dirName;
	}
    //create directory in pwd
   /* if(path==".")
    	path=stackBackHistory.back()+dirName;
    else//directory in given path
    	path="."+path.substr(1,path.size())+"/"+dirName;*/
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
        cout<<" Unable to create directory: "<<strerror(errno);
        return FAILURE;
    }
 
}
enum CommandState createFile(string fileName,string path){
    DIR *pDir;
    if((path[0]=='.'&&path[1]=='.') || (path[0]=='.'&&path[1]=='/') ){
			cout<<"Path must start from / or ~ or directoryName not";
			return FAILURE;
	}
	if(path[0]=='.'){
		path=stackBackHistory.back();
	}
	else if(path[0]=='~'){
		path=("."+path.substr(1,path.size()-1)+"/");
	}
	else if(path[0]=='/'){
		if(path.size()==1)
			path="./";
		else
			path=("."+path+"/");
	}
	else{
		path=stackBackHistory.back()+path+"/";
	}
   /* if(path==".")
    	path=stackBackHistory.back();
    else//file in given path
    	path="."+path.substr(1,path.size())+"/";*/
    struct stat info;
    //if directory already exist?
    if (stat(path.c_str(), &info) != -1) {
   		if (S_ISDIR(info.st_mode)) {
   			open((path+fileName).c_str(), O_WRONLY | O_CREAT , 0664);
			//fopen ((path+fileName).c_str(),"w+");
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
    if((path[0]=='.'&&path[1]=='.') || (path[0]=='.'&&path[1]=='/') ){
			cout<<"Path must start from / or ~ or directoryName not";
			return FAILURE;
	}
	if(path[0]=='.'){
		path=stackBackHistory.back();
	}
	else if(path[0]=='~'){
		path=("."+path.substr(1,path.size()-1)+"/");
	}
	else if(path[0]=='/'){
		if(path.size()==1)
			path="./";
		else
			path=("."+path+"/");
	}
	else{
		path=stackBackHistory.back()+path+"/";
	}    
    /*path=stackBackHistory.back()+path;*/
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
    if((path[0]=='.'&&path[1]=='.') || (path[0]=='.'&&path[1]=='/') ){
			cout<<"Path must start from / or ~ or directoryName not";
			return FAILURE;
	}
	else if(path[0]=='~'){
		path=("."+path.substr(1,path.size()-1));
	}
	else if(path[0]=='/'){
		if(path.size()==1)
			path="./";
		else
			path=("."+path);
	}
	else{
		path=stackBackHistory.back()+path;
	}        
    /*path=stackBackHistory.back()+path;*/
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

enum CommandState copy(vector<string> words){
	DIR *pDir;
	vector<string> filenames;
	struct stat info;
    string currentDir,path,dPath;
    currentDir=stackBackHistory.back();
    int isFileNotExist;
    for (vector<string>::iterator i = words.begin()+1; i != words.end()-1; ++i)
    {
        if(*i=="."||*i==".."){
        	cout<<" Can't copy current/parent directory ";
            return FAILURE;
        }
        path=currentDir+(*i);
        isFileNotExist=stat(path.c_str(),&info);
        if(!isFileNotExist){
        	filenames.push_back(*i);
        }
        else{
            cout<<" Can't copy "<<"'"<<*i<<"'"<<" : No such file or directory";
            return FAILURE;
        }
    
    }
    dPath=words[words.size()-1];
    if((dPath[0]=='.'&&dPath[1]=='.') || (dPath[0]=='.'&&dPath[1]=='/') ){
			cout<<"Path must start from / or ~ or directoryName not";
			return FAILURE;
	}
	else if(dPath[0]=='.'){
		dPath=stackBackHistory.back();
	}
	else if(dPath[0]=='~'){
		dPath=("."+dPath.substr(1,dPath.size()-1)+"/");
	}
	else if(dPath[0]=='/'){
		if(dPath.size()==1)
			dPath="./";
		else
			dPath=("."+dPath+"/");
	}
	else{
		dPath=stackBackHistory.back()+dPath+"/";
	}
    /*if(dPath==".")
    	dPath=stackBackHistory.back();
    else//file in given path
    	dPath="."+dPath.substr(1,dPath.size())+"/";*/

	enum CommandState isSuccess=copyFilesRecursively(filenames,dPath);
	if(isSuccess==SUCCESS_COPY){
		pDir = openDirectory(stackBackHistory.back().c_str());
		//load current directory again
		if(pDir==NULL)
			return FAILURE;
		getFileList(pDir);
		printFilesWinDependent(0,windLine-tailOmit,"");
		closedir(pDir);	
	}
	return isSuccess;
	
    
}


enum CommandState copyFilesRecursively(vector<string> filenames, string destination){
	for (vector<string>::iterator i = filenames.begin(); i != filenames.end(); ++i)
	{
		struct stat info;
		string fileName=*i,filePath;
		//if same file already exist
		filePath=stackBackHistory.back()+fileName;
    	while(stat((destination+fileName).c_str(),&info)!=-1){
    		size_t found = fileName.rfind(".");
  			if (found!=std::string::npos)
  				fileName=fileName.substr(0,found)+"-copy"+fileName.substr(found,fileName.size()-found);
  			else
  				fileName=fileName+"-copy";
    	}
		if(isDirectory(filePath)){
			bool isNotCreated;
			isNotCreated=mkdir((destination+fileName).c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			/*while(isNotCreated){
				fileName=fileName+"-copy";
				isNotCreated=mkdir((destination+fileName).c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			}*/
			if(isNotCreated){
				cout<<strerror(errno);
				return FAILURE;
			}
			copyDir(filePath,destination+fileName+"/");
		}
		else{
			copyFile(filePath,destination+fileName);
		}
	}
	return SUCCESS_COPY;
}

void copyDir(string sourcePath,string destination){
	DIR *pDir;
    struct dirent *pDirent;
    string fileName, tempSourcePath, tempDestinationPath;
    if((pDir=opendir(sourcePath.c_str())))
    {
        while((pDirent=readdir(pDir))!= NULL){
            fileName = pDirent->d_name;
            if((fileName!=".")&&(fileName!=".."))
            {
                tempSourcePath = sourcePath+"/"+fileName;
                if (isDirectory(tempSourcePath))
                {
                    tempDestinationPath = destination+fileName;
                    mkdir(tempDestinationPath.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                    copyDir(tempSourcePath, tempDestinationPath+"/");
                }
                else
                {
                    tempDestinationPath = destination+fileName;
                    copyFile(tempSourcePath, tempDestinationPath);
                }
            }
        }
        closedir(pDir);
    }

}


void copyFile(string filePath,string destination){
	//size_t BUFFER_SIZE = 1024;
    char inputB[BUFSIZ];
    size_t size;
    int inF =open(filePath.c_str(),O_RDONLY, 0);
    int outF = open(destination.c_str(), O_WRONLY | O_CREAT , 0664);
 	 while ((size = read(inF, inputB, BUFSIZ)) > 0) {
        write(outF, inputB, size);
    }
    close(inF);
    close(outF);
}

bool isDirectory(string path)
{
    struct stat info;
    stat(path.c_str(), &info);
    return S_ISDIR(info.st_mode);
}

