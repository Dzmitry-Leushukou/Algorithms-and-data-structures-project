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
class BigInt {
public:
    vector<int> digits;

    BigInt() {}
    BigInt(int num) {
        if (num == 0) {
            digits.push_back(0);
        }
        else {
            while (num) {
                digits.push_back(num % 10);
                num /= 10;
            }
        }
    }

    void removeLeadingZeros() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }

    string toString() const {
        if (digits.empty()) return "0";
        string res;
        for (int i = digits.size() - 1; i >= 0; i--) {
            res += char(digits[i] + '0');
        }
        return res;
    }

    BigInt operator+(const BigInt& other) const {
        BigInt res;
        int carry = 0;
        int i = 0;
        int n = digits.size();
        int m = other.digits.size();
        while (i < n || i < m || carry) {
            int a = (i < n) ? digits[i] : 0;
            int b = (i < m) ? other.digits[i] : 0;
            int sum = a + b + carry;
            carry = sum / 10;
            res.digits.push_back(sum % 10);
            i++;
        }
        return res;
    }

    BigInt operator-(const BigInt& other) const {
        BigInt res;
        int borrow = 0;
        int i = 0;
        int n = digits.size();
        int m = other.digits.size();
        while (i < n) {
            int a = digits[i];
            int b = (i < m) ? other.digits[i] : 0;
            int sub = a - borrow - b;
            borrow = 0;
            if (sub < 0) {
                sub += 10;
                borrow = 1;
            }
            res.digits.push_back(sub);
            i++;
        }
        while (res.digits.size() > 1 && res.digits.back() == 0) {
            res.digits.pop_back();
        }
        return res;
    }

    BigInt operator*(const BigInt& other) const {
        int n = digits.size();
        int m = other.digits.size();
        vector<int> temp(n + m, 0);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                temp[i + j] += digits[i] * other.digits[j];
            }
        }
        int carry = 0;
        for (int i = 0; i < n + m; i++) {
            int total = temp[i] + carry;
            carry = total / 10;
            temp[i] = total % 10;
        }
        BigInt res;
        res.digits = temp;
        res.removeLeadingZeros();
        return res;
    }

    BigInt divide(int divisor) const {
        vector<int> temp;
        int remainder = 0;
        for (int i = digits.size() - 1; i >= 0; i--) {
            int current = remainder * 10 + digits[i];
            temp.push_back(current / divisor);
            remainder = current % divisor;
        }
        reverse(temp.begin(), temp.end());
        while (temp.size() > 1 && temp.back() == 0) {
            temp.pop_back();
        }
        BigInt res;
        res.digits = temp;
        return res;
    }
};

void solve(int n, int k)
{
 
        if (n == 0) {
            //cout << 0 << endl;
            return;
        }

        vector<BigInt> pow_k(51, BigInt(0));
        pow_k[0] = BigInt(1);
        for (int i = 1; i <= 50; i++) {
            pow_k[i] = pow_k[i - 1] * BigInt(k);
        }

        vector<BigInt> T(n + 1, BigInt(0));

        for (int s = n - 1; s >= 0; s--) {
            BigInt F_val(0);
            if (s == 0) {
                if (n >= 1) {
                    if (k == 1) {
                        F_val = BigInt(n);
                    }
                    else {
                        BigInt numerator = (pow_k[n] - BigInt(1)) * BigInt(k);
                        F_val = numerator.divide(k - 1);
                    }
                }
            }
            else {
                if (n - s - 1 >= 1) {
                    if (k == 1) {
                        F_val = BigInt(n - s - 1);
                    }
                    else {
                        BigInt numerator = (pow_k[n - s - 1] - BigInt(1)) * BigInt(k);
                        F_val = numerator.divide(k - 1);
                    }
                }
            }

            BigInt sum_terms(0);
            if (s == 0) {
                for (int d = 1; d <= n - 2; d++) {
                    int t = d;
                    BigInt term = pow_k[d] * T[t];
                    sum_terms = sum_terms + term;
                }
            }
            else {
                if (n - s - 3 >= 0) {
                    for (int d = 1; d <= n - s - 3; d++) {
                        int t = s + 1 + d;
                        if (t > n) continue;
                        BigInt term = pow_k[d] * T[t];
                        sum_terms = sum_terms + term;
                    }
                }
            }

            T[s] = F_val + sum_terms;
        }

        //cout << T[0].toString() << endl;

}
int main()
{
    //std::string filename = "..\\..\\..\\datasets\\12\\max.txt";
    //ofstream fout(filename);
    int n = 10;
    int k = 13;
    //fout << n << '\n' << k;
    //fout.close();

    auto start = std::chrono::high_resolution_clock::now();
    PROCESS_MEMORY_COUNTERS_EX pmc_start;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_start, sizeof(pmc_start));
    
    solve(n, k);
    
    auto end = std::chrono::high_resolution_clock::now();
    PROCESS_MEMORY_COUNTERS_EX pmc_end;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc_end, sizeof(pmc_end));
    std::chrono::duration<double> diff = end - start;
    std::cout << "Time: " << diff.count() << " s\n";
    SIZE_T peak_mem_bytes = pmc_end.PeakWorkingSetSize;

    std::cout << "Mem: " << peak_mem_bytes / (1024 * 1024) << " MB\n";
    return 0;
}

