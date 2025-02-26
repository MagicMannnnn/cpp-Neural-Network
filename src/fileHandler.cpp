#include "fileHandler.h"

#include <fstream>
#include <vector>

void FileHandler::saveLine(std::string path, std::string line) {

    std::ofstream file;
    //can't enable exception now because of gcc bug that raises ios_base::failure with useless message
    //file.exceptions(file.exceptions() | std::ios::failbit);
    file.open(path, std::ios::out | std::ios::app);
    //make sure write fails with exception if something is wrong
    file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);

    file << line << std::endl;
}

void FileHandler::clearFile(std::string path) {
	std::ofstream file(path);
	file.close();
}

bool FileHandler::exists(std::string name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}