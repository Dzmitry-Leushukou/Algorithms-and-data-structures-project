#include "FileService.h"

FileService::FileService(BST& bst, std::string path)
{
	this->bst = &bst;
	this->path = path;
}

void FileService::read()
{
	std::ifstream fin(path);
	std::string s;
	while (std::getline(fin,s))
	{
		if (s.at(0) == '+')
		{
			bst->insert(std::stoi(s.substr(2)));
		}
	}
	fin.close();
}