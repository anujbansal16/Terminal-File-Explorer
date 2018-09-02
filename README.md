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

* **GOTO**- User can navigate to different directories by proving absolute and relative path

Note:-
*Absolute path starts from / or ~
*Relative path starts from directoryName (not ./DirectoryName)

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



## Author

* **Anuj Bansal** -[Profile](https://www.linkedin.com/in/anujbansal16)

