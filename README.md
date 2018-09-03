# Terminal-File-Explorer
This is a console based File Explorer developed in C++ which operates on linux terminal, which uses the operating system's internal functionalities/calls for the implementation of various services.
It provides a interface to the users for managing the files and directories thereby performing various operations on them.
The most common operations, a user can perform on files or groups of files include creating, opening, renaming, moving or copying, deleting and searching for files, and taking snapshot of a directory.

## Built With

* C++

## Getting Started

These instructions will get you a copy of the project up and running on your local machine.

### Prerequisites

Things you need to install the software

* Linux Operating System

### Installing

A step by step series of examples that tell you how to run the File Explorer
	
* Once Downloaded the zip put it in the folder where you want to run the file explorer

Go to the file explorer folder

```
cd Terminal-File-Explorer-master
```

Run the make file to build application

```
make
```

Run the output file

```
./FileExplorer
```

## Features

This file system works in two modes mainly:

### Normal Mode

It is the default mode used to explore the current directory and navigate around in the filesystem.

	Key Events
		UP=> Move cursor up/scroll up
		DOWN=> Move cursor down/scroll down
		LEFT=> Go back to the previously visited Directory
		RIGHT=> Go back to the next visited Directory
		ENTER=> Enter into a directory or opens a file
		BACKSPACE=>Go back to the parent directory
		H/h->Home functionality ,goto root folder(application root)
		:->Switch to command mode
		Q/q: Quit from file explorer (will work in only Normal mode)


### Command Mode

The application can enter in the command mode whenever the “:” (colon) key 
is pressed. Upon entering the command mode the user can enter and executes different commands on files and directories.
	
	Key Events
		ESC=> Switch back to normal mode
		ENTER=>Execute the enetered command
		BACKSPACE=>Clear last character


## Commands Documentation

* **Copy**- User can copy multiple files,folders(recursively), given the file names,folder names and destination directory path.

	* Absolute path starts from / or ~
	* Relative path starts from directoryName (not ./DirectoryName)

Syntax

```
copy <source_file(s)> <destination_directory>
```
source_file(s) must be the file/directory names (ie name of files/folders in present working directory) and not the paths(neither relative nor absolute).

destination_directory may be a abosolute or relative path of destination directory.
Examples:

```
copy file1 file2 dir1 /		#copy files from pwd to application root
copy file2 dir1 ~		#copy files from pwd to application root
copy file2 dir1 .		#copy files from pwd to same pwd with '-copy' apended in the name of files
copy file1 file2 dir1 /dir	#copy files from pwd dir (this is absoulte path)
copy file2 dir1 dir2/dir3	#copy files from pwd to dir3 (this is relative path)
```


* **Move**- User can move multiple files,folders(recursively), given the file names,folder names and destination directory path.

	* Absolute path starts from / or ~
	* Relative path starts from directoryName (not ./DirectoryName)

Syntax

```
move <source_file(s)> <destination_directory>
```
source_file(s) must be the file/directory names (ie name of files/folders in present working directory) and not the paths(neither relative nor absolute).
destination_directory may be a absolute or relative path of destination directory.

Examples:

```
move file1 file2 dir1 /		#move files from pwd to application root
move file2 dir1 ~		#move files from pwd to application root
move file2 dir1 .		#move files from pwd to same pwd with '-copy' apended in the name of files
move file1 file2 dir1 ~/dir	#move files from pwd dir (this is absoulte path)
move file1 file2 dir1 /dir	#move files from pwd dir (this is absoulte path)
move file2 dir1 dir2/dir3	#move files from pwd to dir3 (this is relative path)
```


* **Rename**- User can rename file/folder, given the old file/folder name or path(absolute/relative) and new file/folder name(absoulte/relative).

	* Absolute path starts from / or ~
	* Relative path starts from directoryName (not ./DirectoryName)

Syntax

```
rename <old_filename> <new_filename>
```
Examples

```
rename file1 file2			#rename file1 to file2 in pwd (relative path)
rename dir dir2				#rename dir1 to dir2 in pwd (relative path)
rename /dir1/file1 /dir1/file2		#rename file1 in dir1 to file2 (absolute path)
rename ~/dir1/file1 ~/dir1/file2	#rename file1 in dir1 to file2 (absolute path)
```


* **Create File**- User can create a file given the file name and destination directory path (absolute/relative)

	* Absolute path starts from / or ~
	* Relative path starts from directoryName (not ./DirectoryName)

