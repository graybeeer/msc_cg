#include <GL/glew.h>
#include <GL/glfw3.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <random>

struct Square {
    float cx, cy;           // 현재 중심 좌표
    float orig_cx, orig_cy; // 최초 좌표
	float x_size, y_size;     // 현재 크기 (가로, 세로)
	float x_orig_size, y_orig_size; // 원래 크기 (가로, 세로)
    float r, g, b;          // 색상
    float vx, vy;           // 이동 속도 방향 -1
    float zigzag_t;         // 지그재그용 시간 변수 -2
    float edge_dist;        // 테두리 이동 거리 -3
};

std::vector<Square> squares;
float bg_r = 0.2f, bg_g = 0.2f, bg_b = 0.2f; // 배경 색(짙은 회색)

// 애니메이션 속도 변수 
float speed_anim1 = 500.0f;   // 1번 대각선 이동 속도
float speed_anim2 = 500.0f;   // 2번 가로 이동 속도
float speed_zigzag = 10.0f; // 2번 상하 지그재그 주파수 속도
float speed_anim3 = 300.0f;   // 3번 테두리 순회 이동 속도
float speed_size = 2.0f;    // 4번 크기 변화 속도
float speed_color = 2.0f;   // 5번 색상 변화 속도

// 애니메이션 상태 플래그
bool anim1 = false;     // 1: 대각선 이동 및 벽 튕김
bool anim2 = false;     // 2: 가로 지그재그 이동
bool anim3 = false;     // 3: 윈도우 가장자리 시계방향 순차 이동
bool animSize = false;  // 4: 크기 변화 
bool animColor = false; // 5: 색상 변화

const float windowWidth = 800.0f;
const float windowHeight = 600.0f;

// 사각형 추가 (최대 5개)
void addSquare(float x, float y) {
    if (squares.size() >= 5) return;

    Square sq;
    sq.cx = x;
    sq.cy = y;
    sq.orig_cx = x; 
    sq.orig_cy = y;
    sq.x_size = 50.0f;
    sq.y_size = 50.0f;

    sq.x_orig_size = 50.0f;
    sq.y_orig_size = 50.0f;

    if (anim1) {
        sq.vx = (rand() % 2 == 0 ? speed_anim1 : -speed_anim1);
        sq.vy = (rand() % 2 == 0 ? speed_anim1 : -speed_anim1);
    }
    else if (anim2) {
        sq.vx = speed_anim2; // 오른쪽으로 이동
        sq.vy = 0.0f; // 상하 이동 없음

    }
    else {
        sq.vx = 0;
        sq.vy = 0;
    }
    
    // 랜덤 색상 지정
    sq.r = static_cast<float>(rand()) / RAND_MAX;
    sq.g = static_cast<float>(rand()) / RAND_MAX;
    sq.b = static_cast<float>(rand()) / RAND_MAX;

    
    sq.zigzag_t = 0.0f;
    sq.edge_dist = static_cast<float>(rand() % 500);

    squares.push_back(sq);

    
}
void square_origin_reset() { // 사각형 원래 위치로 이동
	for (auto& sq : squares) {
		sq.cx = sq.orig_cx;
		sq.cy = sq.orig_cy;
		sq.x_size = sq.x_orig_size;
		sq.y_size = sq.y_orig_size;
	}
}
void changeSquareVectors1() { //1버튼 눌렀을 때 속도 방향 랜덤 변경
	for (auto& sq : squares) {
        sq.vx = (rand() % 2 == 0 ? speed_anim1 : -speed_anim1);
		sq.vy = (rand() % 2 == 0 ? speed_anim1 : -speed_anim1);
	}
}
void changeSquareVectors2() { //2버튼 눌렀을 때 속도 방향 랜덤 변경
	for (auto& sq : squares) {
		sq.vx = speed_anim2; // 오른쪽으로 이동
		sq.vy = 0.0f; // 상하 이동 없음
	}
}
void stopSquareVectors() { // 모든 사각형 속도 0으로 변경
	for (auto& sq : squares) {
		sq.vx = 0.0f;
		sq.vy = 0.0f;
	}
}
// 마우스 클릭- 사각형 생성
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        addSquare(static_cast<float>(xpos), static_cast<float>(ypos));
    }
}

