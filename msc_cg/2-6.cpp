#include <GL/glew.h>
#include <GL/glfw3.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>

// 조각 구조체
struct Fragment {
    float cx, cy;       // 중심 좌표
	float x_size, y_size; // 가로, 세로 크기
    float vx, vy;       // 이동 속도 및 방향
    float r, g, b;      // 색상
    bool colorBool;     // true면 밝아지고, false면 어두워짐
    bool isAlive;       // 생존 여부
};

// 원본 사각형 구조체
struct Square {
    float cx, cy;
	float x_size, y_size;
    float r, g, b;
    bool colorBool;     // true면 밝아지고, false면 어두워짐
    bool isAlive;
};

std::vector<Square> squares;
std::vector<Fragment> fragments;

const float windowWidth = 800.0f;
const float windowHeight = 600.0f;

void initSquares() {
    squares.clear();
    fragments.clear();
    int count = 5 + (rand() % 6); // 5 ~ 10개
    for (int i = 0; i < count; ++i) {
        Square sq;
        sq.x_size = 40.0f + static_cast<float>(rand()) / RAND_MAX * 40.0f; // 40~80 크기
        sq.y_size = 40.0f + static_cast<float>(rand()) / RAND_MAX * 40.0f;
        sq.cx = static_cast<float>(rand()) / RAND_MAX * windowWidth;
        sq.cy = static_cast<float>(rand()) / RAND_MAX * windowHeight;
        sq.r = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
        sq.g = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
        sq.b = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
        sq.isAlive = true;
        sq.colorBool = rand() % 2 == 0 ? true : false;
        squares.push_back(sq);
    }
}
float fragSpeed = 50.0f;
void addNewFrag(Square sq, float cx, float cy,int type) {
    Fragment frag;
    frag.cx = cx;
    frag.cy = cy;
    frag.x_size = sq.x_size / 2.0f;
    frag.y_size = sq.y_size / 2.0f;
    frag.r = sq.r;
    frag.g = sq.g;
    frag.b = sq.b;
    frag.colorBool = sq.colorBool;
    frag.isAlive = true;
    if (type == 1) {
        frag.vx = fragSpeed;
        frag.vy = fragSpeed;
    }
    else if (type == 2) {
        frag.vx = fragSpeed;
        frag.vy = -fragSpeed;
    }
    else if (type == 3) {
        frag.vx = -fragSpeed;
        frag.vy = -fragSpeed;
    }
    else if (type == 4) {
        frag.vx = -fragSpeed;
        frag.vy = fragSpeed;
    }
    else if (type == 5) {
        frag.vx = 0;
        frag.vy = fragSpeed;
    }
    else if (type == 6) {
        frag.vx = fragSpeed;
        frag.vy = 0;
    }
    else if (type == 7) {
        frag.vx = 0;
        frag.vy = -fragSpeed;
    }
    else if (type == 8) {
        frag.vx = -fragSpeed;
        frag.vy = 0;
    }
    fragments.push_back(frag);
}

