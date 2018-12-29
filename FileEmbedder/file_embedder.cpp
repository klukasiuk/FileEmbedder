#include "file_embedder.h"
#include "filesystem.h"

#include <iostream>
#include <fstream>
#include <iomanip>


using std::cout;
using std::ifstream;
using std::ofstream;
using std::to_string;


string FileEmbedder::fix_name(string filename)
{
	while (filename.find("\\") != string::npos)
	{
		int pos = filename.find("\\");

		filename.replace(pos, 1, "__");				// Convert any slashes to double underscore
	}

	while (filename.find(".") != string::npos)
	{
		int pos = filename.find(".");

		filename.replace(pos, 1, "_");				// Convert any dots to underscore
	}

	return filename;
}

void FileEmbedder::serialize_file(string file)
{
	string fixed_filename = fix_name(file);			// Remove problematic chars from filename

	string header_filename = output_dir + "\\" + output_subdir + "\\" + fixed_filename + ".h";
	string cpp_filename = output_dir + "\\" + output_subdir + "\\" + fixed_filename + ".cpp";
	string main_header_relative_name = output_subdir + "\\" + fixed_filename + ".h";
	string struct_name = "file_" + to_string(file_id);

	int file_size = getFileSize(source_dir + "\\" + file);

	ofstream header_file(header_filename);		// Create header with extern struct

	header_file << "#pragma once\n";
	header_file << "#include \"..\\" + file_description + "\" \n";
	header_file << "\n";
	header_file << "extern EmbeddedFile " + struct_name + ";\n";
	header_file << "\n";

	header_file.close();

	ifstream source_file(source_dir + "\\" + file, ifstream::in | ifstream::binary);
	ofstream cpp_file(cpp_filename);		

	cpp_file << "#include \"" + fixed_filename + ".h" + "\" \n";
	cpp_file << "\n";

	cpp_file << "string " + struct_name + "_filename = \"" + file + "\";\n";
	cpp_file << "int " + struct_name + "_size = " + to_string(file_size) + ";\n";
	cpp_file << "\n";
	cpp_file << "unsigned char " + struct_name + "_data[] =\n";
	cpp_file << "{\n";

	if (source_file.is_open() == false)				// If file failed to open close cpp file
	{
		cpp_file << "0x00\n";
		cpp_file << "\n\};\n";
		cpp_file.close();
		file_headers.push_back(main_header_relative_name);
		file_id++;
		return;
	}

	for(int i = 0; !source_file.eof(); i++)			// Write bytes as hex numbers array
	{
		unsigned char byte;
		byte = source_file.get();

		if (byte < 0)
		{
			cpp_file << "\n";
			break;
		}

		cpp_file << "0x";
		cpp_file << std::setfill('0') << std::setw(2) << std::hex << (int)byte;

		if(i != file_size)
			cpp_file << ",";

		if (i % 12 == 11 && i != file_size)
			cpp_file << "\n";
	}

	cpp_file << "\n};\n\n";

	cpp_file << "EmbeddedFile " + struct_name + "(" + struct_name + "_filename, " + struct_name + "_size, " + struct_name + "_data" + ");\n";
	cpp_file << "\n";

	cpp_file.close();

	file_headers.push_back(main_header_relative_name);

	file_id++;
}

void FileEmbedder::generate_file_structure_file()
{
	string structure_header_filename = output_dir + "\\" + file_description;

	ofstream struct_header_file(structure_header_filename);

	struct_header_file << "#pragma once\n\n";
	struct_header_file << "#include <string>\n";
	struct_header_file << "\n";
	struct_header_file << "using std::string; \n";
	struct_header_file << "\n";

	struct_header_file << "struct EmbeddedFile\n";
	struct_header_file << "{\n";
	struct_header_file << "\tstring filename;\n";
	struct_header_file << "\tlong size;\n";
	struct_header_file << "\tunsigned char * data;\n\n";
	struct_header_file << "\tEmbeddedFile(string name, long filesize, unsigned char * data_pointer)\n";
	struct_header_file << "\t{\n";
	struct_header_file << "\t\tfilename = name;\n";
	struct_header_file << "\t\tsize = filesize;\n";
	struct_header_file << "\t\tdata = data_pointer;\n";
	struct_header_file << "\t}\n";
	struct_header_file << "};\n";


	struct_header_file.close();
}

