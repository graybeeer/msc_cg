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

// 슬롯 구조체 
struct Slot {
    Point pt;
    bool full = false;
};

// 10개 슬롯을 가지는 리스트
int list_size = 10;
std::vector<Slot> list_data(list_size);
bool bool_f = false; // f 명령어 bool

// 원점과의 거리 
double dist_from_origin(const Point& p) {
    return std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}

// 두 점 사이의 거리 
double dist_between(const Point& p1, const Point& p2) {
    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;
    int dz = p1.z - p2.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

// 리스트 상태 출력 (9번: 맨 위, 0번: 맨 아래)
void show_list() {
    std::cout << "\n현재 점 리스트: " << std::endl;
    for (int i = list_size - 1; i >= 0; i--) {
        std::cout << i << ": ";
        if (list_data[i].full) {
            std::cout << " " << list_data[i].pt.x << " " << list_data[i].pt.y << " " << list_data[i].pt.z;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// + x y z: 아래에서부터 첫 번째 빈 칸에 저장
void push_top(int x, int y, int z) {
    for (int i = 0; i < list_size; i++) {
        if (!list_data[i].full) {
            list_data[i].pt = { x, y, z };
            list_data[i].full = true;
            return;
        }
    }
    std::cout << "리스트가 전부 찼음" << std::endl;
}

// -: 맨 위 데이터 삭제
void pop_top() {
    for (int i = list_size - 1; i >= 0; i--) {
        if (list_data[i].full) {
			list_data[i].pt = { 0, 0, 0 };
            list_data[i].full = false;
            return;
        }
    }
    std::cout << "삭제할 데이터가 없음" << std::endl;
}

// e x y z: 위로 한 칸씩 밀고 0번에 입력
void push_bottom(int x, int y, int z) {
    int empty_idx = -1;

    // 가장 처음 나타나는 빈칸(중간 빈칸)의 위치를 찾음
    for (int i = 0; i < list_size; i++) {
        if (!list_data[i].full) {
            empty_idx = i;
            break;
        }
    }

    if (empty_idx == -1) {
        empty_idx = list_size - 1;
    }

    for (int i = empty_idx; i > 0; i--) {
        list_data[i] = list_data[i - 1];
    }

    list_data[0].pt = { x, y, z };
    list_data[0].full = true;
}

// d: 맨 아래 위치 데이터 삭제
void pop_bottom() {
    for (int i = 0; i < list_size; i++) {
        if (list_data[i].full) {
			list_data[i].pt = { 0, 0, 0 };
            list_data[i].full = false;
            return;
        }
    }
    std::cout << "삭제할 데이터가 없음" << std::endl;
}

// a: 저장된 점 개수 출력
void count_points() {
    int count = 0;
    for (int i = 0; i < list_size; i++) {
        if (list_data[i].full) count++;
    }
    std::cout << "저장된 점 개수: " << count << "개" << std::endl;
}

// b: 한 칸씩 내려보내기 (0->9, 1->0, 2->1, ...), 모듈러 연산
void shift_down() {
    Slot temp = list_data[0];
    for (int i = 0; i < list_size - 1; i++) {
        list_data[i] = list_data[i + 1];
    }
    list_data[list_size - 1] = temp;
}

// c: 리스트 초기화
void clear_list() {
    for (int i = 0; i < list_size; i++) {
        list_data[i].pt = { 0, 0, 0 };
        list_data[i].full = false;
    }
    std::cout << "리스트를 비웠습니다." << std::endl;
}

// f: 원점과의 거리를 기준으로 오름차순 정렬하여 출력
void print_sorted_distance() {
	struct SortedDist { //거리를 기준으로 정렬할 구조체
        Point pt;
        double dist;
    };
    std::vector<SortedDist> dists;

    for (int i = 0; i < list_size; ++i) {
        if (list_data[i].full) {
            dists.push_back({ list_data[i].pt, dist_from_origin(list_data[i].pt) });
        }
    }

	std::sort(dists.begin(), dists.end(), [](const SortedDist& a, const SortedDist& b) { //거리 기준 오름차순 정렬
        return a.dist < b.dist;
        });

    std::cout << "\n원점과의 거리 정렬: " << std::endl;
    for (int i = list_size - 1; i >= 0; i--) {
        std::cout << i;
        if (i < (int)dists.size()) {
            std::cout << " " << dists[i].pt.x << " " << dists[i].pt.y << " " << dists[i].pt.z
                << " (거리: " << std::fixed << std::setprecision(2) << dists[i].dist << ")";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// g: 가장 먼 두 점, 가장 가까운 두 점 출력
void calculate_point_pairs() {
	int valid_count = 0; //현재 저장된 점 개수
    for (int i = 0; i < list_size; i++) {
        if (list_data[i].full) {
            valid_count++;
        }
    }

    if (valid_count < 2) {
        std::cout << "두 점 이상이 필요함" << std::endl;
        return;
    }

    float min_d = 100000000, max_d = -1;
    Point min_p1, min_p2, max_p1, max_p2;

    for (size_t i = 0; i < valid_count; ++i) {
        for (size_t j = i + 1; j < valid_count; ++j) {
            float d = dist_between(list_data[i].pt, list_data[j].pt);
            if (d < min_d) {
                min_d = d;
                min_p1 = list_data[i].pt;
                min_p2 = list_data[j].pt;
            }
            if (d > max_d) {
                max_d = d;
                max_p1 = list_data[i].pt;
                max_p2 = list_data[j].pt;
            }
        }
    }

    std::cout << "\n두 점 간의 거리: " << std::endl;
    std::cout << "가장 가까운 두 점: (" << min_p1.x << ", " << min_p1.y << ", " << min_p1.z << ") 과 ("
        << min_p2.x << ", " << min_p2.y << ", " << min_p2.z << ") 의 거리: " << min_d << std::endl;
    std::cout << "가장 먼 두 점    : (" << max_p1.x << ", " << max_p1.y << ", " << max_p1.z << ") 과 ("
        << max_p2.x << ", " << max_p2.y << ", " << max_p2.z << ") 의 거리: " << max_d << std::endl;
}

int main() {
    std::string line;
    while (true) {
        std::cout << "명령어 입력 (+ x y z, -, e x y z, d, a, b, c, f, g, q): ";
        if (!std::getline(std::cin, line) || line.empty()) continue;

        std::stringstream ss(line);
        char command;
        ss >> command;

        switch (command) {
        case '+': {
            int x, y, z;
            if (ss >> x >> y >> z) {
                push_top(x, y, z);
                show_list();
            }
            break;
        }
        case '-':
            pop_top();
            show_list();
            break;
        case 'e': {
            int x, y, z;
            if (ss >> x >> y >> z) {
                push_bottom(x, y, z);
                show_list();
            }
            break;
        }
        case 'd':
            pop_bottom();
            show_list();
            break;
        case 'a':
            count_points();
            break;
        case 'b':
            shift_down();
            show_list();
            break;
        case 'c':
            clear_list();
            show_list();
            break;
        case 'f':
            bool_f = !bool_f;
            if (bool_f) {
                print_sorted_distance();
            }
            else {
                std::cout << "\n원래 상태 출력: ";
                show_list();
            }
            break;
        case 'g':
            calculate_point_pairs();
            break;
        case 'q':
			std::cout << "프로그램을 종료합니다." << std::endl;
			return 0;
        default:
            std::cout << "잘못된 명령어입니다." << std::endl;
            break;
        }
    }
    return 0;
}