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
mt19937_64 gen(time(0));
void solve (string x, string y)
{
    //string x, y;
    //cin >> x >> y;
    int n = x.size();
    int m = y.size();

    vector<vector<int>> dp2(n + 1, vector<int>(m + 1, 0));
    for (int i = n - 1; i >= 0; --i) {
        for (int j = m - 1; j >= 0; --j) {
            if (x[i] == y[j]) {
                dp2[i][j] = 1 + dp2[i + 1][j + 1];
            }
            else {
                dp2[i][j] = max(dp2[i + 1][j], dp2[i][j + 1]);
            }
        }
    }
    int L = dp2[0][0];
    string z = "";
    int cur_i = 0, cur_j = 0;
    int k = L;

    while (k > 0) {
        bool found = false;
        char target;
        int candidate_p = -1, candidate_q = -1;

        for (int p = cur_i; p < n; ++p) {
            if (x[p] == '1') {
                for (int q = cur_j; q < m; ++q) {
                    if (y[q] == '1') {
                        if (dp2[p + 1][q + 1] >= k - 1) {
                            candidate_p = p;
                            candidate_q = q;
                            found = true;
                            target = '1';
                            break;
                        }
                    }
                }
                if (found) break;
            }
        }

        if (!found) {
            for (int p = cur_i; p < n; ++p) {
                if (x[p] == '0') {
                    for (int q = cur_j; q < m; ++q) {
                        if (y[q] == '0') {
                            if (dp2[p + 1][q + 1] >= k - 1) {
                                candidate_p = p;
                                candidate_q = q;
                                found = true;
                                target = '0';
                                break;
                            }
                        }
                    }
                    if (found) break;
                }
            }
        }

        if (found) {
            z += target;
            cur_i = candidate_p + 1;
            cur_j = candidate_q + 1;
            --k;
        }
        else {
            break;
        }
    }

    if (z.empty()) {
        z = "0";
    }

    //cout << z << endl;

}

std::string genTest(int n)
{
    std::string s;
    while (n--)
    {
        s += (char)(gen() % 2 + '0');
    }
    return s;
}
int main() 
{
    int n=4e3; 
    std::string filename = "..\\..\\..\\datasets\\11\\4000.txt";
    ofstream fout(filename);
    std::string s1 = genTest(n);
    std::string s2 = genTest(n);
    fout << s1 << '\n' << s2;
    fout.close();
    auto start = std::chrono::high_resolution_clock::now();
    PROCESS_MEMORY_COUNTERS_EX pmc_start;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_start, sizeof(pmc_start));
    solve(s1, s2);
    auto end = std::chrono::high_resolution_clock::now();
    PROCESS_MEMORY_COUNTERS_EX pmc_end;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_end, sizeof(pmc_end));
    std::chrono::duration<double> diff = end - start;
    std::cout << "Time: " << diff.count() << " s\n";
    SIZE_T peak_mem_bytes = pmc_end.PeakWorkingSetSize;

    std::cout << "Mem: " << peak_mem_bytes / (1024 * 1024) << " MB\n";
    return 0;
}