void FileEmbedder::generate_master_header()
{
	string master_header_filename = output_dir + "\\" + master_header + ".h";

	ofstream master_header_file(master_header_filename);

	master_header_file << "#pragma once\n\n";
	master_header_file << "#include <string>\n";
	master_header_file << "#include <vector>\n";
	master_header_file << "\n";
	master_header_file << "#include \"embedded_file_struct.h\"\n";
	master_header_file << "\n";
	master_header_file << "using std::vector;\n";
	master_header_file << "using std::string;\n\n";
	master_header_file << "class EmbeddedFiles\n";
	master_header_file << "{\n";
	master_header_file << "\tprivate:\n\n";
	master_header_file << "\tvector<EmbeddedFile> files;\n\n";
	master_header_file << "\tint getIndex(string filename);\n";
	master_header_file << "\n\tpublic:\n\n";
	master_header_file << "\t EmbeddedFiles();\n\n";
	master_header_file << "\tvector<string> listFiles();\n";
	master_header_file << "\tlong getSize(string filename);\n";
	master_header_file << "\tunsigned char * getData(string filename);\n";
	master_header_file << "};\n";

	master_header_file.close();
}

void FileEmbedder::generate_master_cpp()
{
	string master_cpp_filename = output_dir + "\\" + master_header + ".cpp";

	ofstream master_cpp_file(master_cpp_filename);

	master_cpp_file << "#include \"" + master_header + ".h\"\n\n";

	for (int i = 0; i < file_headers.size(); i++)
	{
		master_cpp_file << "#include \"" << file_headers[i] << "\" \n";
	}

	master_cpp_file << "\n\n";
	master_cpp_file << "EmbeddedFiles::EmbeddedFiles()\n";
	master_cpp_file << "{\n";

	for (int i = 0; i < file_id; i++)
	{
		master_cpp_file << "\tfiles.push_back(file_" + to_string(i) + ");\n";
	}

	master_cpp_file << "}\n\n";

	master_cpp_file << "int EmbeddedFiles::getIndex(string filename)\n";
	master_cpp_file << "{\n";
	master_cpp_file << "\tint index = -1;\n\n";
	master_cpp_file << "\tfor (int i = 0; i < files.size(); i++)\n";
	master_cpp_file << "\t{\n";
	master_cpp_file << "\t\tif (files[i].filename == filename)\n";
	master_cpp_file << "\t\t\t{\n";
	master_cpp_file << "\t\t\tindex = i;\n";
	master_cpp_file << "\t\t\tbreak;\n";
	master_cpp_file << "\t\t\t}\n";
	master_cpp_file << "\t}\n\n";
	master_cpp_file << "\treturn index;\n";
	master_cpp_file << "}\n\n";

	master_cpp_file << "vector<string> EmbeddedFiles::listFiles()\n";
	master_cpp_file << "{\n";
	master_cpp_file << "\tvector<string> filenames;\n\n";
	master_cpp_file << "\tfor (int i = 0; i < files.size(); i++)\n";
	master_cpp_file << "\t\tfilenames.push_back(files[i].filename);\n\n";
	master_cpp_file << "\treturn filenames;\n";
	master_cpp_file << "}\n\n";

	master_cpp_file << "long EmbeddedFiles::getSize(string filename)\n";
	master_cpp_file << "{\n";
	master_cpp_file << "\tint index = getIndex(filename);\n\n";
	master_cpp_file << "\treturn files[index].size;\n";
	master_cpp_file << "}\n\n";

	master_cpp_file << "unsigned char * EmbeddedFiles::getData(string filename)\n";
	master_cpp_file << "{\n";
	master_cpp_file << "\tint index = getIndex(filename);\n\n";
	master_cpp_file << "\treturn files[index].data;\n";
	master_cpp_file << "}\n\n";

	master_cpp_file.close();
}

void FileEmbedder::process_directory(string dir)
{
	source_dir = dir;

	createDir(output_dir);
	createDir(output_dir + "\\" + output_subdir);

	vector<string> files = getFilesRecursively(dir);

	cout << "Files to serialize : " + to_string(files.size()) + "\n";

	for (int i = 0; i < files.size(); i++)
	{
		cout << "Serializing : " + files[i] + " ...\n";
		serialize_file(files[i]);
	}

	generate_file_structure_file();
	generate_master_header();
	generate_master_cpp();
}
