/***********************************************************

OWNER:  ANUJ BANSAL
ROLLNO. 2018201096
COPYRIGHT PROTECTED
***********************************************************/

#include"commandModeOperations.h"
extern vector<string> Flist;
//External variables defined in listDir.cpp
extern vector<string> stackBackHistory;
extern unsigned long windLine;
extern unsigned long col;
extern unsigned long tailOmit;
extern map <string,bool> fileToISDirecMap;

vector<string> searchResults;//store searched filenames

/*
AUTHOR:         ANUJ
DESCRIPTION:    Perform tokenization of input buffer and call execute method
PARAMETERS:     input buffer, number of characters in buffer
RETURN:         status of executed command (return value of execute method)
*/
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

/*
AUTHOR:         ANUJ
DESCRIPTION:    Execute the command by calling respecive command functions
PARAMETERS:     vector of words (given by user with space separation)
RETURN:         status of executed command
*/
enum CommandState execute(vector<string> words){
	string opcode=words[0];
	if(opcode==GOTO){
		if(words.size()==2)
        	return gotoDirectory(words[1]);
        else
        	cout<<" Too many/very few arguments for "<<GOTO<<" expecting exactly 2";;
	}
	else if(opcode==CREATE_DIR){
		if(words.size()==3)
        	return createDirectory(words[1],words[2]);
        else
        	cout<<" Too many/very few arguments for "<<CREATE_DIR<<" expecting exactly 3";;
	}
	else if(opcode==CREATE_FILE){
		if(words.size()==3)
        	return createFile(words[1],words[2]);
        else
        	cout<<" Too many/very few arguments for "<<CREATE_FILE<<" expecting exactly 3";;
	}
	else if(opcode==DELETE_DIR){
		if(words.size()==2)
        	return deleteDir(words[1],false);
        else
        	cout<<" Too many/very few arguments for "<<DELETE_DIR<<" expecting exactly 2";
	}
	else if(opcode==DELETE_FILE){
		if(words.size()==2)
        	return deleteFile(words[1],false);
        else
        	cout<<" Too many/very few arguments for "<<DELETE_FILE<<" expecting exactly 2";
	}
	else if(opcode==COPY){
		if(words.size()>=3)
        	return copyOrMove(words,opcode);
        else
        	cout<<" Very few arguments "<<COPY<<" expecting atleast 3";
	}
	else if(opcode==MOVE){
		if(words.size()>=3)
        	return copyOrMove(words,opcode);
        else
        	cout<<" Very few arguments "<<MOVE<<" expecting atleast 3";
	}
	else if(opcode==RENAME){
		if(words.size()==3)
        	return renameF(words[1],words[2]);
        else
        	cout<<" To many/few few arguments "<<RENAME<<" expecting exactly 3";
	}
	else if(opcode==SNAPSHOT){
		if(words.size()==3)
        	return snapShotF(words[1],words[2]);
        else
        	cout<<" To many/few few arguments "<<RENAME<<" expecting exactly 3";
	}
	else if(opcode==SEARCH){
		if(words.size()==2)
        	return search(words[1]);
        else
        	cout<<" To many/few few arguments "<<SEARCH<<" expecting exactly 2";
	}
	else{
		cout<<" Please enter a valid command";
	}
	
