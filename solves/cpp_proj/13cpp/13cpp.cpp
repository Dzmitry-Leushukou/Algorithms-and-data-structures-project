#include <iostream>
#include <vector>
#include <climits>
#include <tuple>
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

struct Set {
    int fanta, banana, cake;
    long long cost;
};

void solve(int n, int mf, int mb, int mc, int m,vector<Set>sets)
{


vector<vector<vector<pair<long long, vector<int>>>>> dp(
    mf + 1, vector<vector<pair<long long, vector<int>>>>(
        mb + 1, vector<pair<long long, vector<int>>>(
            mc + 1, { LLONG_MAX, {} })));


dp[0][0][0] = { 0, {} };

for (int i = 0; i < m; ++i) {
    int f = sets[i].fanta, b = sets[i].banana, c = sets[i].cake;
    long long cost = sets[i].cost;

    for (int ff = mf; ff >= 0; --ff) {
        for (int bb = mb; bb >= 0; --bb) {
            for (int cc = mc; cc >= 0; --cc) {
                if (dp[ff][bb][cc].first != LLONG_MAX) {
                    int newf = min(mf, ff + f);
                    int newb = min(mb, bb + b);
                    int newc = min(mc, cc + c);
                    long long newcost = dp[ff][bb][cc].first + cost;

                    if (newcost < dp[newf][newb][newc].first) {
                        dp[newf][newb][newc].first = newcost;
                        dp[newf][newb][newc].second = dp[ff][bb][cc].second;
                        dp[newf][newb][newc].second.push_back(i + 1);
                    }
                }
            }
        }
    }
}

if (dp[mf][mb][mc].first == LLONG_MAX) {
    //cout << -1 << endl;
    //return 0;
    return;
}

long long mincost = dp[mf][mb][mc].first;
long long mincontribution = (mincost + n - 1) / n;
//cout << mincontribution << endl;

for (int set : dp[mf][mb][mc].second) {
    //cout << set << " ";
}
//cout << endl;
}

int main()
{
    std::string filename = "..\\..\\..\\datasets\\13\\1.txt";
    ofstream fout(filename);
    mt19937 rng(time(0));
    uniform_int_distribution<int> dist_items(0, 10); // mf, mb, mc: 0..10
    uniform_int_distribution<int> dist_set(0, 5); // Кол-во в наборах: 0..5
    uniform_int_distribution<int> dist_cost(1, 100); // Стоимость набора: 1..100
    std::vector<Set>sets;
    int n = 1;
    int mf = 10; 
    int mb = 10; 
    int mc = 10; 
    int m = 10; 

    fout << n << " " << mf << " " << mb << " " << mc << " " << m << endl;
    for (int i = 0; i < m; ++i) {
        int fanta = dist_set(rng);
        int banana = dist_set(rng);
        int cake = dist_set(rng);
        int cost = dist_cost(rng);
        fout << fanta << " " << banana << " " << cake << " " << cost << endl;
        sets.push_back({ fanta,banana,cake,cost });
    }
    fout.close();
    auto start = std::chrono::high_resolution_clock::now();
    PROCESS_MEMORY_COUNTERS_EX pmc_start;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_start, sizeof(pmc_start));

    solve(n, mf,mb,mc,m,sets);

    auto end = std::chrono::high_resolution_clock::now();
    PROCESS_MEMORY_COUNTERS_EX pmc_end;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_end, sizeof(pmc_end));
    std::chrono::duration<double> diff = end - start;
    std::cout << "Time: " << diff.count() << " s\n";
    SIZE_T peak_mem_bytes = pmc_end.PeakWorkingSetSize;

    std::cout << "Mem: " << peak_mem_bytes / (1024 * 1024) << " MB\n";
    return 0;
}