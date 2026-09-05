#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

const int N = 4; // 행렬의 크기는 항상 4로 고정
std::vector<std::vector<int>> matrix1(N, std::vector<int>(N));
std::vector<std::vector<int>> matrix2(N, std::vector<int>(N));
char command;

// 행렬 출력 함수
void show(const std::vector<std::vector<int>>& matrix) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// 무작위 값(1~9) 설정
void reset_random(std::vector<std::vector<int>>& matrix) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 9 + 1;
        }
    }
}

void setting() {
    reset_random(matrix1);
    reset_random(matrix2);
}

// 행렬 덧셈
std::vector<std::vector<int>> add(const std::vector<std::vector<int>>& m1, const std::vector<std::vector<int>>& m2) {
    std::vector<std::vector<int>> result(N, std::vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i][j] = m1[i][j] + m2[i][j];
        }
    }
    return result;
}

// 행렬 뺄셈
std::vector<std::vector<int>> minus(const std::vector<std::vector<int>>& m1, const std::vector<std::vector<int>>& m2) {
    std::vector<std::vector<int>> result(N, std::vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i][j] = m1[i][j] - m2[i][j];
        }
    }
    return result;
}

// 행렬 곱셈
std::vector<std::vector<int>> multiply(const std::vector<std::vector<int>>& m1, const std::vector<std::vector<int>>& m2) {
    std::vector<std::vector<int>> result(N, std::vector<int>(N, 0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return result;
}

// 재귀를 이용한 행렬식 계산
int determinant(const std::vector<std::vector<int>>& matrix) {
    int n = matrix.size();
    if (n == 1) return matrix[0][0];
    if (n == 2) return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];

    int det = 0;
	//여인수 전개를 이용하여 행렬식 계산
    for (int p = 0; p < n; p++) {
        std::vector<std::vector<int>> subMatrix;
        for (int i = 1; i < n; i++) {
            std::vector<int> row;
            for (int j = 0; j < n; j++) {
                if (j == p) continue;
                row.push_back(matrix[i][j]);
            }
            subMatrix.push_back(row);
        }
        int sign = (p % 2 == 0) ? 1 : -1;
        det += sign * matrix[0][p] * determinant(subMatrix);
    }
    return det;
}

// 전치 행렬
std::vector<std::vector<int>> transpose(const std::vector<std::vector<int>>& matrix) {
    std::vector<std::vector<int>> result(N, std::vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

// 명령어 'e': 행의 최솟값을 해당 행에서 뺌
std::vector<std::vector<int>> subtract_row_min(const std::vector<std::vector<int>>& matrix) {
    std::vector<std::vector<int>> result = matrix;
    for (int i = 0; i < N; i++) {
        int min_val = result[i][0];
        for (int j = 1; j < N; j++) {
            if (result[i][j] < min_val) min_val = result[i][j];
        }
        for (int j = 0; j < N; j++) {
            result[i][j] -= min_val;
        }
    }
    return result;
}

// 명령어 'f': 열의 최댓값을 해당 열에 더함
std::vector<std::vector<int>> add_col_max(const std::vector<std::vector<int>>& matrix) {
    std::vector<std::vector<int>> result = matrix;
    for (int j = 0; j < N; j++) {
        int max_val = result[0][j];
        for (int i = 1; i < N; i++) {
            if (result[i][j] > max_val) max_val = result[i][j];
        }
        for (int i = 0; i < N; i++) {
            result[i][j] += max_val;
        }
    }
    return result;
}

// 명령어 '+', '-': 모듈라 10 연산 (0~9 범위 유지)
void modify_values(std::vector<std::vector<int>>& matrix, int delta) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = (matrix[i][j] + delta + 10) % 10;
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    setting();

    bool e_active = false;
    bool f_active = false;

    std::cout << "행렬 1" << std::endl;
    show(matrix1);
    std::cout << "행렬 2" << std::endl;
    show(matrix2);

    while (true) {
        std::cout << "명령어를 입력하세요 (m, a, d, r, t, e, f, +, -, s, q): ";
        std::cin >> command;

        if (command == 'q') {
            std::cout << "프로그램을 종료합니다." << std::endl;
            break;
        }

        switch (command) {
        case 'm':
            std::cout << "\n[두 행렬의 곱]" << std::endl;
            show(multiply(matrix1, matrix2));
            break;
        case 'a':
            std::cout << "\n[두 행렬의 합]" << std::endl;
            show(add(matrix1, matrix2));
            break;
        case 'd':
            std::cout << "\n[두 행렬의 차 (Matrix1 - Matrix2)]" << std::endl;
            show(minus(matrix1, matrix2));
            break;
        case 'r':
            std::cout << "\n[행렬식(Determinant)]" << std::endl;
            std::cout << "Matrix 1 Determinant: " << determinant(matrix1) << std::endl;
            std::cout << "Matrix 2 Determinant: " << determinant(matrix2) << std::endl;
            break;
        case 't': {
            std::cout << "\n[Matrix 1 전치 행렬]" << std::endl;
            auto t1 = transpose(matrix1);
            show(t1);
            std::cout << "Matrix 1 전치 행렬의 Determinant: " << determinant(t1) << std::endl;

            std::cout << "\n[Matrix 2 전치 행렬]" << std::endl;
            auto t2 = transpose(matrix2);
            show(t2);
            std::cout << "Matrix 2 전치 행렬의 Determinant: " << determinant(t2) << std::endl;
            break;
        }
        case 'e':
            e_active = !e_active;
            if (e_active) {
                std::cout << "\n[각 행의 최솟값을 뺀 결과]" << std::endl;
                std::cout << "<Matrix 1>" << std::endl;
                show(subtract_row_min(matrix1));
                std::cout << "<Matrix 2>" << std::endl;
                show(subtract_row_min(matrix2));
            }
            else {
                std::cout << "\n[원래 행렬 출력]" << std::endl;
                std::cout << "<Matrix 1>" << std::endl;
                show(matrix1);
                std::cout << "<Matrix 2>" << std::endl;
                show(matrix2);
            }
            break;
        case 'f':
            f_active = !f_active;
            if (f_active) {
                std::cout << "\n[각 열의 최댓값을 더한 결과]" << std::endl;
                std::cout << "<Matrix 1>" << std::endl;
                show(add_col_max(matrix1));
                std::cout << "<Matrix 2>" << std::endl;
                show(add_col_max(matrix2));
            }
            else {
                std::cout << "\n[원래 행렬 출력]" << std::endl;
                std::cout << "<Matrix 1>" << std::endl;
                show(matrix1);
                std::cout << "<Matrix 2>" << std::endl;
                show(matrix2);
            }
            break;
        case '+':
            std::cout << "\n[모든 값 +1 (Mod 10)]" << std::endl;
            modify_values(matrix1, 1);
            modify_values(matrix2, 1);
            std::cout << "<Matrix 1>" << std::endl;
            show(matrix1);
            std::cout << "<Matrix 2>" << std::endl;
            show(matrix2);
            break;
        case '-':
            std::cout << "\n[모든 값 -1 (Mod 10)]" << std::endl;
            modify_values(matrix1, -1);
            modify_values(matrix2, -1);
            std::cout << "<Matrix 1>" << std::endl;
            show(matrix1);
            std::cout << "<Matrix 2>" << std::endl;
            show(matrix2);
            break;
        case 's':
            std::cout << "\n[행렬 값 새로 생성]" << std::endl;
            reset_random(matrix1);
            reset_random(matrix2);
            e_active = false;
            f_active = false;
            std::cout << "<Matrix 1>" << std::endl;
            show(matrix1);
            std::cout << "<Matrix 2>" << std::endl;
            show(matrix2);
            break;
        default:
            std::cout << "잘못된 명령어입니다. 다시 입력해 주세요." << std::endl;
            break;
        }
    }
    return 0;
}