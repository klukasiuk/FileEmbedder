#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;


long getFileSize(string filename);														// Return size of file in bytes

string getLocalPath();																	// Return path for folder containing this program

vector<string> getDirectoryFilenames(string  directoryName);							// Lists all files in given folder

vector<string> getSubDirectories(string  directoryName);								// Lists all sub directories in directory

vector<string> getFilesRecursively(string  directoryName, int level = 0);				// List all files in folder and in subfolders

vector<string> getSubDirectoriesRecursively(string  directoryName, int level = 0);		// Lists all directories in folder and in subfolders

void createDir(string name);															// Creates new dir in local dir
