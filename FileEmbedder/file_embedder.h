#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;


class FileEmbedder
{
private:

	string source_dir;
	string output_dir		= "output";									// Output directory
	string output_subdir	= "files_headers";							// Subdirectory with all header converted files
	string file_description	= "embedded_file_struct.h";					// Name for file with structure definition
	string master_header	= "embedded_files";							// Master header name

	vector<string> file_headers;										// Vector with headers names

	int file_id = 0;													// Next file id

	string fix_name(string filename);									// Function for cleaning filenames from \\ or . chars
	void serialize_file(string file);									// Function for converting file to header
	void generate_file_structure_file();								// Function for generating struct describing serialized file
	void generate_master_header();										// Function for generating master header file
	void generate_master_cpp();											// Function for generating master source file

public:

	void process_directory(string dir);									// This function serializes all files in given directory
};