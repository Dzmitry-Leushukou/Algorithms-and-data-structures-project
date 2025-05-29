#include <iostream>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <random>
#include <string>
#include <iomanip>
#include <sstream>
#include <cmath>

using namespace std;

// Функция для получения текущего потребления памяти
size_t get_current_memory() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
    return 0;
}

// Функция для вычисления минимального количества удалений
int minDeletionsForIncreasingSequence(const vector<int>& A) {
    if (A.empty()) return 0;
    vector<int> tail;
    for (int x : A) {
        auto it = lower_bound(tail.begin(), tail.end(), x);
        if (it == tail.end()) {
            tail.push_back(x);
        }
        else {
            *it = x;
        }
    }
    return A.size() - tail.size();
}

// Генератор тестовых данных
vector<int> generate_test(int n, int type) {
    vector<int> A(n);
    random_device rd;
    mt19937 gen(rd());

    if (type == 0) { // Случайная последовательность
        uniform_int_distribution<int> dist(0, n * 10);
        for (int i = 0; i < n; i++) {
            A[i] = dist(gen);
        }
    }
    else if (type == 1) { // Строго возрастающая
        for (int i = 0; i < n; i++) {
            A[i] = i;
        }
    }
    else if (type == 2) { // Строго убывающая
        for (int i = 0; i < n; i++) {
            A[i] = n - i;
        }
    }
    else if (type == 3) { // Почти отсортированная
        for (int i = 0; i < n; i++) {
            A[i] = i;
        }
        uniform_int_distribution<int> idx_dist(0, n - 1);
        for (int i = 0; i < n / 10; i++) {
            int i1 = idx_dist(gen);
            int i2 = idx_dist(gen);
            swap(A[i1], A[i2]);
        }
    }
    else if (type == 4) { // Много повторений
        uniform_int_distribution<int> dist(0, 100);
        for (int i = 0; i < n; i++) {
            A[i] = dist(gen);
        }
    }
    return A;
}

// Функция для принудительного освобождения памяти
void clean_memory() {
    vector<int> temp;
    temp.swap(temp);
}

int main() {
    const vector<int> sizes = { 10000, 100000, 1000000, 10000000 };
    const vector<int> types = { 0, 1, 2, 3, 4 };

    cout << "Testing minDeletionsForIncreasingSequence algorithm\n";
    cout << "-------------------------------------------------------------------\n";
    cout << setw(12) << "Size" << setw(8) << "Type"
        << setw(15) << "Time (ms)" << setw(15) << "Memory (MB)"
        << setw(20) << "Memory per element" << "\n";
    cout << "-------------------------------------------------------------------\n";

    for (int n : sizes) {
        for (int type : types) {
            // Освобождаем память перед тестом
            clean_memory();

            // Генерируем тестовые данные
            vector<int> A = generate_test(n, type);

            // Получаем начальное состояние памяти
            size_t mem_before = get_current_memory();

            // Запускаем алгоритм
            auto start_time = chrono::high_resolution_clock::now();
            int result = minDeletionsForIncreasingSequence(A);
            auto end_time = chrono::high_resolution_clock::now();

            // Получаем конечное состояние памяти
            size_t mem_after = get_current_memory();

            // Вычисляем использование памяти
            size_t memory_used_bytes = (mem_after > mem_before) ? (mem_after - mem_before) : 0;
            double memory_used_mb = static_cast<double>(memory_used_bytes) / (1024 * 1024);
            double bytes_per_element = (n > 0) ? static_cast<double>(memory_used_bytes) / n : 0;

            // Рассчитываем время выполнения
            auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

            // Выводим результаты
            cout << setw(12) << n
                << setw(8) << type
                << setw(15) << duration.count()
                << setw(15) << fixed << setprecision(2) << memory_used_mb
                << setw(20) << fixed << setprecision(2) << bytes_per_element << "\n";

            // Освобождаем память перед следующим тестом
            vector<int>().swap(A);
            clean_memory();
        }
    }

    cout << "-------------------------------------------------------------------\n";
    cout << "Testing completed!\n";

    return 0;
}