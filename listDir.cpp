/***********************************************************

OWNER:  ANUJ BANSAL
ROLLNO. 2018201096
COPYRIGHT PROTECTED
***********************************************************/
#include"listDir.h"
//map <string,struct stat> mapNameToInfo;	

DIR * openDirectory(const char* s){
	DIR *pDir;
    pDir = opendir(s);
    if (pDir == NULL) {
            perror(strerror(errno));
            exit(1);
    }
    return pDir;
}

unsigned long initialLS(){
		unsigned long totalfiles;	
		vector<string> list;
        struct dirent *pDirent;
        DIR *pDir;
        const char* rootPath="./";
        pDir = openDirectory(rootPath);
        
        while ((pDirent = readdir(pDir)) != NULL) {
            string s=pDirent->d_name;
            list.push_back(s);                        
        }
        if(!list.empty())
        	sort(list.begin(),list.end());
		totalfiles=list.size();
		printHead();
        for (vector<string>::iterator iDirentName = list.begin(); iDirentName != list.end(); ++iDirentName)
        {
 			getStatInfo(rootPath,*iDirentName);
        }
        closedir (pDir);
        return totalfiles;

}

void getStatInfo(string rootPath, string fName){
	struct passwd *pswd;
	struct group  *grp;	
	struct stat info;	
	if(stat((rootPath+fName).c_str(), &info)==-1)
		perror(strerror(errno));
	pswd=getpwuid(info.st_uid);
	grp=getgrgid(info.st_gid);	
 	mode_t mode=info.st_mode;
 	//tab=maxLen-name.size()+offset;
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
