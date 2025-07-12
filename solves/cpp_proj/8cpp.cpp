#include <iostream>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <fstream>
#include <random>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

struct TestCase {
    vector<int> a;
    vector<int> b;
    int s;
    bool expected;
};

TestCase generate_test(int n, int m, bool want_answer) 
{
    TestCase test;
    test.expected = want_answer;

    random_device rd;
    mt19937 gen(rd());

    if (want_answer) {
        test.a = vector<int>(n, 1);
        test.b = vector<int>(m, 1);
        uniform_int_distribution<int> dist(0, n);
        test.s = dist(gen);
    }
    else {
        test.a = vector<int>(n, 2);
        test.b = vector<int>(m, 2);
        uniform_int_distribution<int> dist(0, n);
        int k = dist(gen);
        test.s = 1 + 2 * k;
    }
    return test;
}

bool solve(vector<int> a, vector<int> b, int s) {
    if (s < 0)
        return false;

    int totalA = accumulate(a.begin(), a.end(), 0);
    if (totalA < s)
        return false;

    int totalB = accumulate(b.begin(), b.end(), 0);

    vector<bool> dpA(totalA + 1, false);
    dpA[0] = true;
    for (int coin : a) {
        for (int j = totalA; j >= coin; j--) {
            if (dpA[j - coin])
                dpA[j] = true;
        }
    }

    vector<bool> dpB(totalB + 1, false);
    dpB[0] = true;
    for (int coin : b) {
        for (int j = totalB; j >= coin; j--) {
            if (dpB[j - coin])
                dpB[j] = true;
        }
    }

    int maxX = min(totalA, s + totalB);
    for (int X = s; X <= maxX; X++) {
        if (!dpA[X])
            continue;
        int change = X - s;
        if (change == 0)
            return true;
        if (change <= totalB && dpB[change])
            return true;
    }

    return false;
}
TestCase read_test(const std::string& filename) {
    TestCase test;
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return test;
    }

    auto read_vector = [&](std::vector<int>& v) {
        size_t size;
        in.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        v.resize(size);
        in.read(reinterpret_cast<char*>(v.data()), size * sizeof(int));
        };

    read_vector(test.a);
    read_vector(test.b);
    in.read(reinterpret_cast<char*>(&test.s), sizeof(int));
    in.read(reinterpret_cast<char*>(&test.expected), sizeof(bool));
    in.close();
    return test;
}
void save_test(const TestCase& test, const string& filename) {
    ofstream out(filename, ios::binary);
    if (!out) {
        cerr << "Cannot open file: " << filename << endl;
        return;
    }

    auto write_vector = [&](const vector<int>& v) {
        size_t size = v.size();
        out.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
        out.write(reinterpret_cast<const char*>(v.data()), size * sizeof(int));
        };

    write_vector(test.a);
    write_vector(test.b);
    out.write(reinterpret_cast<const char*>(&test.s), sizeof(int));
    out.write(reinterpret_cast<const char*>(&test.expected), sizeof(bool));
    out.close();
}
int main()
{
    int n, m;
    n = m = 1e3;
    TestCase test = generate_test(n, m, 0);
    save_test(test, "..\\..\\..\\datasets\\8\\1000_2.bin");

	auto start = std::chrono::high_resolution_clock::now();
	PROCESS_MEMORY_COUNTERS_EX pmc_start;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_start, sizeof(pmc_start));
    TestCase test1 = read_test("..\\..\\..\\datasets\\8\\1000_2.bin");
    bool res = solve(test1.a, test1.b, test1.s);

	auto end = std::chrono::high_resolution_clock::now();
	PROCESS_MEMORY_COUNTERS_EX pmc_end;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_end, sizeof(pmc_end));
	std::chrono::duration<double> diff = end - start;
	std::cout << "Time: " << diff.count() << " s\n";
	SIZE_T peak_mem_bytes = pmc_end.PeakWorkingSetSize;

	std::cout << "Mem: " << peak_mem_bytes / (1024 * 1024) << " MB\n";
}
