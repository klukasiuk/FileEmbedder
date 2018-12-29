#include <iostream>

#include "filesystem.h"
#include "file_embedder.h"


using std::cout;


int main()
{
	cout << "FileEmbedder ver 0.1\n\n";

	string local_path = getLocalPath();						// Local folder

	FileEmbedder embedder;									// Embedder object

	string test_dir = local_path + "test";					// Default folder is named test

	cout << "Serializing deafult folder : " + test_dir + "\n\n";

	embedder.process_directory(test_dir);					// Process all files with embedder

	cout << "\nAll done\n";
	cout << "Press any key\n";

	getchar();

	return 0;
}