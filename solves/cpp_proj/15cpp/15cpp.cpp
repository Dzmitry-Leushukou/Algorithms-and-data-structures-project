#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// ������� ��� �������� ����������� ���������
bool hasArbitrage(vector<vector<double>>& rates, int n) {
    // ������� ������� ��� ���������� ������ (����� �������� � ������� ������ ������������)
    vector<vector<double>> dist(n, vector<double>(n, -INFINITY));

    // ��������� ������� ���������� ������
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (rates[i][j] > 0) {
                dist[i][j] = log(rates[i][j]); // log(a[i,j]) ��� ������������� ������
            }
        }
    }

    // �������� ������-�������� ��� ������ �������������� �����
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

    // ��������� ������������ �������� �� ���������������
    for (int i = 0; i < n; ++i) {
        if (dist[i][i] > 0) {
            return true; // ������ ���� � ������������� ������ > 1
        }
    }

    return false;
}

int main() {
    int n;
    cout << "������� ���������� �����: ";
    cin >> n;

    // ���� ������� ������
    vector<vector<double>> rates(n, vector<double>(n));
    cout << "������� ������� ������ ������ (" << n << "x" << n << "):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> rates[i][j];
        }
    }

    // �������� �� ��������
    if (hasArbitrage(rates, n)) {
        cout << "�������� ��������\n";
    }
    else {
        cout << "�������� ����������\n";
    }

    return 0;
}