#include <iostream>
#include <vector>
#include <climits>
#include <ctime>
#include <random>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <algorithm>
#include <fstream>

int findMaxSquare(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return 0;

    int n = matrix.size();
    int m = matrix[0].size();

    std::vector<std::vector<int>> dp(n, std::vector<int>(m, 0));

    for (int j = 0; j < m; j++) {
        dp[0][j] = matrix[0][j];
    }

    for (int i = 0; i < n; i++) {
        dp[i][0] = matrix[i][0];
    }

    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            if (matrix[i][j] == 1) {
                int mn;
                if(dp[i][j-1]>dp[i-1][j-1])
                    mn = dp[i - 1][j - 1];
                else
                    mn = dp[i][j - 1];

                if (mn > dp[i - 1][j])
                    mn = dp[i - 1][j];
                dp[i][j] = mn + 1;
            }
        }
    }

    int maxSize = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int mx;
            if (maxSize > dp[i][j])
                mx = maxSize;
            else
                mx = dp[i][j];
            maxSize = mx;
        }
    }

    return maxSize;
}
void solve(std::vector<std::vector<int>> matrix)
{
    //std::cout << findMaxSquare(matrix) << std::endl;
    findMaxSquare(matrix);
}

std::mt19937_64 gen(time(0));
int main() {
    std::string filename = "..\\..\\..\\datasets\\14\\3.txt";
    std::ofstream fout(filename);
    std::vector<std::vector<int>> matrix;
    matrix.resize(1e3);
    for (auto& i : matrix) {
        i.resize(1e4);
        for (auto& j : i) 
        {
            if (gen() % 10 < 7)
                j = 1;
            else
                j = 0;
            fout << j << " ";
        }
        fout << '\n';
    }
    fout.close();
    auto start = std::chrono::high_resolution_clock::now();
    PROCESS_MEMORY_COUNTERS_EX pmc_start;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_start, sizeof(pmc_start));
    solve(matrix);
    

    auto end = std::chrono::high_resolution_clock::now();
    PROCESS_MEMORY_COUNTERS_EX pmc_end;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_end, sizeof(pmc_end));
    std::chrono::duration<double> diff = end - start;
    std::cout << "Time: " << diff.count() << " s\n";
    SIZE_T peak_mem_bytes = pmc_end.PeakWorkingSetSize;

    std::cout << "Mem: " << peak_mem_bytes / (1024 * 1024) << " MB\n";
    return 0;
    return 0;
}