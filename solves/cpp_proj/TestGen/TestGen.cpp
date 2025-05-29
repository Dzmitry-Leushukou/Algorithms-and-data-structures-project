#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
int main()
{
	std::vector<int> v;
	for (int i = 1; i <= 100; i++)
	{
		v.push_back(i);
	}
	std::random_shuffle(v.begin(), v.end());
	//std::reverse(v.begin(), v.end());
	std::ofstream fout("..\\..\\..\\datasets\\5\\100.txt");
	for (auto& i : v)
		fout << "+ " << i << "\n";
}