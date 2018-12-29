#include <iostream>
#include <fstream>

#include "embedded_files.h"

using std::cout;
using std::ofstream;

int main()
{
	cout << "File embedder test - Self Unpacking Exe\n\n";

	EmbeddedFiles embedded;

	vector<string> files_list = embedded.listFiles();

	cout << "Files list : \n";

	for (int i = 0; i < files_list.size(); i++)
	{
		cout << "\t\t\t" + files_list[i] + "\n";
	}

	cout << "\n";

	for (int i = 0; i < files_list.size(); i++)
	{
		cout << "Unpacking file : " + files_list[i] + "\n";

		int size = embedded.getSize(files_list[i]);

		unsigned char * data = embedded.getData(files_list[i]);

		ofstream save(files_list[i], ofstream::out | ofstream::binary);

		for (int z = 0; z < size; z++)
		{
			save << data[z];
		}

		save.close();
	}

	cout << "\nAll done :)";

	getchar();

	return 0;
}