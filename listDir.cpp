/***********************************************************

OWNER:  ANUJ BANSAL
ROLLNO. 2018201096
COPYRIGHT PROTECTED
***********************************************************/
#include"listDir.h"
#include <sys/wait.h>
vector<string> Flist;
struct winsize w;   
unsigned long windLine;
vector<string> stackBackHistory; // ./|dir1/|dir2|
vector<string> stackForwardHistory;// dir1|dir2|
map <string,bool> fileToISDirecMap; //filename mapping to boolean to determine if file is directory

void printFilesWinDependent(unsigned long firstIndex,unsigned long lastIndex,string path){
        //0-41
        clearConsole();
        //to print files current directory path need to be on the top of the stacbackHistory
        for(unsigned long i=firstIndex;i<lastIndex&&i<Flist.size();i++){
            string s=Flist[i];
            printStatInfo(getStatInfo(stackBackHistory.back(),s),s);    
        }
}


/*
AUTHOR:         ANUJ
DESCRIPTION:    Update the name of file and fetches visited directory files on right arrow key press using FORWARD Stack
RETURN:         number of files present in the previous directory
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
        printFilesWinDependent(0,windLine-1,path);
        //cout<<path<<endl;
        closedir (pDir);
        }
        
        return totalfiles;

}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Update the name of file and fetches previous directory files on left arrow key press using BACKWARD Stack
RETURN:         number of files present in the previous directory
*/

unsigned long backspace(){
    return backDirect();
}


unsigned long backDirect(){
    unsigned long totalfiles=Flist.size();
    //check if we are at root ie './''
    //if((stackBackHistory.back().compare("./")!=0)){
    if((stackBackHistory.size()!=1)){
        string path;
        //push the directory from where we are going back for forward traversing
        stackForwardHistory.push_back(stackBackHistory.back());
        //take the current directory out to go back on previous directory
        stackBackHistory.pop_back();
        path=stackBackHistory.back();    
        //for (vector<string>::iterator i= stackBackHistory.begin(); i != stackBackHistory.end(); ++i){
        //    path+=*i;
        //}

        DIR * pDir = openDirectory(path.c_str());
        getFileList(pDir);
        totalfiles=Flist.size();
        printFilesWinDependent(0,windLine-1,path);
        //cout<<path<<endl;
        closedir (pDir);

    }
    return totalfiles;
    
}

/*
AUTHOR:         ANUJ
DESCRIPTION:    Open up the file using xdg-open
PARAMETERS:     path of file
*/

void openFile(string filePath){
    pid_t pid = fork();
    int status;
    if (pid == -1){
        printf("can't fork, error occured\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        if(execl("/usr/bin/xdg-open", "xdg-open",filePath.c_str(),NULL)<0){
            perror ("execv");
            exit(0);
        }
    }
    else{
         if (waitpid(pid, &status, 0) > 0) {
            if (WIFEXITED(status) && !WEXITSTATUS(status)) ;
              //printf("program execution successfull\n");
             
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
    string fName=Flist[indexOfFile-1];  //name of the file or directory
    //chck if we are in root directory and enterning ..
    struct stat info1;struct stat info2;
    stat("./..",&info1);stat((stackBackHistory.back()+fName).c_str(),&info2);
    if(info1.st_dev==info2.st_dev&&info1.st_ino==info2.st_ino){
    //if((stackBackHistory.back().compare("./")==0)){
        //controlling enter press on .. in root directory
        if(fName.compare("..")==0){
            fName=".";
        }
        //clearing forward history once we are at root and clicking on sibling directory
        //if(fileToISDirecMap.find(fName)!=fileToISDirecMap.end())
          //  if(fileToISDirecMap[fName]&&(fName.compare(".")!=0)&&(fName.compare("..")!=0))
            //    stackForwardHistory.clear();
    }
    string filePath=stackBackHistory.back()+fName+"/";//format - ./dirname/
    //Check if file is directory and push its name in stack
    if(fileToISDirecMap.find(fName)!=fileToISDirecMap.end()){
        if(fileToISDirecMap[fName]){
            if(fName.compare(".")==0);//no push pop in case filename is .
            else if(fName.compare("..")==0){//pop out the last directory name from stack
                //stackForwardHistory.push_back(stackBackHistory.back());
                /*string currDir=stackBackHistory.back();
                stackBackHistory.pop_back();
                string parent=stackBackHistory.back();
                stackBackHistory.push_back(currDir);
                stackBackHistory.push_back(parent);*/
                stackBackHistory.push_back(stackBackHistory.back()+fName+"/");//format - ./dirname/
            }
            else//push the complete absolute path of current directory
                stackBackHistory.push_back(stackBackHistory.back()+fName+"/");//format - ./dirname/

            filePath=stackBackHistory.back();//format - ./dirname/
            DIR * pDir = openDirectory(filePath.c_str());
            getFileList(pDir);
            totalfiles=Flist.size();
            //cout<<totalfiles;    
            //cout<<filePath<<endl;
            printFilesWinDependent(0,windLine-1,filePath);
            //cout<<filePath<<endl;
            closedir (pDir);
        }
    }
    //open the file if the regular file is entered
    else{        
        openFile(filePath);
        totalfiles=-1;
    }
    return totalfiles;
}


/*
AUTHOR:         ANUJ
DESCRIPTION:    This function will print the iniitial ls -l kind of list of current directory files
*/

unsigned long initialLS(){
        ioctl(0, TIOCGWINSZ, &w);
        windLine=w.ws_row;
		unsigned long totalfiles;	
        //struct dirent *pDirent;
        DIR *pDir;
        const char* rootPath="./";
        //stach ./
        stackBackHistory.push_back(rootPath);
        pDir = openDirectory(rootPath);
        getFileList(pDir);
        totalfiles=Flist.size();        
        printFilesWinDependent(0,windLine-1,rootPath);
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
            perror(strerror(errno));
            //exit(1);
    }
    return pDir;
}


/*
AUTHOR:         ANUJ
DESCRIPTION:    This function initialize the vector of file names
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
DESCRIPTION:    This function will return the stat of a file.
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
    //SIZE TO BE HUMAN READABLE
    string time=(string(asctime(gmtime(&(info.st_mtime))))).substr(0,24);
    printf("%7s",per.c_str());
    printf("  %7s",pswd->pw_name);
    printf("  %7s",grp->gr_name);
    printf("  %7ld",info.st_size);
    printf("  %s",fName.c_str());
    //commented bcz of spacing issue
    //cout<<per<<"\t"<<pswd->pw_name<<"\t"<<grp->gr_name<<"\t"<<info.st_size<<"\t"<<setw(10)<<fName;
    cout<<endl; 
}


/*

AUTHOR: ANUJ
DESCRIPTION:    This function will print the header information of file listed

*/
void printHead(){
	//cout<<"PERMISSIONS"<<"\t"<<"OWNER"<<"\t"<<"GROUP"<<"\t"<<"SIZE"<<"\t"<<"LAST-MODIFIED"<<"\t\t\t"<<"NAME\n";
}
