#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <windows.h>

// 콘솔 색상 변경 함수
void set_color(WORD color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 텍스트 출력 함수
void print_color(const std::string& text, WORD color = 7) {
    set_color(color);
    std::cout << text;
    set_color(7);
}

// 사각형 구조체
struct Square {
    int min_x;
    int max_x;
    int min_y;
    int max_y;
};

int initial_xsize = 30;
int initial_ysize = 30;
int board_xsize = 30;
int board_ysize = 30;

std::vector<Square> squares;

// 보드 이탈 시 반대편 위치 계산 함수
int wrap(int val, int max_limit) {
    return (val % max_limit + max_limit) % max_limit;
}

// 좌표 점유 여부 판별 함수
bool is_in_square(const Square& sq, int x, int y) {
    int width = sq.max_x - sq.min_x + 1;
    int height = sq.max_y - sq.min_y + 1;

    if (width >= board_xsize || height >= board_ysize) return true;

    bool inside_x = false;
    int start_x = wrap(sq.min_x, board_xsize);
    int end_x = wrap(sq.max_x, board_xsize);
    if (start_x <= end_x) {
        inside_x = (x >= start_x && x <= end_x);
    }
    else {
        inside_x = (x >= start_x || x <= end_x);
    }

    bool inside_y = false;
    int start_y = wrap(sq.min_y, board_ysize);
    int end_y = wrap(sq.max_y, board_ysize);
    if (start_y <= end_y) {
        inside_y = (y >= start_y && y <= end_y);
    }
    else {
        inside_y = (y >= start_y || y <= end_y);
    }

    return inside_x && inside_y;
}

// 도형 이동 함수
void move_square(Square& sq, int dx, int dy) {
    sq.min_x = wrap(sq.min_x + dx, board_xsize);
    sq.max_x = wrap(sq.max_x + dx, board_xsize);
    sq.min_y = wrap(sq.min_y + dy, board_ysize);
    sq.max_y = wrap(sq.max_y + dy, board_ysize);
}

// 면적 계산 함수
int get_area(const Square& sq) {
    int width = (sq.max_x - sq.min_x + 1);
    int height = (sq.max_y - sq.min_y + 1);
    return width * height;
}

// 크기 변경 함수
void resize_square(Square& sq, int dw, int dh) {
    int current_w = sq.max_x - sq.min_x + 1;
    int current_h = sq.max_y - sq.min_y + 1;

    if (current_w + dw >= 1 && current_w + dw <= board_xsize) {
        sq.max_x += dw;
    }
    if (current_h + dh >= 1 && current_h + dh <= board_ysize) {
        sq.max_y += dh;
    }
}

// 보드 상태 출력 함수
void show_board() {
    std::cout << "\n현재 보드 크기: " << board_xsize << " x " << board_ysize << std::endl;
    for (int y = 0; y < board_ysize; y++) {
        for (int x = 0; x < board_xsize; x++) {
            bool in_sq1 = (squares.size() > 0) && is_in_square(squares[0], x, y);
            bool in_sq2 = (squares.size() > 1) && is_in_square(squares[1], x, y);

            if (in_sq1 && in_sq2) {
                print_color("# ", 12);
            }
            else if (in_sq1) {
                print_color("0 ");
            }
            else if (in_sq2) {
                print_color("x ");
            }
            else {
                print_color(". ");
            }
        }
        std::cout << std::endl;
    }
}

// 좌표 입력 받는 함수
void input_coordinates() {
    squares.clear();
    int x1, y1, x2, y2;
    std::cout << "첫번째 도형 좌표 입력 (x1 y1 x2 y2): ";
    std::cin >> x1 >> y1 >> x2 >> y2;
    squares.push_back({ x1, y1, x2, y2 });

    std::cout << "두번째 도형 좌표 입력 (x3 y3 x4 y4): ";
    std::cin >> x1 >> y1 >> x2 >> y2;
    squares.push_back({ x1, y1, x2, y2 });
}

int main() {
    input_coordinates();
    std::string line;
    std::cin.ignore();

    while (true) {
        show_board();
        std::cout << "\n명령어 입력 (예: x 1, X 2, s 1, S 2, i 1, J 2, b, c, d, r, q): ";

        if (!std::getline(std::cin, line) || line.empty()) continue;
        std::stringstream ss(line);

        char command;
        ss >> command;

        if (command == 'q') {
            std::cout << "프로그램을 종료합니다." << std::endl;
            return 0;
        }

        // 전체 명령 처리
        if (command == 'b') {
            std::cout << "\n도형 면적 정보" << std::endl;
            std::cout << "1번 도형 면적: " << get_area(squares[0]) << std::endl;
            std::cout << "2번 도형 면적: " << get_area(squares[1]) << std::endl;
            continue;
        }
        else if (command == 'c') {
            if (board_xsize < initial_xsize + 10) {
                board_xsize++;
                board_ysize++;
                std::cout << "보드 크기를 늘렸습니다. 현재 크기: " << board_xsize << "x" << board_ysize << std::endl;
            }
            else {
                std::cout << "최대 확장 한계에 도달했습니다." << std::endl;
            }
            continue;
        }
        else if (command == 'd') {
            if (board_xsize > initial_xsize - 20) {
                bool can_reduce = true;
                for (const auto& sq : squares) {
                    if (sq.max_x >= board_xsize - 1 || sq.max_y >= board_ysize - 1) {
                        can_reduce = false;
                        break;
                    }
                }
                if (can_reduce) {
                    board_xsize--;
                    board_ysize--;
                    std::cout << "보드 크기를 줄였습니다. 현재 크기: " << board_xsize << "x" << board_ysize << std::endl;
                }
                else {
                    std::cout << "도형이 보드 경계에 있어 크기를 줄일 수 없습니다." << std::endl;
                }
            }
            else {
                std::cout << "최대 축소 한계에 도달했습니다." << std::endl;
            }
            continue;
        }
        else if (command == 'r') {
            board_xsize = initial_xsize;
            board_ysize = initial_ysize;
            input_coordinates();
            std::cin.ignore();
            continue;
        }

        // 대상 도형 번호 입력 확인
        int target_idx = 0;
        if (!(ss >> target_idx) || (target_idx != 1 && target_idx != 2)) {
            std::cout << "잘못된 입력입니다. 명령어 뒤에 도형 번호를 입력해야 합니다." << std::endl;
            continue;
        }

        int idx = target_idx - 1;

        // 개별 도형 변환 처리
        switch (command) {
        case 'x': move_square(squares[idx], 1, 0); break;
        case 'X': move_square(squares[idx], -1, 0); break;
        case 'y': move_square(squares[idx], 0, 1); break;
        case 'Y': move_square(squares[idx], 0, -1); break;
        case 's': resize_square(squares[idx], -1, -1); break;
        case 'S': resize_square(squares[idx], 1, 1); break;
        case 'i': resize_square(squares[idx], 1, 0); break;
        case 'I': resize_square(squares[idx], -1, 0); break;
        case 'j': resize_square(squares[idx], 0, 1); break;
        case 'J': resize_square(squares[idx], 0, -1); break;
        case 'a': resize_square(squares[idx], 1, -1); break;
        case 'A': resize_square(squares[idx], -1, 1); break;
        default:
            std::cout << "잘못된 명령어입니다." << std::endl;
            break;
        }
    }
    return 0;
}