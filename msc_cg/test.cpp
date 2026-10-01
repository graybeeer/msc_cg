//--- 필요한 헤더파일 선언
#include "msc.h"
//--- 사용자 정의 함수
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
//--- 필요한 변수 선언
GLint width, height;
GLuint shaderProgramID;
GLuint vertexShader;
GLuint fragmentShader;


//--- 메인 함수
int main(int argc, char** argv)
{
	width = 500;
	height = 500;
	//--- GLFW 초기화
	if (!glfwInit())
		return -1;
	//--- OpenGL 버전 및 프로파일 설정
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//--- 윈도우 생성
	GLFWwindow* window =
		glfwCreateWindow(width, height, "Example1", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	//--- 생성한 윈도우의 OpenGL Context를 현재 Context로 설정
	glfwMakeContextCurrent(window);
	//--- GLEW 초기화
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}
	//--- 세이더 읽어와서 세이더 프로그램 만들기
	make_vertexShaders();
	make_fragmentShaders();
	shaderProgramID = make_shaderProgram();
	//--- Core 프로파일에서는 정점 속성이 없어도 VAO가 필요함
	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	//--- 렌더링 루프
	while (!glfwWindowShouldClose(window))
	{
		drawScene();
		//--- 윈도우에 렌더링 결과 출력
		glfwSwapBuffers(window);
		//--- 이벤트 처리
		glfwPollEvents();
	}
	//--- 종료
	glDeleteVertexArrays(1, &vertexArray);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
//--- 버텍스 세이더 객체 만들기
void make_vertexShaders()
{
	//--- 셰이더 코드 읽어오기
	std::string vertexSource = filetobuf("vertex.glsl");
	const char* source = vertexSource.c_str();
	//--- 셰이더 생성하기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 셰이더에 코드 연결하고 컴파일 하기
	glShaderSource(vertexShader, 1, &source, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	//--- 에러 체크하기
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cout << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
//--- 프래그먼트 세이더 객체 만들기
void make_fragmentShaders()
{
	//--- 셰이더 코드 읽어오기
	std::string fragmentSource = filetobuf("fragment.glsl");
	const char* source = fragmentSource.c_str();
	//--- 셰이더 생성하기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 셰이더에 코드 연결하고 컴파일 하기
	glShaderSource(fragmentShader, 1, &source, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	//--- 에러 체크하기
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
//--- 세이더 프로그램 만들고 세이더 객체 링크하기
GLuint make_shaderProgram() {
	GLint result;
	GLchar errorLog[512];
	//--- 셰이더 프로그램 생성
	GLuint shaderID = glCreateProgram();
	//--- 버텍스 셰이더와 프래그먼트 셰이더 연결
	glAttachShader
	(shaderID, vertexShader);
	glAttachShader
	(shaderID, fragmentShader);
	//--- 셰이더 프로그램 링크
	glLinkProgram
	(shaderID);
	//--- 링크가 끝났으므로 셰이더 객체 삭제
	glDeleteShader
	(vertexShader);
	glDeleteShader
	(fragmentShader);
	//--- 링크 성공 여부 확인
	glGetProgramiv
	(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog
		(shaderID, 512, NULL, errorLog);
		std::cout << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return 0;
	}
	//--- 셰이더 프로그램 사용
	glUseProgram
	(shaderID);
	return shaderID
		;
}

//--- 출력 함수
void drawScene()
{
	GLfloat rColor, gColor, bColor;
	rColor = gColor = 0.0f;
	bColor = 1.0f;
	//--- 배경색을 파란색으로 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//--- 셰이더 프로그램 사용
	glUseProgram(shaderProgramID);
	//--- 점의 크기 설정
	glPointSize(5.0f);
	//--- 0번 인덱스에서 1개의 버텍스를 사용하여 점 그리기
	glDrawArrays(GL_POINTS, 0, 1);
}