	return FAILURE;
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Rename a file or Directory
PARAMETERS:     source file path(to be renamed), destination file path(new name)
RETURN:         status of executed command
*/
enum CommandState renameF(string source,string destination){
	if((source[0]=='.'&&source[1]=='.') || (source[0]=='.'&&source[1]=='/') ){
				cout<<" Path must start from / or ~ or directoryName not .";
				return FAILURE;
	}
	if(source[0]=='.'){
		source=stackBackHistory.back();
	}
	else if(source[0]=='~'){
		source=("."+source.substr(1,source.size()-1));
	}
	else if(source[0]=='/'){
		if(source.size()==1){
			cout<<" Can't rename the root directory";
				return FAILURE;
		}
		else
			source=("."+source);
	}
	else{
		source=stackBackHistory.back()+source;
	} 
	if((destination[0]=='.'&&destination[1]=='.') || (destination[0]=='.'&&destination[1]=='/') ){
		cout<<" Path must start from / or ~ or directoryName not .";
		return FAILURE;
	}
	if(destination[0]=='.'){
		destination=stackBackHistory.back();
	}
	else if(destination[0]=='~'){
		destination=("."+destination.substr(1,destination.size()-1));
	}
	else if(destination[0]=='/'){
		if(destination.size()==1){
			cout<<" Newname cant be root directory";
				return FAILURE;
		}
		else
			destination=("."+destination);
	}
	else{
		destination=stackBackHistory.back()+destination;
	} 
	struct stat info;
    int isFileNotExist=stat(source.c_str(),&info);
    if(!isFileNotExist){
		int isNotRenamed = rename(source.c_str(), destination.c_str());
		if(isNotRenamed == 0) {
			DIR *pDir;
			pDir = openDirectory((stackBackHistory.back()).c_str());
			if(pDir==NULL)
				return FAILURE;
	        getFileList(pDir);
	        printFilesWinDependent(0,windLine-tailOmit,"");
	        closedir(pDir);
		} 
		else {
			cout<<" Can't "<<RENAME<<strerror(errno);
		}
    }
    else{
        cout<<" Can't "<<RENAME<<" '"<<source<<"'"<<" : No such file or directory";
        return FAILURE;
    }
	return SUCCESS_RENAME;
}


/*
AUTHOR:         ANUJ
DESCRIPTION:    Go to a particular directory given a absolute path must start from /
PARAMETERS:     Directory path- 
				Absoute- starting from / or ~/
				Relative- starting from DirName1/Dirname2 (Not ./DirName/Dirname2)
RETURN:         status of executed command
*/
enum CommandState gotoDirectory(string directory){
		string path;
		if(path[0]=='.'){
			cout<<" Path must start from / or ~ not .";
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

/*
AUTHOR:         ANUJ
DESCRIPTION:    Create directory
PARAMETERS:     Directory Name, Path (Absolute/Relative)
				Absoute- starting from / or ~/
				Relative- starting from DirName1/Dirname2 (Not ./DirName/Dirname2)
RETURN:         status of executed command
*/
enum CommandState createDirectory(string dirName,string path){
    int isNotCreated;
    DIR *pDir;
	if((path[0]=='.'&&path[1]=='.') || (path[0]=='.'&&path[1]=='/') ){
		cout<<" Path must start from / or ~ or directoryName not .";
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
    isNotCreated = mkdir(path.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    //Created
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
    //Not created
    else {
        cout<<" Unable to create directory: "<<strerror(errno);
        return FAILURE;
    }
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Create File
PARAMETERS:     Filename, Path (Absolute/Relative)
				Absoute- starting from / or ~/
				Relative- starting from DirName1/Dirname2 (Not ./DirName/Dirname2)
RETURN:         status of executed command
*/
enum CommandState createFile(string fileName,string path){
    DIR *pDir;
    if((path[0]=='.'&&path[1]=='.') || (path[0]=='.'&&path[1]=='/') ){
			cout<<" Path must start from / or ~ or directoryName not .";
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
    struct stat info;
    //if directory already exist?
    if (stat(path.c_str(), &info) != -1) {
   		if (S_ISDIR(info.st_mode)) {
   			open((path+fileName).c_str(), O_WRONLY | O_CREAT , 0664);			
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
		cout<<" Can't create the file.Directory doesn't exist ";
		return FAILURE;
	}
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Delete directory(only Empty Directory)
PARAMETERS:     Path (Absolute/Relative), insideCode=false if call from user else true
				Absoute- starting from / or ~/
				Relative- starting from DirName1/Dirname2 (Not ./DirName/Dirname2)
RETURN:         status of executed command
*/
enum CommandState deleteDir(string path, bool insideCode){
	int isNotDeleted;
    DIR *pDir;
    if(!insideCode){
	    if((path[0]=='.'&&path[1]=='.') || (path[0]=='.'&&path[1]=='/') ){
				cout<<" Path must start from / or ~ or directoryName not .";
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
	}
    //check if its current working directory
    struct stat info1;struct stat info2;
    stat(path.c_str(),&info1);stat((stackBackHistory.back()).c_str(),&info2);
    if(info1.st_dev==info2.st_dev&&info1.st_ino==info2.st_ino){
        cout<<" Cant remove working directory ";
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

/*
AUTHOR:         ANUJ
DESCRIPTION:    Delete File
PARAMETERS:     Path (Absolute/Relative), insideCode=false if call from user else true
				Absoute- starting from / or ~/
				Relative- starting from DirName1/Dirname2 (Not ./DirName/Dirname2)
RETURN:         status of executed command
*/
enum CommandState deleteFile(string path,bool insideCode){
	int isNotDeleted;
    DIR *pDir;
    if(!insideCode){
	    if((path[0]=='.'&&path[1]=='.') || (path[0]=='.'&&path[1]=='/') ){
			cout<<" Path must start from / or ~ or directoryName not .";
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
	}
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
   		cout<<" Error: "<<" "<<strerror(errno);
        return FAILURE;
   	}
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Copy of move abstaction
PARAMETERS:     Vector of file names, opcode=copy/move
RETURN:         status of executed command
*/
enum CommandState copyOrMove(vector<string> words,string opcode){
	DIR *pDir;
	vector<string> filenames;
	struct stat info;
    string currentDir,path,dPath;
    currentDir=stackBackHistory.back();
    int isFileNotExist;
    enum CommandState isSuccess;
    for (vector<string>::iterator i = words.begin()+1; i != words.end()-1; ++i)
    {
        if(*i=="."||*i==".."){
        	cout<<" Can't "<<opcode<<" current/parent directory ";
            return FAILURE;
        }
        path=currentDir+(*i);
        isFileNotExist=stat(path.c_str(),&info);
        if(!isFileNotExist){
        	filenames.push_back(*i);
        }
        else{
            cout<<" Can't "<<opcode<<" '"<<*i<<"'"<<" : No such file or directory";
            return FAILURE;
        }
    
    }
    dPath=words[words.size()-1];
    if((dPath[0]=='.'&&dPath[1]=='.') || (dPath[0]=='.'&&dPath[1]=='/') ){
		cout<<" Path must start from / or ~ or directoryName not";
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
	if(opcode==COPY)
		isSuccess=copyFilesRecursively(filenames,dPath);
	if(opcode==MOVE){
		isSuccess=copyFilesRecursively(filenames,dPath);
		if(isSuccess==SUCCESS_COPY)
			isSuccess=deleteFilesRecursively(filenames,true);
	}
	if(isSuccess==SUCCESS_COPY || isSuccess==SUCCESS_MOVE){
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

/*
AUTHOR:         ANUJ
DESCRIPTION:    Delete File Recursively - called from move command
PARAMETERS:     Vector of file names, insideCode=false if call from user else true
RETURN:         status of executed command
*/
enum CommandState deleteFilesRecursively(vector<string> filenames,bool insideCode){
	for (vector<string>::iterator i = filenames.begin(); i != filenames.end(); ++i)
	{
		string fileName=*i,filePath;
		filePath=stackBackHistory.back()+fileName;
		if(isDirectory(filePath)){
			deleteDirRecursively(filePath,insideCode);
			deleteDir(filePath,insideCode);
		}
		else{
			deleteFile(filePath,insideCode);
		}
	}
	return SUCCESS_MOVE;
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Delete Directory Recursive Function - called from deleteFilesRecursively
PARAMETERS:     Path (Absolute/Relative), insideCode=false if call from user else true
				Absoute- starting from / or ~/
				Relative- starting from DirName1/Dirname2 (Not ./DirName/Dirname2)
*/
void deleteDirRecursively(string filePath, bool insideCode){
	DIR *pDir;
    struct dirent *pDirent;
    string fileName, tempSourcePath;
    if((pDir=opendir(filePath.c_str())))
    {
        while((pDirent=readdir(pDir))!= NULL){
            fileName = pDirent->d_name;
            if((fileName!=".")&&(fileName!=".."))
            {
                tempSourcePath = filePath+"/"+fileName;
                if (isDirectory(tempSourcePath))
                {
                    deleteDirRecursively(tempSourcePath,insideCode);
                    deleteDir(tempSourcePath,insideCode);
                }
                else
                {
                    deleteFile(tempSourcePath,insideCode);
                }
            }
        }
        closedir(pDir);
    }
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Copy Files Recursively - called from copy command
PARAMETERS:     vector of file names, Destinaton path
				Absoute- starting from / or ~/
				Relative- starting from DirName1/Dirname2 (Not ./DirName/Dirname2)
RETURN:         status of executed command
*/
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

/*
AUTHOR:         ANUJ
DESCRIPTION:    Copy Directory Recursive Function - called from copyFilesRecursively
PARAMETERS:     Sorce Path (Absolute/Relative), Destination path (Absolute/Relative)
				Absoute- starting from / or ~/
				Relative- starting from DirName1/Dirname2 (Not ./DirName/Dirname2)
*/
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

/*
AUTHOR:         ANUJ
DESCRIPTION:    Copy File Function 
PARAMETERS:     Sorce Path (Absolute/Relative), Destination path (Absolute/Relative)
				Absoute- starting from / or ~/
				Relative- starting from DirName1/Dirname2 (Not ./DirName/Dirname2)
*/
void copyFile(string filePath,string destination){
	//size_t BUFFER_SIZE = 1024;
    char inputB[BUFSIZ];
    size_t size;
    int inF =open(filePath.c_str(),O_RDONLY, 0);
    struct stat info;
    stat(filePath.c_str(),&info);
    int outF = open(destination.c_str(), O_WRONLY | O_CREAT , info.st_mode);
 	 while ((size = read(inF, inputB, BUFSIZ)) > 0) {
        write(outF, inputB, size);
    }
    close(inF);
    close(outF);
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    SnapShot abstraction function, calls snapRec recursive function
PARAMETERS:     Directory path to snap (Absolute/Relative), Destination path (Absolute/Relative) to create dump file
				Absoute- starting from / or ~/
				Relative- starting from DirName1/Dirname2 (Not ./DirName/Dirname2)
RETURN:         status of executed command
*/
enum CommandState snapShotF(string path, string destination){
	if((path[0]=='.'&&path[1]=='.') || (path[0]=='.'&&path[1]=='/') ){
		cout<<"Path must start from / or ~ or directoryName not .";
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
			path="."+path;
		else
			path=("."+path+"/");
	}
	else{
		path=stackBackHistory.back()+path+"/";
	}
	if((destination[0]=='.'&&destination[1]=='.') || (destination[0]=='.'&&destination[1]=='/') ){
		cout<<"Path must start from / or ~ or directoryName not .";
		return FAILURE;
	}
	if(destination[0]=='.'){
		destination=stackBackHistory.back();
	}
	else if(destination[0]=='~'){
		destination=("."+destination.substr(1,destination.size()-1));
	}
	else if(destination[0]=='/'){
		if(destination.size()==1)
			destination="."+destination;
		else
			destination=("."+destination);
	}
	else{
		destination=stackBackHistory.back()+destination;
	}
	if(isDirectory(path)){
		snapRec(path,destination,"");
		DIR *pDir = openDirectory(stackBackHistory.back().c_str());
		//load current directory again
		if(pDir==NULL)
			return FAILURE;
		getFileList(pDir);
		printFilesWinDependent(0,windLine-tailOmit,"");
		closedir(pDir);
		return SUCCESS_SNAPSHOT;
	}
	else{
		cout<<"Error:Not a Directory";
		return FAILURE;
	}
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    SnapShot Recursive function function, called from snapshotF function
PARAMETERS:     Directory path to snap (Absolute/Relative), 
				Destination path (Absolute/Relative) to create dump file
					Absoute- starting from / or ~/
					Relative- starting from DirName1/Dirname2 (Not ./DirName/Dirname2)
				seachfile=> used for search command not for snapshot
*/
void snapRec(string path, string destination, string searchFile){
	DIR *pDir;
    struct dirent *pDirent;
    string fileName, tempSourcePath;
    stack <string> q;
    //int outF = open(destination.c_str(), O_APPEND | O_CREAT , 0664);
    ofstream outFile;
	if(searchFile.size()==0)
		outFile.open(destination,ofstream::out); 
	
	q.push(path);
	while(!q.empty()){
		string front=q.top();
		q.pop();
		string pathWithoutDot=front.substr(1,front.size()-1);
		if(searchFile.size()==0)
			outFile<<pathWithoutDot<<":\n";	
	    if((pDir=opendir(front.c_str())))
	    {	
	        while((pDirent=readdir(pDir))!= NULL){
	            fileName = pDirent->d_name;
	            if((fileName!=".")&&(fileName!=".."))
	            {
	            	if(searchFile.size()>0){
	            		checkAndAddResult(pathWithoutDot,fileName,searchFile);
	            	}
	                tempSourcePath = front+fileName;
	                if (isDirectory(tempSourcePath))
	                {
	                    q.push(tempSourcePath+"/");
	                }
	                if(searchFile.size()==0)
	                	outFile<<fileName<<" ";
	            }

	        }
	        if(searchFile.size()==0)
	        	outFile<<"\n\n";
	    }
	}
	closedir(pDir);
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    search file functionality abstractions- calls snapRec function to search file
PARAMETERS:     search file/directory name
RETURN:         status of executed command
*/
enum CommandState search(string searchFile){
	searchResults.clear();
	string currentDir=stackBackHistory.back();
	if(searchFile[0]=='.' || searchFile[0]=='/' || searchFile[0]=='~' ){
		cout<<"Enter a proper searchFile  Name";
		return FAILURE;
	}
	snapRec(currentDir,"",searchFile);
	if(searchResults.empty()){
			cout<<"No results found for "<<searchFile;
			return FAILURE;
	}
	stackBackHistory.push_back(currentDir);
	return printSearchResults(searchResults,searchFile);
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    search file functionality helper function to store searched results(path) in vector searchResults
PARAMETERS:     path of file (absoulute,relative), searchfile name
RETURN:         status of executed command
*/
void checkAndAddResult(string path,string fileName,string searchFile){
	string file;
	if(fileName==searchFile){
		cout<<path+file;
		file="."+path;
		searchResults.push_back(file);		
	}
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Print search file results ie permission of containing folder and containing folder absolute path
PARAMETERS:     Vector of searchedResults=>containg path and filename
RETURN:         status of executed command
*/
enum CommandState printSearchResults(vector<string> searchResults, string searchFileName){       
    struct stat info;   
    string searchedFile,dirPath,currentDir;
    clearConsole();
    Flist.clear();
	fileToISDirecMap.clear();
	currentDir=stackBackHistory.back();
	long cDirLen=currentDir.size();
    for(unsigned long i=0;i<searchResults.size();i++){
        searchedFile=searchResults[i];
        dirPath=searchedFile.substr(cDirLen,searchedFile.size()-cDirLen);
        Flist.push_back(searchedFile.substr(cDirLen,searchedFile.size()-cDirLen-1));
        fileToISDirecMap[searchedFile.substr(cDirLen,searchedFile.size()-cDirLen-1)]=true;
		if(stat((searchedFile).c_str(), &info)==-1){
    		perror(strerror(errno));
		}
		mode_t mode=info.st_mode;
		string per=((S_ISDIR(mode)) ? "d" : "-");
		per+=((mode & S_IRUSR) ? "r" : "-");
		per+=((mode & S_IWUSR) ? "w" : "-");
		per+=((mode & S_IXUSR) ? "x" : "-");
		per+=((mode & S_IRGRP) ? "r" : "-");
		per+=((mode & S_IWGRP) ? "w" : "-");
		per+=((mode & S_IXGRP) ? "x" : "-");
		per+=((mode & S_IROTH) ? "r" : "-");
		per+=((mode & S_IWOTH) ? "w" : "-");
		per+=((mode & S_IXOTH) ? "x" : "-");
		printf("%s",per.c_str());
		string fName=(searchedFile.substr(1,searchedFile.size()-1));
		//dynamic adjustment of text based on window columns
		if(fName.size()+12>col)
        	printf(" %s...",(fName.substr(0,col-14)).c_str());
		else
    		printf(" %s",fName.c_str());
		printf("\n");
    }
    return SUCCESS_SEARCH;
}


/*
AUTHOR:         ANUJ
DESCRIPTION:    Check if a file is Directory
PARAMETERS:     File path Absolute				
RETURN:         true if file is dirctory
*/
bool isDirectory(string path)
{
    struct stat info;
    stat(path.c_str(), &info);
    return S_ISDIR(info.st_mode);
}

