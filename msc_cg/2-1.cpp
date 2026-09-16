#include <GL/glew.h>
#include <GL/glfw3.h>
#include <iostream>
float color_r = 0;
float color_g = 0;
float color_b = 1.0f;
float timer = 0.0f;
bool isTimerActive = false;
void changeColor(float r, float g, float b) {
	color_r = r;
	color_g = g;
	color_b = b;
}
int main() {
	//--- GLFW 초기화
	if (!glfwInit()) {
		std::cerr << "GLFW 초기화 실패!" << std::endl;
		return -1;
	}
	//--- OpenGL 버전 설정 (예: 3.3 Core Profile)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//--- 윈도우 생성
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
	if (!window) {
		std::cerr << "윈도우 생성 실패!" << std::endl;
		glfwTerminate();
		return -1;
	}
	//--- 컨텍스트 설정
	glfwMakeContextCurrent(window);
	//--- GLEW 초기화
	glewExperimental = GL_TRUE; // 최신 기능 사용
	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW 초기화 실패!" << std::endl;

		return -1;
	}
	//--- 뷰포트 설정
	glViewport(0, 0, 800, 600);
	
	//--- 메인 루프
	while (!glfwWindowShouldClose(window)) {
		// 입력 처리
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
			changeColor(0.0f, 1.0f, 1.0f); // 청록
			std::cout << "C 키가 눌렸습니다. 청록색으로 변경" << std::endl;
		}

		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
			std::cout << "M 키가 눌렸습니다. 자홍색으로 변경" << std::endl;
			changeColor(1.0f, 0.0f, 1.0f); // 자홍
		}
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS){
			std::cout << "Y 키가 눌렸습니다. 노란색으로 변경" << std::endl;
			changeColor(1.0f, 1.0f, 0.0f); // 노랑
		}	
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			srand(time(NULL));
			std::cout << "A 키가 눌렸습니다. 랜덤 색상으로 변경" << std::endl;
			changeColor(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f); // 랜덤 색상
		}
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
			std::cout << "G 키가 눌렸습니다. 회색으로 변경" << std::endl;
			changeColor(0.5f, 0.5f, 0.5f); // 회색
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
			std::cout << "K 키가 눌렸습니다. 검정색으로 변경" << std::endl;
			changeColor(0.0f, 0.0f, 0.0f); // 검정
		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
			std::cout << "T 키가 눌렸습니다. 타이머마다 랜덤 색상 변경" << std::endl;
			isTimerActive = true; // 타이머 활성화

		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			std::cout << "S 키가 눌렸습니다. 타이머 종료" << std::endl;
			isTimerActive = false; // 타이머 비활성화	
			timer = 0.0f; // 타이머 초기화
		}

		if (isTimerActive) {
			//std::cout << "타이머가 활성화되었습니다. 1초마다 랜덤 색상으로 변경됩니다." << std::endl;
			timer += 0.01f; // 타이머 증가
			if (timer >= 1.0f) { // 1초마다 색상 변경
				srand(time(NULL));
				changeColor(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f); // 랜덤 색상
				timer = 0.0f; // 타이머 초기화
			}
		}
		// 화면 지우기 
		glClearColor(color_r, color_g, color_b, 1.0f); // 정해진 색상으로 화면 지우기
		glClear(GL_COLOR_BUFFER_BIT);
		// 버퍼 교체
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	//--- 종료 처리
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
