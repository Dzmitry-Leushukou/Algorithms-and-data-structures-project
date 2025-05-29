#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Функция для проверки возможности арбитража
bool hasArbitrage(vector<vector<double>>& rates, int n) {
    // Создаем матрицу для логарифмов курсов (чтобы работать с суммами вместо произведений)
    vector<vector<double>> dist(n, vector<double>(n, -INFINITY));

    // Заполняем матрицу логарифмов курсов
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (rates[i][j] > 0) {
                dist[i][j] = log(rates[i][j]); // log(a[i,j]) для положительных курсов
            }
        }
    }

    // Алгоритм Флойда-Уоршалла для поиска положительного цикла
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] != -INFINITY && dist[k][j] != -INFINITY) {
                    if (dist[i][j] < dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }

    // Проверяем диагональные элементы на положительность
    for (int i = 0; i < n; ++i) {
        if (dist[i][i] > 0) {
            return true; // Найден цикл с произведением курсов > 1
        }
    }

    return false;
}

int main() {
    int n;
    cout << "Введите количество валют: ";
    cin >> n;

    // Ввод матрицы курсов
    vector<vector<double>> rates(n, vector<double>(n));
    cout << "Введите матрицу курсов обмена (" << n << "x" << n << "):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> rates[i][j];
        }
    }

    // Проверка на арбитраж
    if (hasArbitrage(rates, n)) {
        cout << "Арбитраж возможен\n";
    }
    else {
        cout << "Арбитраж невозможен\n";
    }

    return 0;
}