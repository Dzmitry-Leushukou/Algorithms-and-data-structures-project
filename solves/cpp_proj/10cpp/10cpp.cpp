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
#include <iostream>
#include <vector>
#include <map>
#include <climits>
#include <algorithm>
#include <cmath>
#include <unordered_map>

using namespace std;

vector<long long> find_all_divisors_in_range(long long n, long long b, long long c, bool include_1) {
    vector<long long> divisors;
    if (n == 1) {
        if (include_1 && b <= 1 && 1 <= c) {
            divisors.push_back(1);
        }
        return divisors;
    }
    if (!include_1) {
        if (b <= 1 && 1 <= c) {
            // But we avoid 1 for n>1
        }
    }

    for (long long i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            if (i >= b && i <= c) {
                if (include_1 || i != 1) {
                    divisors.push_back(i);
                }
            }
            long long j = n / i;
            if (j != i) {
                if (j >= b && j <= c) {
                    if (include_1 || j != 1) {
                        divisors.push_back(j);
                    }
                }
            }
        }
    }
    return divisors;
}

unordered_map<long long, int> memo;
long long global_b, global_c;

int solve(long long temp) {
    if (memo.find(temp) != memo.end()) {
        return memo[temp];
    }
    if (temp == 1) {
        if (global_b <= 1 && 1 <= global_c) {
            return 0;
        } else {
            return -1;
        }
    }
    int best = INT_MAX;
    if (temp >= global_b && temp <= global_c) {
        best = 1;
    }
    vector<long long> divisors = find_all_divisors_in_range(temp, global_b, global_c, false);
    for (size_t i = 0; i < divisors.size(); i++) {
        long long d = divisors[i];
        if (d == temp) {
            continue;
        }
        if (temp % d != 0) {
            continue;
        }
        long long next = temp / d;
        int next_ans = solve(next);
        if (next_ans != -1) {
            if (1 + next_ans < best) {
                best = 1 + next_ans;
            }
        }
    }
    if (best == INT_MAX) {
        memo[temp] = -1;
        return -1;
    }
    memo[temp] = best;
    return best;
}

int main() {
    long long a, b, c;
    a = 1e18;
    b = 2;
    c = a-1;
    auto start = std::chrono::high_resolution_clock::now();
    PROCESS_MEMORY_COUNTERS_EX pmc_start;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_start, sizeof(pmc_start));

    global_b = b;
    global_c = c;

    if (a == 1) {
        if (b <= 1 && 1 <= c) {
           // cout << 1 << endl;
        } else {
            //cout << "No solution" << endl;
        }
        auto end = std::chrono::high_resolution_clock::now();
        PROCESS_MEMORY_COUNTERS_EX pmc_end;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_end, sizeof(pmc_end));
        std::chrono::duration<double> diff = end - start;
        std::cout << "Time: " << diff.count() << " s\n";
        SIZE_T peak_mem_bytes = pmc_end.PeakWorkingSetSize;

        std::cout << "Mem: " << peak_mem_bytes / (1024 * 1024) << " MB\n";
        return 0;
    }
    if (b > c) {
        //cout << "No solution" << endl;
        auto end = std::chrono::high_resolution_clock::now();
        PROCESS_MEMORY_COUNTERS_EX pmc_end;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_end, sizeof(pmc_end));
        std::chrono::duration<double> diff = end - start;
        std::cout << "Time: " << diff.count() << " s\n";
        SIZE_T peak_mem_bytes = pmc_end.PeakWorkingSetSize;

        std::cout << "Mem: " << peak_mem_bytes / (1024 * 1024) << " MB\n";
        return 0;
    }
    if (b > a) {
        //cout << "No solution" << endl;
        auto end = std::chrono::high_resolution_clock::now();
        PROCESS_MEMORY_COUNTERS_EX pmc_end;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_end, sizeof(pmc_end));
        std::chrono::duration<double> diff = end - start;
        std::cout << "Time: " << diff.count() << " s\n";
        SIZE_T peak_mem_bytes = pmc_end.PeakWorkingSetSize;

        std::cout << "Mem: " << peak_mem_bytes / (1024 * 1024) << " MB\n";
        return 0;
    }

    memo.clear();
    int ans = solve(a);
    if (ans == -1) {
        //cout << "No solution" << endl;
    } else {
        //cout << ans << endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    PROCESS_MEMORY_COUNTERS_EX pmc_end;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_end, sizeof(pmc_end));
    std::chrono::duration<double> diff = end - start;
    std::cout << "Time: " << diff.count() << " s\n";
    SIZE_T peak_mem_bytes = pmc_end.PeakWorkingSetSize;

    std::cout << "Mem: " << peak_mem_bytes / (1024 * 1024) << " MB\n";
    return 0;
}

