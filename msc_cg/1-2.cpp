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

// 텍스트 출력 함수 
void print_color(const std::string& text, WORD color = 7) {
    set_color(color);
    std::cout << text;
    set_color(7);
}

// 연속된 공백을 단일 공백으로 변환 함수
std::string normalize_spaces(const std::string& input) {
    std::string result;
    bool in_space = false; // 연속된 공백인지 bool값
    for (char c : input) {
        if (std::isspace(static_cast<unsigned char>(c))) { //공백인지 판별
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
    return result;
}

// 문장 출력 함수
void print_lines(const std::vector<std::string>& lines) {
    for (const auto& line : lines) {
        print_color(line);
        std::cout << std::endl;
    }
}

// 단어 개수 세기 (공백 구분)
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
    
    //std::cout << "파일 이름 입력: ";
    //std::cin >> filename;
    filename = "data.txt";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "파일을 열 수 없습니다." << std::endl;
        return 1;
    }

    std::vector<std::string> original_lines; // 원본 문장
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            original_lines.push_back(normalize_spaces(line));
        }
    }
    file.close();

    std::cout << "\n-읽어온 파일 내용-" << std::endl;
    print_lines(original_lines);
    std::cout << "-----------------------------\n" << std::endl;

    // 상태 bool값
    bool bool_a = false;
    bool bool_c = false;
    bool bool_d = false;
    bool bool_e = false;
    bool bool_f = false;
    bool bool_g = false;
    bool bool_h = false;

	int j_count = 0; // j 명령어용 변수

    // g 명령어용 변수 백업
    char g_old_char = '\0', g_new_char = '\0';

    char command;
    while (true) {
        std::cout << "\n명령어 (a, b, c, d, e, f, g, h, i, j, q): ";
        std::cin >> command;

        if (command == 'q') {
            std::cout << "프로그램을 종료합니다." << std::endl;
            break;
        }

        switch (command) {
        case 'a': {
            bool_a = !bool_a;
            std::cout << "\na- 대소문자 반전" << (bool_a ? " 적용" : " 해제 ") << std::endl;
            if (!bool_a) {
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
            std::cout << "\n b- 문장별 단어 개수 출력: " << std::endl;
            for (const auto& l : original_lines) {
                std::cout << l << " (" << count_words(l) << "개)" << std::endl;
            }
            break;
        }
        case 'c': {
            bool_c = !bool_c;
            std::cout << "\nc- 대문자로 시작하는 단어" << (bool_c ? " 적용" : " 해제 ") << std::endl;
            if (!bool_c) {
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
                            print_color(word, 11); //다른색으로 출력
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
            bool_d = !bool_d;
            std::cout << "\nd- 문장 거꾸로 출력" << (bool_d ? " 적용" : " 해제 ") << std::endl;
            if (!bool_d) {
                print_lines(original_lines);
            }
            else {
                for (const auto& l : original_lines) {
                    std::string rev = l;
					std::reverse(rev.begin(), rev.end()); //reverse 함수로 문자열 뒤집기
                    std::cout << rev << std::endl;
                }
            }
            break;
        }
        case 'e': {
            bool_e = !bool_e;
            std::cout << "\ne- 공백을 *로 변경" << (bool_e ? " 적용" : " 해제 ") << std::endl;
            if (!bool_e) {
                print_lines(original_lines);
            }
            else {
                std::vector<std::string> modified_lines(original_lines.size()); //바뀐 순서 문장 저장
                size_t n = original_lines.size();
                for (size_t i = 0; i < n; ++i) {
                    modified_lines[i] = original_lines[i];
                }
                
                for (const auto& l : modified_lines) {
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
            bool_f = !bool_f;
            std::cout << "\nf- 각 단어 거꾸로 출력" << (bool_f ? " 적용" : " 해제 ") << std::endl;
            if (!bool_f) {
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
            bool_g = !bool_g;
            std::cout << "\ng- 특정 문자 치환" << (bool_g ? " 적용" : " 해제 ") << std::endl;
            if (!bool_g) {
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
            bool_h = !bool_h;
            std::cout << "\nh- 숫자 뒤 줄바꿈" << (bool_h ? " 적용" : " 해제 ") << std::endl;
            if (!bool_h) {
                print_lines(original_lines);
            }
            else {
                for (const auto& l : original_lines) {
                    for (size_t i = 0; i < l.length(); ++i) {
                        std::cout << l[i];
                        if (std::isdigit(static_cast<unsigned char>(l[i]))) { //isdight 숫자인지 확인
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

            // ws로 입력 버퍼에 남아있는 공백 및 줄바꿈 제거 후 입력 받기
            std::cin >> std::ws >> search_word;

            // 검색어를 소문자로 변환
            std::string lower_search = search_word;
            std::transform(lower_search.begin(), lower_search.end(), lower_search.begin(), ::tolower);

            int total_found = 0;
            std::cout << "\ni- 단어 '" << search_word << "' 검색 결과: " << std::endl;

            for (const auto& l : original_lines) {
                std::stringstream ss(l); //입출력스트림
                std::string word;
                bool first = true;

                while (ss >> word) {
                    if (!first) std::cout << " ";
                    first = false;

                    std::string clean_word = "";
                    for (char c : word) {
                        if (std::isalnum(static_cast<unsigned char>(c))) { // 알파벳이나 숫자인 경우만
                            clean_word += c;
                        }
                    }

                    // 소문자로 변환하여 비교
                    std::string lower_clean = clean_word;
                    std::transform(lower_clean.begin(), lower_clean.end(), lower_clean.begin(), ::tolower);

                    // 비교 및 색상 출력 
                    if (!lower_clean.empty() && lower_clean == lower_search) {
                        print_color(word, 11); // 다른색으로 출력
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
            std::cout << "\nj- 문장 순서 이동 (1->2, 2->3, ..., N->1)" << std::endl;
            j_count++;
            if (!original_lines.empty()) {
                std::vector<std::string> shifted_lines(original_lines.size()); //바뀐 순서 문장 저장
                size_t n = original_lines.size();
                for (size_t i = 0; i < n; i++) {
                    shifted_lines[(i +j_count) % n] = original_lines[i];
					
                }
                print_lines(shifted_lines);
            }
            break;
        }
        default:
            std::cout << "잘못된 명령어입니다." << std::endl;
            break;
        }
    }

    return 0;
}