// 마우스 클릭 콜백 (사각형 내부 클릭 시 사등분 및 애니메이션 부여)
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        float mx = static_cast<float>(xpos);
        float my = static_cast<float>(ypos);

        for (auto& sq : squares) {
            if (!sq.isAlive) continue;

            float halfX = sq.x_size / 2.0f;
            float halfY = sq.y_size / 2.0f;
            float quatX = sq.x_size / 4.0f;
            float quatY = sq.y_size / 4.0f;
            // 사각형 내부 클릭 판정
            if (mx >= sq.cx - halfX && mx <= sq.cx + halfX &&
                my >= sq.cy - halfY && my <= sq.cy + halfY) {

                sq.isAlive = false; // 원본 사각형 소멸

                // 4가지 애니메이션 중 하나를 랜덤 선택 (1, 2, 3, 4)
                int animType = 1 + (rand() % 4);
                
                float subSize = sq.x_size / 2.0f;
                
                if(animType==1){ //좌우상하 이동
                    addNewFrag(sq, sq.cx + quatX, sq.cy + quatY, 5);
                    addNewFrag(sq, sq.cx + quatX, sq.cy - quatY, 6);
                    addNewFrag(sq, sq.cx - quatX, sq.cy - quatY, 7);
                    addNewFrag(sq, sq.cx - quatX, sq.cy + quatY, 8);
                }
                else if (animType == 2) { //대각선 이동
                    addNewFrag(sq, sq.cx + quatX, sq.cy + quatY, 1);
                    addNewFrag(sq, sq.cx + quatX, sq.cy - quatY, 2);
                    addNewFrag(sq, sq.cx - quatX, sq.cy - quatY, 3);
                    addNewFrag(sq, sq.cx - quatX, sq.cy + quatY, 4);
                }
                else if (animType == 3) { //사등분 된 사각형이 한쪽 방향으로 같이 이동한다.
                    int frag33 = 1 + (rand() % 8);
                    addNewFrag(sq, sq.cx + quatX + 1.0f, sq.cy + quatY + 1.0f, frag33);
                    addNewFrag(sq, sq.cx + quatX + 1.0f, sq.cy - quatY - 1.0f, frag33);
                    addNewFrag(sq, sq.cx - quatX - 1.0f, sq.cy - quatY - 1.0f, frag33);
                    addNewFrag(sq, sq.cx - quatX - 1.0f, sq.cy + quatY + 1.0f, frag33);
                }
                else if (animType == 4) {
                    addNewFrag(sq, sq.cx + quatX, sq.cy + quatY, 5);
                    addNewFrag(sq, sq.cx + quatX, sq.cy - quatY, 6);
                    addNewFrag(sq, sq.cx - quatX, sq.cy - quatY, 7);
                    addNewFrag(sq, sq.cx - quatX, sq.cy + quatY, 8);
                    addNewFrag(sq, sq.cx + quatX, sq.cy + quatY, 1);
                    addNewFrag(sq, sq.cx + quatX, sq.cy - quatY, 2);
                    addNewFrag(sq, sq.cx - quatX, sq.cy - quatY, 3);
                    addNewFrag(sq, sq.cx - quatX, sq.cy + quatY, 4);
                }
               
            }
        }
    }
}

// 키보드 콜백
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;
    if (key == GLFW_KEY_Q) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_R) {
        initSquares(); // r: 리셋
    }
}

// 애니메이션 업데이트 로직
float reduceSpeed = 10.0f;
void update(float deltaTime) {
    for (auto& frag : fragments) {
        if (!frag.isAlive) continue;

        // 위치 이동
        frag.cx += frag.vx * deltaTime;
        frag.cy += frag.vy * deltaTime;

        // 크기 점점 작아짐
        frag.x_size -= reduceSpeed * deltaTime;
        frag.y_size -= reduceSpeed * deltaTime;
        if (frag.x_size <= 1.0f || frag.y_size <= 1.0f) {
            frag.isAlive = false; // 특정 크기 이하가 되면 완전히 사라짐
        }

        // 색상 밝기 변화 (밝아지거나 어두워짐)
        if (frag.colorBool) {
            frag.r = std::clamp(frag.r + 1.0f * deltaTime, 0.0f, 1.0f);
            frag.g = std::clamp(frag.g + 1.0f * deltaTime, 0.0f, 1.0f);
            frag.b = std::clamp(frag.b + 1.0f * deltaTime, 0.0f, 1.0f);
        }
        else {
            frag.r = std::clamp(frag.r + -1.0f * deltaTime, 0.0f, 1.0f);
            frag.g = std::clamp(frag.g + -1.0f * deltaTime, 0.0f, 1.0f);
            frag.b = std::clamp(frag.b + -1.0f * deltaTime, 0.0f, 1.0f);
        }
    }

}

// 렌더링 함수
void render() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 흰색 배경
    glClear(GL_COLOR_BUFFER_BIT);

    // 사각형 그리기
    for (const auto& sq : squares) {
        if (!sq.isAlive) continue;
        glColor3f(sq.r, sq.g, sq.b);
        float halfX = sq.x_size / 2.0f;
        float halfY = sq.y_size / 2.0f;
        glRectf(sq.cx - halfX, sq.cy - halfY, sq.cx + halfX, sq.cy + halfY);
    }

    // 조각들 그리기
    for (const auto& frag : fragments) {
        if (!frag.isAlive) continue;
        glColor3f(frag.r, frag.g, frag.b);
        float halfX = frag.x_size / 2.0f;
        float halfY = frag.y_size / 2.0f;
        glRectf(frag.cx - halfX, frag.cy - halfY, frag.cx + halfX, frag.cy + halfY);
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(static_cast<int>(windowWidth), static_cast<int>(windowHeight), "Expanding Squares Animation", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) return -1;

    // 좌표계
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, windowWidth, windowHeight, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);

    initSquares();
    float lastTime = static_cast<float>(glfwGetTime());
    while (!glfwWindowShouldClose(window)) {
        
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        update(deltaTime);
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}