Syntax

```
create_file <fileName> <destination_path>
```
Examples

```
create_file file1 /		#create file1 in application root (absolute path)
create_file file1 ~		#create file1 in application root (absolute path)
create_file file1 /dir1/dir2	#create file1 in dir2 (absolute path)
create_file file1 ~/dir1/dir2	#create file1 in dir2 (absolute path)
create_file file1 dir3/dir4	#create_file file1 in dir4 (relative path)
create_file file1 .		#create file1 in pwd  (relative path)
create_file file1 dir3		#create_file file1 in dir3 (relative path)
```


* **Create Directory**- User can create a directory given the directory name and destination directory path (absolute/relative)

	* Absolute path starts from / or ~
	* Relative path starts from directoryName (not ./DirectoryName)

Syntax

```
create_dir <directory_Name> <destination_path>
```
Examples

```
create_dir dir1 /		#create dir1 in application root (absolute path)
create_dir dir1 ~		#create dir1 in application root (absolute path)
create_dir dir1 /dir5/dir6	#create dir1 in dir6 (absolute path)
create_dir dir1 ~/dir5/dir6	#create dir1 in dir6 (absolute path)
create_dir dir1 dir3/dir4	#create dir1 in dir4 (relative path)
create_dir dir1 .		#create dir1 in pwd  (relative path)
create_dir dir1 dir3		#create dir1 in dir3 (relative path)
```


* **Delete file**- User can delete a file given the file path (absolute/relative)

	* Absolute path starts from / or ~
	* Relative path starts from directoryName (not ./DirectoryName)

Syntax

```
delete_file <file_path>
```
Examples

```
delete_file /file1		#delete file1 from application root (absolute path)
delete_file ~/file1		#delete file1 from application root (absolute path)
delete_file /dir1/file1		#delete file1 from dir1 (absolute path)
delete_file ~/dir1/dir2/file1	#delete file1 from dir2 (absolute path)
delete_file dir3/dir4/file1	#delete file1 from dir4 (relative path)
delete_file file1		#delete file1 from pwd (relative path)
```


* **Delete Directory**- User can delete a empty directory given the directory path (absolute/relative)

	* Absolute path starts from / or ~
	* Relative path starts from directoryName (not ./DirectoryName)

Syntax

```
create_dir <directory_path>
```
Examples

```
delete_dir /dir1		#delete dir1 from application root (absolute path)
delete_dir ~/dir1		#delete dir1 from application root (absolute path)
delete_dir /dir3/dir1		#delete dir1 from dir3 (absolute path)
delete_dir ~/dir4/dir2/dir1	#delete dir1 from dir2 (absolute path)
delete_dir dir3/dir4/dir1	#delete dir1 from dir4 (relative path)
delete_dir dir1			#delete dir1 from pwd (relative path)
```

* **GOTO**- User can navigate to different directories by providing absolute or relative path.

	* Absolute path starts from / or ~
	* Relative path starts from directoryName (not ./DirectoryName)

Syntax

```
goto <directory_path>
```
Examples:

```
goto /			#goto application root
goto ~			#goto application root
goto /dir/dir2		#absolute path
goto ~/dir/dir2		#absolute path
goto dir/dir2		#relative path from present working directory
```


* **Search file**- User can search any file or folder by giving its name. Searching will start from the current working directory.

Syntax

```
search <filename/FolderName>
```
Examples

```
search file1.txt	#delete dir1 from application root (relative search from pwd )
search dir1.txt		#delete dir1 from application root (relative search from pwd )
```


* **Snapshot**- Given a base directory(absolute/relative) this command should recursively crawl the directory and store the output in dumpfile.Output format should be similar to ls -R

Note:-
* Absolute path starts from / or ~
* Relative path starts from directoryName (not ./DirectoryName)

Syntax

```
snapshot <folder> <dumpfile>
```
Examples

```
snapshot / /dumpfile 		#snapshot application root to dumpfile (absolute path)
snapshot ~/dir ~/dumpfile 	#snapshot dir to dumpfile (absolute path)
snapshot . dumpfile		#snapshot pwd to dumfile (relative path)
snapshot dir/dir2 dumpfile	#snapshot dir2 to dumpfile (relative path)
```


Note:-

* **Absolute path starts from / or ~ **
* **Relative path starts from directoryName (not ./DirectoryName)**

## Author

* **Anuj Bansal** -[Profile](https://www.linkedin.com/in/anujbansal16)

