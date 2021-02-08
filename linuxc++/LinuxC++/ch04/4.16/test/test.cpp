// Copy a file
#include <fstream>      // std::ifstream, std::ofstream

int main() {
	std::ifstream infile("myfile.txt", std::ifstream::binary);
	std::ofstream outfile("new.txt", std::ofstream::binary);

	  // get size of file
	infile.seekg(0, infile.end);
	long size = infile.tellg();
	infile.seekg(0);

	  // allocate memory for file content
	char* buffer = new char[size];

	  // read content of infile
	infile.read(buffer, size);

	  // write to outfile
	outfile.write(buffer, size);

	  // release dynamically-allocated memory
	delete[] buffer;

	outfile.close();
	infile.close();
	return 0;
}