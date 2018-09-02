/***********************************************************

OWNER:  ANUJ BANSAL
ROLLNO. 2018201096
COPYRIGHT PROTECTED
***********************************************************/
#include"listDir.h"

vector<string> Flist;
struct winsize w;
unsigned long tailOmit=1;
unsigned long windLine;
unsigned long col;
vector<string> stackBackHistory; //visited directory history format=> ./|dir1|/|dir2|
vector<string> stackForwardHistory;//next visited directory history format=> ./|dir1|/|dir2|/
map <string,bool> fileToISDirecMap; //filename mapping to boolean to determine if file is directory

/*
AUTHOR:         ANUJ
DESCRIPTION:    Print number of files depending upon the window size
PARAMETERS:     Input start index, Last index(determined by window size), path (optional)
*/
void printFilesWinDependent(unsigned long firstIndex,unsigned long lastIndex,string path){
        clearConsole();
        //to print files current directory path need to be on the top of the stackBackHistory
        for(unsigned long i=firstIndex;i<lastIndex&&i<Flist.size();i++){
            string s=Flist[i];
            printStatInfo(getStatInfo(stackBackHistory.back(),s),s);    
        }
        cursorMove(1000,1);//move cursor to last line of terminal
        cursorUp(1);//move cursor to 1 line up to print path
        string displayPath=(stackBackHistory.back()).substr(1,(stackBackHistory.back()).size()-1);
        //display path (append ... if path is greater than the window size)
        if(displayPath.size()+1>col)
            printf("%s...",(displayPath.substr(0,col-3)).c_str());
        else
        printf("%s",displayPath.c_str());
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Goto next visited directory (Using forward history stack) on RIGHT arrow key event
RETURN:         number of files present in the next visited directory
*/
unsigned long forwardDirect(){
    unsigned long totalfiles=Flist.size();
    //check if forward stack is empty
    if(!(stackForwardHistory.empty())){
        //forward traverse directory should be popped out and will pushed in back history stack
        string path=stackForwardHistory.back();
        stackForwardHistory.pop_back();
        stackBackHistory.push_back(path);
        DIR * pDir = openDirectory(path.c_str());
        getFileList(pDir);
        totalfiles=Flist.size();
        printFilesWinDependent(0,windLine-tailOmit,path);
        closedir (pDir);
        return totalfiles;
    }
    //nothing in stack
    return 0;
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Goto parent directory (Using backward history stack) on BACKSPACE arrow key event
RETURN:         number of files present in the next visited directory
*/
unsigned long backspace(){
    string fName="..";string filePath;
    struct stat info1;struct stat info2;
    stat("./..",&info1);stat((stackBackHistory.back()+fName).c_str(),&info2);
    if(info1.st_dev==info2.st_dev&&info1.st_ino==info2.st_ino){
        filePath="./";
    }
    else{
        string tfileName=(stackBackHistory.back()).substr(0,(stackBackHistory.back()).size()-1);
        size_t found = tfileName.rfind("/");
        if (found!=std::string::npos)
            tfileName=tfileName.substr(0,found);
        stackBackHistory.push_back(tfileName+"/");
        filePath=stackBackHistory.back();//format - ./|dirname|/
    }
    DIR * pDir = openDirectory(filePath.c_str());
    getFileList(pDir);
    unsigned long totalfiles=Flist.size();
    printFilesWinDependent(0,windLine-tailOmit,filePath);
    closedir (pDir);
    return totalfiles;
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Go to Home(Root) Directory
RETURN:         number of files present in the root directory
*/
unsigned long goHome(){
        return initialLS();
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Goto previously visited directory (Using backward history stack) on LEFT arrow key event
RETURN:         number of files present in the next visited directory
*/
unsigned long backDirect(){
    unsigned long totalfiles=Flist.size();
    //check if we are at root ie './''
    if((stackBackHistory.size()!=1)){
        string path;
        //push the directory from where we are going back for forward traversing
        stackForwardHistory.push_back(stackBackHistory.back());
        //take the current directory out to go back on previous directory
        stackBackHistory.pop_back();
        //previous directory
        path=stackBackHistory.back();    
        DIR * pDir = openDirectory(path.c_str());
        getFileList(pDir);
        totalfiles=Flist.size();
        printFilesWinDependent(0,windLine-tailOmit,path);
        closedir (pDir);
        return totalfiles;
    }
    //Nothing in stack
    return 0;
}


/*
AUTHOR:         ANUJ
DESCRIPTION:    Open up the file using mimeOpen
PARAMETERS:     path of file
*/

void openFile(string filePath){
    //redirecting errors to a file
    freopen("error.txt", "w", stderr);
    pid_t pid = fork();
    int status;
    if (pid == -1){freopen("error.txt", "w", stderr);
        printf("can't fork, error occured\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        freopen("error.txt", "w", stdout);
        if(execl("/usr/bin/mimeopen", "mimeopen","-n",filePath.c_str(),NULL)<0){
        //if(execl("/usr/bin/xdg-open", "xdg-open",filePath.c_str(),NULL)<0){
            perror ("execv");
            exit(0);
        }
    }
    else{
         if (waitpid(pid, &status, 0) > 0) {
            if (WIFEXITED(status) && !WEXITSTATUS(status)) ;
        } 
        else {
           printf("waitpid() failed\n");
           exit(0);
        }
    }
    
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    This function handles ENTER KEY PRESS EVENT
PARAMETERS:     line number where user hit ENTER
RETURN:         number of files(if directory) | -1 (if regular file)
*/

long enterDirectory(unsigned long indexOfFile){
    unsigned long totalfiles;
    string fName=Flist[indexOfFile-1];//name of the file or directory
    //check if we are in root directory and enterning ..
    struct stat info1;struct stat info2;
    stat("./..",&info1);stat((stackBackHistory.back()+fName).c_str(),&info2);
    if(info1.st_dev==info2.st_dev&&info1.st_ino==info2.st_ino){
        //controlling enter press on .. in root directory
        if(fName.compare("..")==0){
            fName=".";
        }
    }
    string filePath=stackBackHistory.back()+fName;//format=> ./|dirname|/
    //Check if file is directory and push its name in stack
    if(fileToISDirecMap.find(fName)!=fileToISDirecMap.end()){
        if(fileToISDirecMap[fName]){
            if(fName.compare(".")==0);//no push pop in case filename is .
            else if(fName.compare("..")==0){
                //pop out the last directory name from stack
                string tfileName=(stackBackHistory.back()).substr(0,(stackBackHistory.back()).size()-1);
                size_t found = tfileName.rfind("/");
                if (found!=std::string::npos)
                    tfileName=tfileName.substr(0,found);
                stackBackHistory.push_back(tfileName+"/");
            }
            else{
                //clear the forward stack when moving to different directory from root
                if((stackBackHistory.size()==1)){
                       stackForwardHistory.clear();
                }
                //push the complete absolute path of current directory
                stackBackHistory.push_back(stackBackHistory.back()+fName+"/");//format - ./|dirname|/
            }
            filePath=stackBackHistory.back();//format - ./|dirname|/
            DIR * pDir = openDirectory(filePath.c_str());
            getFileList(pDir);
            totalfiles=Flist.size();
            printFilesWinDependent(0,windLine-tailOmit,filePath);
            closedir (pDir);
        }
    }
    //open the file if the regular file is ENTERED
    else{        
        openFile(filePath);
        totalfiles=-1;
    }
    return totalfiles;
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    This function will print the initial ls -l type of listing of application
                root directory files.
*/
unsigned long initialLS(){
    stackBackHistory.clear();
    stackForwardHistory.clear();
    ioctl(0, TIOCGWINSZ, &w);
    windLine=w.ws_row-1;
    col=w.ws_col;
	unsigned long totalfiles;	
    DIR *pDir;
    const char* rootPath="./";
    stackBackHistory.push_back(rootPath);//stack push ./
    pDir = openDirectory(rootPath);
    getFileList(pDir);
    totalfiles=Flist.size();        
    printFilesWinDependent(0,windLine-tailOmit,rootPath);
    closedir (pDir);
    return totalfiles;

}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Open and return the directory
PARAMETERS:     path of directory
*/
DIR * openDirectory(const char* s){
    DIR *pDir;
    pDir = opendir(s);
    if (pDir == NULL) {
            cout<<" "<<(strerror(errno));
    }
    return pDir;
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    This function read and initialize the vector of fileNames (Used in indexing to open a file)
PARAMETERS:     DIR * - directory we want to read
*/
void getFileList(DIR * pDir){
    Flist.clear();
    fileToISDirecMap.clear();
    struct dirent *pDirent;
    while ((pDirent = readdir(pDir)) != NULL) {
        string s=pDirent->d_name;
        Flist.push_back(s);                        
    }
    if(!Flist.empty())
        sort(Flist.begin(),Flist.end());
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    This function will return the status of a file.
PARAMETERS:     Path of directory of file (ended with /) , filename
*/
struct stat getStatInfo(string rootPath, string fName){
    struct stat info;   
    if(stat((rootPath+fName).c_str(), &info)==-1){
        perror(strerror(errno));
    }
    if(S_ISDIR(info.st_mode))
        fileToISDirecMap[fName]=true;
    return info;
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    This function will print the row of File information
PARAMETERS:     stat, filename
*/

void printStatInfo(struct stat info, string fName){
    struct passwd *pswd=getpwuid(info.st_uid);
    struct group *grp=getgrgid(info.st_gid);  
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
    string time=((string)ctime(&info.st_mtime)).substr(0,24);
    printf("%s",per.c_str());
    cout<<setw(8)<<right<<pswd->pw_name;
    cout<<setw(8)<<right<<grp->gr_name;
    printf(" %s",time.c_str());
    printHumanReadableSize(info.st_size);
    //dynamic adjustment of text based on window columns ... apending
    if(fName.size()+62>col)
        printf(" %s...",(fName.substr(0,col-64)).c_str());
    else
        printf(" %s",fName.c_str());
    cout<<endl; 
}

/*
DESCRIPTION:    This function will print the header information of file listed
*/
void printHead(){
	//cout<<"PERMISSIONS"<<"\t"<<"OWNER"<<"\t"<<"GROUP"<<"\t"<<"SIZE"<<"\t"<<"LAST-MODIFIED"<<"\t\t\t"<<"NAME\n";
}
