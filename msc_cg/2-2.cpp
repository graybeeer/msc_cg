#include <GL/glew.h>
#include <GL/glfw3.h>
#include <iostream>
#include <list>
#include <cstdlib>
#include <ctime>
#include <algorithm>

struct AreaSquare {
    float r, g, b;
    int quadrant; // 1: 3사분면(좌하), 2: 4사분면(우하), 3: 2사분면(좌상), 4: 1사분면(우상)
    float x1, y1, x2, y2;
};

struct Square {
    float x1, y1, x2, y2; // 좌하단(x1,y1), 우상단(x2,y2)
    float r, g, b;       // 색상
    bool isSelected;     // 선택 여부
    int quadrant;        // 속한 사분면 번호
};

// 4개의 사분면 영역 설정 (1:좌하, 2:우하, 3:좌상, 4:우상)
AreaSquare area_squares[4] = {
    {0.2f, 0.2f, 0.2f, 1, -1.0f, -1.0f,  0.0f,  0.0f}, // 3사분면
    {0.3f, 0.3f, 0.3f, 2,  0.0f, -1.0f,  1.0f,  0.0f}, // 4사분면
    {0.4f, 0.4f, 0.4f, 3, -1.0f,  0.0f,  0.0f,  1.0f}, // 2사분면
    {0.5f, 0.5f, 0.5f, 4,  0.0f,  0.0f,  1.0f,  1.0f}  // 1사분면
};

std::list<Square> squares;
float bg_r = 0.1f, bg_g = 0.1f, bg_b = 0.1f;

// 영역 색상 랜덤 생성
void randomizeAreaColors() {
    for (auto& area : area_squares) {
        area.r = static_cast<float>(rand()) / RAND_MAX;
        area.g = static_cast<float>(rand()) / RAND_MAX;
        area.b = static_cast<float>(rand()) / RAND_MAX;
    }
}

// 해당 사분면에 사각형 추가 (최대 5개 제한)
void addSquareToQuadrant(int quad) {
    int count = 0;
    for (const auto& sq : squares) {
        if (sq.quadrant == quad) count++;
    }
    if (count >= 5) {
        std::cout << quad << "번 사분면에 더 이상 사각형을 추가할 수 없습니다 (최대 5개)" << std::endl;
        return;
    }

    const AreaSquare& area = area_squares[quad - 1];

    // 랜덤 크기 및 색상 
    Square sq;
    sq.x1 = area.x1 + (static_cast<float>(rand()) / RAND_MAX) * 0.5f;
    sq.y1 = area.y1 + (static_cast<float>(rand()) / RAND_MAX) * 0.5f;
    sq.x2 = area.x2 - (static_cast<float>(rand()) / RAND_MAX) * 0.5f;
    sq.y2 = area.y2 - (static_cast<float>(rand()) / RAND_MAX) * 0.5f;
    sq.r = static_cast<float>(rand()) / RAND_MAX;
    sq.g = static_cast<float>(rand()) / RAND_MAX;
    sq.b = static_cast<float>(rand()) / RAND_MAX;
    sq.isSelected = false;
    sq.quadrant = quad;

    squares.push_back(sq);
}

// 화면 렌더링 함수
void drawScene() {
    glClearColor(bg_r, bg_g, bg_b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto& area : area_squares) {
        glColor3f(area.r, area.g, area.b);
        glRectf(area.x1, area.y1, area.x2, area.y2);
    }

    for (const auto& sq : squares) {
        if (sq.isSelected) {
            // 선택 표시: 약간 더 큰 하얀색 테두리 사각형 렌더링
            glColor3f(1.0f, 1.0f, 1.0f);
            glRectf(sq.x1 - 0.02f, sq.y1 - 0.02f, sq.x2 + 0.02f, sq.y2 + 0.02f);
        }
        glColor3f(sq.r, sq.g, sq.b);
        glRectf(sq.x1, sq.y1, sq.x2, sq.y2);
    }
}

// 마우스 버튼 콜백 함수 (사각형 선택)
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // 윈도우 스크린 좌표 
        float ndc_x = static_cast<float>((xpos / width) * 2.0 - 1.0);
        float ndc_y = static_cast<float>(1.0 - (ypos / height) * 2.0);

        // 이전 선택 해제 및 가장 최근에 그려진 사각형부터 피킹 체크
        bool picked = false;
        for (auto it = squares.rbegin(); it != squares.rend(); ++it) {
            if (!picked && ndc_x >= it->x1 && ndc_x <= it->x2 && ndc_y >= it->y1 && ndc_y <= it->y2) {
                it->isSelected = true;
                picked = true;
            }
            else {
                it->isSelected = false;
            }
        }
    }
}

// 키보드 입력 콜백 함수
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

    if (key == GLFW_KEY_Q) glfwSetWindowShouldClose(window, true);

    // 1~4 키: 사각형 추가
    if (key == GLFW_KEY_1) addSquareToQuadrant(1);
    if (key == GLFW_KEY_2) addSquareToQuadrant(2);
    if (key == GLFW_KEY_3) addSquareToQuadrant(3);
    if (key == GLFW_KEY_4) addSquareToQuadrant(4);

    // r 키: 초기화
    if (key == GLFW_KEY_R) {
        squares.clear();
        randomizeAreaColors();
    }

    // c 키: 선택된 사각형 색상 변경
    if (key == GLFW_KEY_C) {
        for (auto& sq : squares) {
            if (sq.isSelected) {
                sq.r = static_cast<float>(rand()) / RAND_MAX;
                sq.g = static_cast<float>(rand()) / RAND_MAX;
                sq.b = static_cast<float>(rand()) / RAND_MAX;
            }
        }
    }

    // + / - 키: 선택된 사각형 크기 조정 (사분면 영역 안벗어남)
    if (key == GLFW_KEY_EQUAL) { // + 키
        for (auto& sq : squares) {
            if (sq.isSelected) {
                const AreaSquare& area = area_squares[sq.quadrant - 1];
                float delta = 0.02f;
                sq.x1 = (std::max)(area.x1, sq.x1 - delta);
                sq.y1 = (std::max)(area.y1, sq.y1 - delta);
                sq.x2 = (std::min)(area.x2, sq.x2 + delta);
                sq.y2 = (std::min)(area.y2, sq.y2 + delta);
            }
        }
    }

    if (key == GLFW_KEY_MINUS ) { // - 키
        for (auto& sq : squares) {
            if (sq.isSelected) {
                float delta = 0.02f;
                if ((sq.x2 - sq.x1) > 0.05f && (sq.y2 - sq.y1) > 0.05f) { // 최소 크기 보장
                    sq.x1 += delta;
                    sq.y1 += delta;
                    sq.x2 -= delta;
                    sq.y2 -= delta;
                }
            }
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));

    if (!glfwInit()) {
        std::cerr << "GLFW 초기화 실패!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Quadrant Management", nullptr, nullptr);
    if (!window) {
        std::cerr << "윈도우 생성 실패!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW 초기화 실패!" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    // 이벤트 콜백 함수 등록
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);

    randomizeAreaColors();

    while (!glfwWindowShouldClose(window)) {
        drawScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}