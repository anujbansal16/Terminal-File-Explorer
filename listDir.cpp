/***********************************************************

OWNER:  ANUJ BANSAL
ROLLNO. 2018201096
COPYRIGHT PROTECTED
***********************************************************/
#include"listDir.h"
#include <sys/wait.h>
vector<string> Flist;
vector<string> stck;
map <string,bool> fileToISDirecMap;


unsigned long backDirect(){
    unsigned long totalfiles=Flist.size();
    if((stck.back().compare("./")!=0)){
        string path;
        stck.pop_back();    
        for (vector<string>::iterator i= stck.begin(); i != stck.end(); ++i){
            path+=*i;
        }

        DIR * pDir = openDirectory(path.c_str());
        getFileList(pDir);
        totalfiles=Flist.size();
        clearConsole();
        for (vector<string>::iterator iDirentName = Flist.begin(); iDirentName != Flist.end(); ++iDirentName)
        { 
            printStatInfo(getStatInfo(path,*iDirentName),*iDirentName);
        }
        closedir (pDir);

    }
    return totalfiles;
    
}

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


long enterDirectory(unsigned long indexOfFile){
    string s1="";
    //appending strings from stack to get full path
    for (vector<string>::iterator i= stck.begin(); i != stck.end(); ++i){
        s1+=*i;
    }
    string fName=Flist[indexOfFile-1];  //name of the file or directory
    if((stck.back().compare("./")==0)&&fName.compare("..")==0)
        fName=".";
    string filePath=s1+fName+"/";//makeing relative path
    unsigned long totalfiles;

    //open the directory if its entered
    if(fileToISDirecMap.find(fName)!=fileToISDirecMap.end()){
        if(fileToISDirecMap[fName]){
            DIR * pDir = openDirectory(filePath.c_str());
            getFileList(pDir);
            if(fName.compare(".")==0);//no push pop in case filename is .
            else if(fName.compare("..")==0)//pop out the last directory name from stack
                stck.pop_back();
            else//keep pusing directory names
                stck.push_back(fName+"/");
            totalfiles=Flist.size();
            //cout<<totalfiles;    
            clearConsole();
            //cout<<filePath<<endl;
            for (vector<string>::iterator iDirentName = Flist.begin(); iDirentName != Flist.end(); ++iDirentName)
            { 
                printStatInfo(getStatInfo(filePath,*iDirentName),*iDirentName);
            }
            closedir (pDir);
        }
    }
    //open the file if the file is entered
    else{        
        openFile(filePath);
        totalfiles=-1;
    }
    return totalfiles;
}

DIR * openDirectory(const char* s){
	DIR *pDir;
    pDir = opendir(s);
    if (pDir == NULL) {
            perror(strerror(errno));
            //exit(1);
    }
    //cout<<readdir(pDir)->d_name;
    return pDir;
}


void getFileList(DIR * pDir){
    Flist.clear();
    fileToISDirecMap.clear();
    struct dirent *pDirent;
    while ((pDirent = readdir(pDir)) != NULL) {
        string s=pDirent->d_name;
        Flist.push_back(s);                        
    }
    //cout<<Flist.size();
    if(!Flist.empty())
        sort(Flist.begin(),Flist.end());

}

unsigned long initialLS(){

		unsigned long totalfiles;	
        //struct dirent *pDirent;
        DIR *pDir;
        const char* rootPath="./";
        stck.push_back(rootPath);
        pDir = openDirectory(rootPath);
        getFileList(pDir);
        totalfiles=Flist.size();        
        for (vector<string>::iterator iDirentName = Flist.begin(); iDirentName != Flist.end(); ++iDirentName)
        {
 			printStatInfo(getStatInfo(rootPath,*iDirentName),*iDirentName);
        }
        closedir (pDir);
        return totalfiles;

}

struct stat getStatInfo(string rootPath, string fName){
	struct stat info;	
	if(stat((rootPath+fName).c_str(), &info)==-1){
		perror(strerror(errno));
    }
    if(S_ISDIR(info.st_mode))
        fileToISDirecMap[fName]=true;
    return info;
}

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
    cout<<per<<"\t"<<pswd->pw_name<<"\t"<<grp->gr_name<<"\t"<<info.st_size<<"\t"<<(string(asctime(gmtime(&(info.st_mtime))))).substr(0,24)<<"\t"<<fName;
    cout<<endl; 
}

void printHead(){
	//cout<<"PERMISSIONS"<<"\t"<<"OWNER"<<"\t"<<"GROUP"<<"\t"<<"SIZE"<<"\t"<<"LAST-MODIFIED"<<"\t\t\t"<<"NAME\n";
}
