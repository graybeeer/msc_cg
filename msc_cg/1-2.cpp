#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <windows.h>

// 콘솔 색상 변경 함수
void set_color(WORD color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 텍스트 출력 함수 (색상 초기화 포함)
void print_color(const std::string& text, WORD color) {
    set_color(color);
    std::cout << text;
    set_color(7); // 기본 색상(밝은 회색)으로 복원
}

// 연속된 공백을 단일 공백으로 변환
std::string normalize_spaces(const std::string& input) {
    std::string result;
    bool in_space = false;
    for (char c : input) {
        if (std::isspace(static_cast<unsigned char>(c))) {
            if (!in_space) {
                result += ' ';
                in_space = true;
            }
        }
        else {
            result += c;
            in_space = false;
        }
    }
    // 앞뒤 공백 제거
    if (!result.empty() && result.front() == ' ') result.erase(result.begin());
    if (!result.empty() && result.back() == ' ') result.pop_back();
    return result;
}

// 문장 출력 함수
void print_lines(const std::vector<std::string>& lines) {
    for (const auto& line : lines) {
        std::cout << line << std::endl;
    }
}

// 단어 개수 세기 (공백/star 구분)
int count_words(const std::string& line) {
    std::stringstream ss(line);
    std::string word;
    int count = 0;
    while (ss >> word) {
        count++;
    }
    return count;
}

int main() {
    std::string filename;
    std::cout << "input data file name: ";
    std::cin >> filename;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "파일을 열 수 없습니다." << std::endl;
        return 1;
    }

    std::vector<std::string> original_lines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            original_lines.push_back(normalize_spaces(line));
        }
    }
    file.close();

    std::cout << "\n=== [읽어온 파일 내용] ===" << std::endl;
    print_lines(original_lines);
    std::cout << "===========================\n" << std::endl;

    // 상태 토글 플래그
    bool toggle_a = false;
    bool toggle_c = false;
    bool toggle_d = false;
    bool toggle_e = false;
    bool toggle_f = false;
    bool toggle_g = false;
    bool toggle_h = false;

    // g 명령어용 변수 백업
    char g_old_char = '\0', g_new_char = '\0';

    char command;
    while (true) {
        std::cout << "\ninput the command (a~j, q): ";
        std::cin >> command;

        if (command == 'q') {
            std::cout << "프로그램을 종료합니다." << std::endl;
            break;
        }

        switch (command) {
        case 'a': {
            toggle_a = !toggle_a;
            std::cout << "\n[a: 대소문자 반전" << (toggle_a ? " 적용]" : " 해제 - 원본 출력]") << std::endl;
            if (!toggle_a) {
                print_lines(original_lines);
            }
            else {
                for (const auto& l : original_lines) {
                    std::string mod = l;
                    for (char& c : mod) {
                        if (std::isupper(static_cast<unsigned char>(c))) c = std::tolower(static_cast<unsigned char>(c));
                        else if (std::islower(static_cast<unsigned char>(c))) c = std::toupper(static_cast<unsigned char>(c));
                    }
                    std::cout << mod << std::endl;
                }
            }
            break;
        }
        case 'b': {
            std::cout << "\n[b: 문장별 단어 개수 출력]" << std::endl;
            for (const auto& l : original_lines) {
                std::cout << l << " (" << count_words(l) << "개)" << std::endl;
            }
            break;
        }
        case 'c': {
            toggle_c = !toggle_c;
            std::cout << "\n[c: 대문자로 시작하는 단어 강조" << (toggle_c ? " 적용]" : " 해제 - 원본 출력]") << std::endl;
            if (!toggle_c) {
                print_lines(original_lines);
            }
            else {
                int total_count = 0;
                for (const auto& l : original_lines) {
                    std::stringstream ss(l);
                    std::string word;
                    bool first = true;
                    while (ss >> word) {
                        if (!first) std::cout << " ";
                        first = false;

                        if (!word.empty() && std::isupper(static_cast<unsigned char>(word[0]))) {
                            print_color(word, 14); // 노란색으로 출력
                            total_count++;
                        }
                        else {
                            std::cout << word;
                        }
                    }
                    std::cout << std::endl;
                }
                std::cout << "-> 대문자로 시작하는 단어 총 개수: " << total_count << "개" << std::endl;
            }
            break;
        }
        case 'd': {
            toggle_d = !toggle_d;
            std::cout << "\n[d: 문장 거꾸로 출력" << (toggle_d ? " 적용]" : " 해제 - 원본 출력]") << std::endl;
            if (!toggle_d) {
                print_lines(original_lines);
            }
            else {
                for (const auto& l : original_lines) {
                    std::string rev = l;
                    std::reverse(rev.begin(), rev.end());
                    std::cout << rev << std::endl;
                }
            }
            break;
        }
        case 'e': {
            toggle_e = !toggle_e;
            std::cout << "\n[e: 공백에 '*' 삽입" << (toggle_e ? " 적용]" : " 해제 - 원본 출력]") << std::endl;
            if (!toggle_e) {
                print_lines(original_lines);
            }
            else {
                for (const auto& l : original_lines) {
                    std::string mod = l;
                    for (char& c : mod) {
                        if (c == ' ') c = '*';
                    }
                    std::cout << mod << std::endl;
                }
            }
            break;
        }
        case 'f': {
            toggle_f = !toggle_f;
            std::cout << "\n[f: 각 단어 거꾸로 출력" << (toggle_f ? " 적용]" : " 해제 - 원본 출력]") << std::endl;
            if (!toggle_f) {
                print_lines(original_lines);
            }
            else {
                for (const auto& l : original_lines) {
                    std::string mod = l;
                    // '*'가 적용되어 있다면 공백으로 간주
                    for (char& c : mod) if (c == '*') c = ' ';

                    std::stringstream ss(mod);
                    std::string word;
                    bool first = true;
                    while (ss >> word) {
                        if (!first) std::cout << " ";
                        first = false;
                        std::reverse(word.begin(), word.end());
                        std::cout << word;
                    }
                    std::cout << std::endl;
                }
            }
            break;
        }
        case 'g': {
            toggle_g = !toggle_g;
            std::cout << "\n[g: 특정 문자 치환" << (toggle_g ? " 적용]" : " 해제 - 원본 출력]") << std::endl;
            if (!toggle_g) {
                print_lines(original_lines);
            }
            else {
                std::cout << "바꿀 문자 입력: ";
                std::cin >> g_old_char;
                std::cout << "새로 입력할 문자 입력: ";
                std::cin >> g_new_char;

                for (const auto& l : original_lines) {
                    std::string mod = l;
                    for (char& c : mod) {
                        if (c == g_old_char) c = g_new_char;
                    }
                    std::cout << mod << std::endl;
                }
            }
            break;
        }
        case 'h': {
            toggle_h = !toggle_h;
            std::cout << "\n[h: 숫자 뒤 줄바꿈" << (toggle_h ? " 적용]" : " 해제 - 원본 출력]") << std::endl;
            if (!toggle_h) {
                print_lines(original_lines);
            }
            else {
                for (const auto& l : original_lines) {
                    for (size_t i = 0; i < l.length(); ++i) {
                        std::cout << l[i];
                        if (std::isdigit(static_cast<unsigned char>(l[i]))) {
                            std::cout << std::endl;
                        }
                    }
                    std::cout << std::endl;
                }
            }
            break;
        }
        case 'i': {
            std::string search_word;
            std::cout << "찾을 단어를 입력하세요: ";
            std::cin >> search_word;

            std::string lower_search = search_word;
            std::transform(lower_search.begin(), lower_search.end(), lower_search.begin(), ::tolower);

            int total_found = 0;
            std::cout << "\n[i: 단어 '" << search_word << "' 검색 결과]" << std::endl;
            for (const auto& l : original_lines) {
                std::stringstream ss(l);
                std::string word;
                bool first = true;
                while (ss >> word) {
                    if (!first) std::cout << " ";
                    first = false;

                    // 구두점 등을 고려하지 않고 순수 문자열 비교 (대소문자 무시)
                    std::string lower_word = word;
                    std::transform(lower_word.begin(), lower_word.end(), lower_word.begin(), ::tolower);

                    if (lower_word == lower_search) {
                        print_color(word, 11); // 밝은 하늘색으로 출력
                        total_found++;
                    }
                    else {
                        std::cout << word;
                    }
                }
                std::cout << std::endl;
            }
            std::cout << "-> 단어 '" << search_word << "' 총 개수: " << total_found << "개" << std::endl;
            break;
        }
        case 'j': {
            std::cout << "\n[j: 문장 순서 순환 (1->2, 2->3, ..., N->1)]" << std::endl;
            if (!original_lines.empty()) {
                std::vector<std::string> shifted_lines(original_lines.size());
                size_t n = original_lines.size();
                for (size_t i = 0; i < n; ++i) {
                    shifted_lines[(i + 1) % n] = original_lines[i];
                }
                original_lines = shifted_lines; // 바뀐 순서 업데이트
                print_lines(original_lines);
            }
            break;
        }
        default:
            std::cout << "잘못된 명령어입니다. 다시 입력해 주세요." << std::endl;
            break;
        }
    }

    return 0;
}