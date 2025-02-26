#pragma once

#include <string>

class FileHandler {
public:
	static void clearFile(std::string path);
	static bool exists(std::string path);
	static void saveLine(std::string path, std::string line);
	
};