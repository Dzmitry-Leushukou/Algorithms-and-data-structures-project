#include <iostream>
#include "BST.h"
#include "FileService.h"
#include <chrono>
#include <windows.h>
#include <psapi.h>

int main()
{
	auto start = std::chrono::high_resolution_clock::now();
	PROCESS_MEMORY_COUNTERS_EX pmc_start;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_start, sizeof(pmc_start));
	BST* tree = new BST();
	std::string s = "..\\..\\..\\datasets\\2\\10000_2.txt";
	FileService* fs = new FileService(*tree, s);
	fs->read();


	//std::cout << "Height: " << tree->getHeight() << '\n';
	tree->getHeight();

	int val = tree->getMedianEq();
	//std::cout << "Median element: " << val << '\n';

	tree->rightRemove(val);

	//std::cout << "Left bypass: " <<tree->leftByPass();
	tree->leftByPass();
	auto end = std::chrono::high_resolution_clock::now();
	PROCESS_MEMORY_COUNTERS_EX pmc_end;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_end, sizeof(pmc_end));
	std::chrono::duration<double> diff = end - start;
	std::cout << "Time: " << diff.count() << " s\n";
	SIZE_T peak_mem_bytes = pmc_end.PeakWorkingSetSize;

	std::cout << "Mem: " << peak_mem_bytes / (1024 * 1024) << " MB\n";
}
