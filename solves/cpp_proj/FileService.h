#pragma once
#include "BST.h"
#include <string>
#include <fstream>
class FileService
{
public:
	FileService(BST& bst,std::string path);
	void read();
private:
	BST* bst = nullptr;
	std::string path;
};

