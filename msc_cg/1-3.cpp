#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <sstream>

// 3차원 점 구조체
struct Point {
    int x = 0;
    int y = 0;
    int z = 0;
};

// 슬롯 구조체 (데이터 및 유효성 여부)
struct Slot {
    Point pt;
    bool occupied = false;
};

// 10개 슬롯을 가지는 리스트
std::vector<Slot> list_data(10);
bool f_active = false; // f 명령어 토글 플래그

// 원점과의 거리 계산
double dist_from_origin(const Point& p) {
    return std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}

// 두 점 사이의 거리 계산
double dist_between(const Point& p1, const Point& p2) {
    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;
    int dz = p1.z - p2.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

// 리스트 상태 출력 함수 (9번: 맨 위, 0번: 맨 아래)
void print_list() {
    std::cout << "\n[현재 리스트 상태]" << std::endl;
    for (int i = 9; i >= 0; --i) {
        std::cout << i;
        if (list_data[i].occupied) {
            std::cout << " " << list_data[i].pt.x << " " << list_data[i].pt.y << " " << list_data[i].pt.z;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// + x y z: 아래에서부터 첫 번째 빈 칸에 저장
void push_top(int x, int y, int z) {
    for (int i = 0; i < 10; ++i) {
        if (!list_data[i].occupied) {
            list_data[i].pt = { x, y, z };
            list_data[i].occupied = true;
            return;
        }
    }
    std::cout << "리스트가 가득 찼습니다!" << std::endl;
}

// -: 맨 위 데이터 삭제
void pop_top() {
    for (int i = 9; i >= 0; --i) {
        if (list_data[i].occupied) {
            list_data[i].occupied = false;
            return;
        }
    }
    std::cout << "삭제할 데이터가 없습니다." << std::endl;
}

// e x y z: 위로 한 칸씩 밀고 0번에 입력
void push_bottom(int x, int y, int z) {
    for (int i = 9; i > 0; --i) {
        list_data[i] = list_data[i - 1];
    }
    list_data[0].pt = { x, y, z };
    list_data[0].occupied = true;
}

// d: 0번 위치 데이터 삭제
void pop_bottom() {
    if (list_data[0].occupied) {
        list_data[0].occupied = false;
    }
    else {
        std::cout << "0번 위치가 이미 비어있습니다." << std::endl;
    }
}

// a: 저장된 점의 개수 출력
void count_points() {
    int count = 0;
    for (int i = 0; i < 10; ++i) {
        if (list_data[i].occupied) count++;
    }
    std::cout << "저장된 점의 개수: " << count << "개" << std::endl;
}

// b: 한 칸씩 내려보내기 (0->9, 1->0, 2->1, ...)
void shift_down() {
    Slot temp = list_data[0];
    for (int i = 0; i < 9; ++i) {
        list_data[i] = list_data[i + 1];
    }
    list_data[9] = temp;
}

// c: 리스트 초기화
void clear_list() {
    for (int i = 0; i < 10; ++i) {
        list_data[i].occupied = false;
    }
    std::cout << "리스트를 비웠습니다." << std::endl;
}

// f: 원점과의 거리를 기준으로 오름차순 정렬하여 출력
void print_sorted_distance() {
    struct SortedItem {
        Point pt;
        double dist;
    };
    std::vector<SortedItem> items;

    for (int i = 0; i < 10; ++i) {
        if (list_data[i].occupied) {
            items.push_back({ list_data[i].pt, dist_from_origin(list_data[i].pt) });
        }
    }

    std::sort(items.begin(), items.end(), [](const SortedItem& a, const SortedItem& b) {
        return a.dist < b.dist;
        });

    std::cout << "\n[원점과의 거리 정렬 출력 (오름차순)]" << std::endl;
    for (int i = 9; i >= 0; --i) {
        std::cout << i;
        if (i < (int)items.size()) {
            std::cout << " " << items[i].pt.x << " " << items[i].pt.y << " " << items[i].pt.z
                << " (거리: " << std::fixed << std::setprecision(2) << items[i].dist << ")";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// g: 가장 먼 두 점, 가장 가까운 두 점 출력
void calculate_point_pairs() {
    std::vector<Point> valid_pts;
    for (int i = 0; i < 10; ++i) {
        if (list_data[i].occupied) {
            valid_pts.push_back(list_data[i].pt);
        }
    }

    if (valid_pts.size() < 2) {
        std::cout << "두 점 이상의 데이터가 필요합니다." << std::endl;
        return;
    }

    double min_d = 1e9, max_d = -1.0;
    Point min_p1, min_p2, max_p1, max_p2;

    for (size_t i = 0; i < valid_pts.size(); ++i) {
        for (size_t j = i + 1; j < valid_pts.size(); ++j) {
            double d = dist_between(valid_pts[i], valid_pts[j]);
            if (d < min_d) {
                min_d = d;
                min_p1 = valid_pts[i];
                min_p2 = valid_pts[j];
            }
            if (d > max_d) {
                max_d = d;
                max_p1 = valid_pts[i];
                max_p2 = valid_pts[j];
            }
        }
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n[두 점 간의 거리 계산 결과]" << std::endl;
    std::cout << "가장 가까운 두 점: (" << min_p1.x << ", " << min_p1.y << ", " << min_p1.z << ") 과 ("
        << min_p2.x << ", " << min_p2.y << ", " << min_p2.z << ") / 거리: " << min_d << std::endl;
    std::cout << "가장 먼 두 점    : (" << max_p1.x << ", " << max_p1.y << ", " << max_p1.z << ") 과 ("
        << max_p2.x << ", " << max_p2.y << ", " << max_p2.z << ") / 거리: " << max_d << std::endl;
}

int main() {
    std::string line;
    while (true) {
        std::cout << "명령어 입력 (+ x y z, -, e x y z, d, a, b, c, f, g, q): ";
        if (!std::getline(std::cin, line) || line.empty()) continue;

        std::stringstream ss(line);
        char cmd;
        ss >> cmd;

        if (cmd == 'q') {
            std::cout << "프로그램을 종료합니다." << std::endl;
            break;
        }

        switch (cmd) {
        case '+': {
            int x, y, z;
            if (ss >> x >> y >> z) push_top(x, y, z);
            print_list();
            break;
        }
        case '-':
            pop_top();
            print_list();
            break;
        case 'e': {
            int x, y, z;
            if (ss >> x >> y >> z) push_bottom(x, y, z);
            print_list();
            break;
        }
        case 'd':
            pop_bottom();
            print_list();
            break;
        case 'a':
            count_points();
            break;
        case 'b':
            shift_down();
            print_list();
            break;
        case 'c':
            clear_list();
            print_list();
            break;
        case 'f':
            f_active = !f_active;
            if (f_active) {
                print_sorted_distance();
            }
            else {
                std::cout << "\n[원래 상태 출력]";
                print_list();
            }
            break;
        case 'g':
            calculate_point_pairs();
            break;
        default:
            std::cout << "알 수 없는 명령어입니다." << std::endl;
            break;
        }
    }
    return 0;
}