// 키보드 입력 
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;

    if (key == GLFW_KEY_Q) {
        glfwSetWindowShouldClose(window, true); 
        // q: 종료
    }
    else if (key == GLFW_KEY_1) {
		// 1,2,3번은 각각 토글, 켜면 나머지는 꺼짐
        anim1 = !anim1;
        if (anim1) { 
            anim2 = false; 
            anim3 = false; 
			//square_origin_reset(); // 1번 애니메이션 시작 시 사각형 원래 위치로 이동
			changeSquareVectors1(); // 1번 애니메이션 시작 시 속도 방향 랜덤 변경
        }
    }
    else if (key == GLFW_KEY_2) {
		anim2 = !anim2;
        if (anim2) { 
            anim1 = false; 
            anim3 = false; 
			//square_origin_reset(); // 2번 애니메이션 시작 시 사각형 원래 위치로 이동
			changeSquareVectors2(); // 2번 애니메이션 시작 시 속도 방향 변경
        }
    }
    else if (key == GLFW_KEY_3) {
        anim3 = !anim3;
        if (anim3) { 
            anim1 = false; 
            anim2 = false; 
            square_origin_reset();
        }
    }
	else if (key == GLFW_KEY_4) { //4 크기 변화 
        animSize = !animSize; 
    }
    else if (key == GLFW_KEY_5) { //5 색상 변화
        animColor = !animColor; 
    }
    else if (key == GLFW_KEY_S) {
        // s: 모든 애니메이션 정지
        anim1 = false;
        anim2 = false;
        anim3 = false;
        animSize = false;
        animColor = false;
    }
    else if (key == GLFW_KEY_M) {
        // m: 원래 그린 위치로 이동
        for (auto& sq : squares) {
            sq.cx = sq.orig_cx;
            sq.cy = sq.orig_cy;
			sq.x_size = sq.x_orig_size;
			sq.y_size = sq.y_orig_size;
            anim1 = anim2 = anim3 = animSize = animColor = false;
        }
    }
    else if (key == GLFW_KEY_R) {
        // r: 사각형 삭제 및 초기화
        squares.clear();
        anim1 = anim2 = anim3 = animSize = animColor = false;
    }
}

// 애니메이션 업데이트
void update(float deltaTime) {
    float time = static_cast<float>(glfwGetTime());

    for (size_t i = 0; i < squares.size(); ++i) {
        auto& sq = squares[i];
        float half_x = sq.x_size / 2.0f;
        float half_y = sq.y_size / 2.0f;
        sq.cx += sq.vx * deltaTime;
        sq.cy += sq.vy * deltaTime;

        // 1. 대각선 이동 및 벽 튕김
        if (anim1) {
            if (sq.cx - half_x < 0.0f) { 
                sq.cx = half_x; 
				sq.vx = -sq.vx; //속도 방향 반전
            }
            if (sq.cx + half_x > windowWidth) {  // 오른쪽 벽에 닿으면
                sq.cx = windowWidth - half_x; 
                sq.vx = -sq.vx; 
            }
            if (sq.cy - half_y < 0.0f) {  // 위쪽 벽에 닿으면
                sq.cy = half_y; 
                sq.vy = -sq.vy; 
            }
			if (sq.cy + half_y > windowHeight) { // 아래쪽 벽에 닿으면
                sq.cy = windowHeight - half_y; 
                sq.vy = -sq.vy; 
            }
        }

        sq.zigzag_t += speed_zigzag * deltaTime;
        // 2. 가로 지그재그 이동
        if (anim2) {
			//일정 주기마다 좌우 방향 전환         
            if (sinf(sq.zigzag_t) > 0) {
                sq.vx = speed_anim2;
            } else {
                sq.vx = -speed_anim2;
            }
        }
        

        // 3. 윈도우 가장자리 시계방향 순차 이동
        if (anim3) {
            float perimeter = 2.0f * (windowWidth + windowHeight);

            size_t count = squares.size();
            float divisor = (count > 0) ? static_cast<float>(count) : 1.0f;
            float offset = i * (perimeter / divisor);

            sq.edge_dist += speed_anim3 * deltaTime;
            float currentDist = fmodf(sq.edge_dist + offset, perimeter);

            if (currentDist < windowWidth) {
                sq.cx = currentDist;
                sq.cy = half_y;
            }
            else if (currentDist < windowWidth + windowHeight) {
                sq.cx = windowWidth - half_x;
                sq.cy = currentDist - windowWidth;
            }
            else if (currentDist < 2.0f * windowWidth + windowHeight) {
                sq.cx = windowWidth - (currentDist - (windowWidth + windowHeight));
                sq.cy = windowHeight - half_y;
            }
            else {
                sq.cx = half_x;
                sq.cy = windowHeight - (currentDist - (2.0f * windowWidth + windowHeight));
            }
        }
		if (!anim1 && !anim2 && !anim3) {
			sq.vx = 0.0f;
			sq.vy = 0.0f;
		}
        // 4. 크기 변화 
        if (animSize) {
            sq.x_size = sq.x_orig_size + sinf(time * (speed_size * 0.7f) + i) * 20.0f;
            sq.y_size = sq.y_orig_size + sinf(time * (speed_size * 1.3f) + i) * 20.0f;
            if (sq.x_size < 15.0f) sq.x_size = 15.0f;
            if (sq.y_size < 15.0f) sq.y_size = 15.0f;
        }

        // 5. 색상 변화
        if (animColor) {
            sq.r = sinf(time * speed_color + i);
            sq.g = sinf(time * (speed_color * 0.5f) + i);
            sq.b = sinf(time * (speed_color * 1.1f) + i);
        }
    }
}

// 렌더링 함수
void render() {
    glClearColor(bg_r, bg_g, bg_b, 1.0f); // 짙은 회색 배경
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto& sq : squares) {
        glColor3f(sq.r, sq.g, sq.b);
        float half_x = sq.x_size / 2.0f;
        float half_y = sq.y_size / 2.0f;
        glRectf(sq.cx - half_x, sq.cy - half_y, sq.cx + half_x, sq.cy + half_y);
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(static_cast<int>(windowWidth), static_cast<int>(windowHeight), "Square Animation", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) return -1;

    // 2D 픽셀 좌표계 설정 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, windowWidth, windowHeight, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);

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