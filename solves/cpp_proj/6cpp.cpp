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
	BST* tree1 = new BST();

	FileService* fs = new FileService(*tree1, "..\\..\\..\\datasets\\6\\10000_2.txt");
	fs->read();
	tree1->solveMaxPath();

	auto end = std::chrono::high_resolution_clock::now();
	PROCESS_MEMORY_COUNTERS_EX pmc_end;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_end, sizeof(pmc_end));
	std::chrono::duration<double> diff = end - start;
	std::cout << "Time: " << diff.count() << " s\n";
	SIZE_T peak_mem_bytes = pmc_end.PeakWorkingSetSize;

	std::cout << "Mem: " << peak_mem_bytes / (1024 * 1024) << " MB\n";
}
