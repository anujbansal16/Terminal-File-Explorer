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

unsigned long openFile(string filePath){
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

       
    }
    return -1;
}


unsigned long enterDirectory(unsigned long indexOfFile){
    string s1="./";
    string fName=Flist[indexOfFile-1];
    string filePath=s1+fName;
    //cout<<fName<<endl;
    unsigned long totalfiles;
    if(fileToISDirecMap.find(fName)!=fileToISDirecMap.end()){
        if(fileToISDirecMap[fName]){
            cout<<filePath.c_str()<<endl;
            DIR * pDir = openDirectory(filePath.c_str());
            getFileList(pDir);
            totalfiles=Flist.size();
            //cout<<totalfiles;    
            clearConsole();
            for (vector<string>::iterator iDirentName = Flist.begin(); iDirentName != Flist.end(); ++iDirentName)
            {   //cout<<*iDirentName<<endl;
                printStatInfo(getStatInfo(filePath,*iDirentName),*iDirentName);
            }
            closedir (pDir);
        }
        return totalfiles;
    }
    else{
        openFile(filePath);
    }
    return -1;  
}

DIR * openDirectory(const char* s){
	DIR *pDir;
    pDir = opendir(s);
    if (pDir == NULL) {
            perror(strerror(errno));
            exit(1);
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
        const char* rootPath=".";
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
	if(stat((rootPath+"/"+fName).c_str(), &info)==-1){
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
