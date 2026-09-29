#include <GL/glew.h>
#include <GL/glfw3.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

struct Square {
    float cx, cy;       // 중심 좌표
	float x_size, y_size; // 가로, 세로 크기
    float r, g, b;      // 색상
    bool isAlive;       // 살아있는지 여부
};

// 전역 변수
std::vector<Square> squares;
const float windowWidth = 800.0f;
const float windowHeight = 600.0f;

// 지우개 사각형 
bool isEraserActive = false;
float eraserX = 0.0f;
float eraserY = 0.0f;
float eraser_x_size = 40.0f; // 기본 사각형 크기의 2배 
float eraser_y_size = 40.0f; // 기본 사각형 크기
float eraser_x_orig_size = 40.0f; // 초기 크기
float eraser_y_orig_size = 40.0f; // 초기 크기
float eraserR = 0.0f, eraserG = 0.0f, eraserB = 0.0f; // 초기 검은색
int erasedCount = 0; // 지운 사각형 개수 (크기 조절용)

// 우클릭으로 생성된 새로운 사각형 개수 (최대 10개)
int newAddedCount = 0;

// 초기 사각형 20~40개 생성 함수
void initSquares() {
    squares.clear();
    int count = 20 + (rand() % 21); // 20 ~ 40 사이의 랜덤 개수
    for (int i = 0; i < count; ++i) {
        Square sq;
        sq.x_size = 20.0f + static_cast<float>(rand()) / RAND_MAX * 10.0f; // 작은 사각형 크기
        sq.y_size = 20.0f + static_cast<float>(rand()) / RAND_MAX * 10.0f;;
        // 좌표설정
        sq.cx = static_cast<float>(rand()) / RAND_MAX * (windowWidth);
        sq.cy = static_cast<float>(rand()) / RAND_MAX * (windowHeight);
        // 다양한 색상
        sq.r = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
        sq.g = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
        sq.b = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
        sq.isAlive = true;
        squares.push_back(sq);
    }
}
bool is_colliding(float x1, float y1, float x_size1, float y_size1, float x2, float y2, float x_size2, float y_size2) {
	float half1_x = x_size1 / 2.0f;
	float half2_x = x_size2 / 2.0f;
	float half1_y = y_size1 / 2.0f;
	float half2_y = y_size2 / 2.0f;
	return !(x1 + half1_x < x2 - half2_x || x1 - half1_x > x2 + half2_x ||
		y1 + half1_y < y2 - half2_y || y1 - half1_y > y2 + half2_y);
}
// 마우스 콜백 함수
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            // 왼쪽 마우스 누름: 지우개 사각형 생성 (검은색, 초기 크기)
            isEraserActive = true;
            eraserX = static_cast<float>(xpos);
            eraserY = static_cast<float>(ypos);
            eraser_x_size = eraser_x_orig_size; 
            eraser_y_size = eraser_y_orig_size;
            eraserR = 0.0f; eraserG = 0.0f; eraserB = 0.0f;
            erasedCount = 0;
        }
        else if (action == GLFW_RELEASE) {
            // 왼쪽 마우스 뗌: 지우개 소멸, 사라진 사각형 복원
            isEraserActive = false;
            for (auto& sq : squares) {
                sq.isAlive = true;
            }
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        // 오른쪽 마우스 누름: 새로운 사각형 생성 (최대 10개)
        if (newAddedCount < 10) {
            Square sq;
            sq.x_size = 20.0f + static_cast<float>(rand()) / RAND_MAX * 10.0f;
            sq.y_size = 20.0f + static_cast<float>(rand()) / RAND_MAX * 10.0f;
            sq.cx = static_cast<float>(xpos);
            sq.cy = static_cast<float>(ypos);
            sq.r = static_cast<float>(rand()) / RAND_MAX;
            sq.g = static_cast<float>(rand()) / RAND_MAX;
            sq.b = static_cast<float>(rand()) / RAND_MAX;
            sq.isAlive = true;
            squares.push_back(sq);
            newAddedCount++;

			eraser_x_size -= 3.0f; // 지우개 크기 감소
			eraser_y_size -= 3.0f;
			eraser_x_orig_size -= 3.0f; // 지우개 초기 크기 감소
			eraser_y_orig_size -= 3.0f;
        }
    }
}

// 마우스 커서 이동, 드래그 시 지우개 위치 갱신 및 충돌 체크
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (isEraserActive) {
        eraserX = static_cast<float>(xpos);
        eraserY = static_cast<float>(ypos);

        // 지우개 사각형과 기존 사각형들의 충돌 
        float eraserHalfX = eraser_x_size / 2.0f;
        float eraserHalfY = eraser_y_size / 2.0f;
        for (auto& sq : squares) {
            if (!sq.isAlive) continue;

            float sqHalfX = sq.x_size / 2.0f;
            float sqHalfY = sq.y_size / 2.0f;

            // 사각형 간의 충돌 판정
            bool collision = is_colliding(eraserX, eraserY, eraser_x_size, eraser_y_size, sq.cx, sq.cy, sq.x_size, sq.y_size);

            if (collision) {
                sq.isAlive = false; // 사각형 사라짐[cite: 2]
                erasedCount++;

                // 지우개 크기 커짐 및 색상 변경[cite: 2]
                eraser_x_size += 5.0f;
                eraser_y_size += 5.0f;
                eraserR = sq.r;
                eraserG = sq.g;
                eraserB = sq.b;
            }
        }
    }
}

// 키보드 입력 
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;

    if (key == GLFW_KEY_Q) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_R) {
        // r: 리셋 및 새로 시작[cite: 2]
        initSquares();
        isEraserActive = false;
        newAddedCount = 0;
		eraser_x_orig_size = 30.0f;
		eraser_y_orig_size = 30.0f;
        eraser_x_size = eraser_x_orig_size;
		eraser_y_size = eraser_y_orig_size;
    }
}

// 렌더링 함수
void render() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 흰색 배경
    glClear(GL_COLOR_BUFFER_BIT);

    // 일반 사각형들 그리기
    for (const auto& sq : squares) {
        if (!sq.isAlive) continue;
        glColor3f(sq.r, sq.g, sq.b);
        float half_x = sq.x_size / 2.0f;
        float half_y = sq.y_size / 2.0f;
        glRectf(sq.cx - half_x, sq.cy - half_y, sq.cx + half_x, sq.cy + half_y);
    }

    // 지우개 사각형 그리기 (왼쪽 마우스 누르는 동안만)
    if (isEraserActive) {
        glColor3f(eraserR, eraserG, eraserB);
        float half_x = eraser_x_size / 2.0f;
        float half_y = eraser_y_size / 2.0f;
        glRectf(eraserX - half_x, eraserY - half_y, eraserX + half_x, eraserY + half_y);
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(static_cast<int>(windowWidth), static_cast<int>(windowHeight), "Screen Eraser Game", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) return -1;

    // 2D 픽셀 좌표계 설정 (좌상단 0,0 ~ 우하단 800,600)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, windowWidth, windowHeight, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 콜백 함수 등록
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetKeyCallback(window, keyCallback);

    // 초기 사각형 배치
    initSquares();

    while (!glfwWindowShouldClose(window)) {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}