#include <iostream> // 순서 상관 없음
#include <glm/glm.hpp> // 순서 상관 없음

#include <GL/glew.h> // 순서가 바뀌면 안됨
#include <GLFW/glfw3.h> // 순서가 바뀌면 안됨
#include "MyGlWindow.h"

GLFWwindow* window;
MyGlWindow* win;

static void key_callback(GLFWwindow*, int, int, int, int);
static void window_resize(GLFWwindow* window, int width, int height);

int main() {
	// @------------------------------------------------------------------------------@
	// | Settings																	  |
	// @------------------------------------------------------------------------------@
#pragma region Settings
	// @------------------------------------------------------------------------------@
	// | Initialize the library														  |
	// | openGL 버전 설정 = MAJOR.MINOR = 4.3										  |
	// | CORE = 최신기능, COMPATIBLE = 모든기능										  |
	// @------------------------------------------------------------------------------@
	if (!glfwInit()) {
		// 초기화 무조건 해야함.
		std::cout << "glfw init error" << std::endl;
		return 0;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	// @------------------------------------------------------------------------------@
	// | Create a windowed mode window and its OpenGL context						  |
	// | glfw = 창, 이벤트															  |
	// @------------------------------------------------------------------------------@
	int width = 800;
	int height = 800;
	GLFWwindow* window = glfwCreateWindow(width, height, u8"나의 openGL윈도우", NULL, NULL); // NULL NULL = 창으로 열겠다. 다른 값 넣으면 풀스크린 가능 // 한글은 u8을 붙여줘야함.
	//glfwSetWindowTitle(window, u8"나의 openGL윈도우"); // 한글은 u8을 붙여줘야함.
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // 거대한 구조체 생성



	// @------------------------------------------------------------------------------@
	// | Callbacks																	  |
	// @------------------------------------------------------------------------------@
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, window_resize);



	// @------------------------------------------------------------------------------@
	// | Make the window's context current											  |
	// | glew = Helper																  |
	// @------------------------------------------------------------------------------@
	GLenum err = glewInit(); // 헬퍼 초기화
	if (err != GLEW_OK) {
		//Problem: glewInit failed, something is seriously wrong.
		std::cout << "glewInit failed, aborting." << std::endl;
		return 0;
	}
	glfwSwapInterval(1);
	// enable vsync = 모니터 주사율과 동기화; glewExperimental = GL_TRUE; // 지워도 됨. 코어프로파일을 쓸때 TRUE 값 넣도록 만들었는데 현재는 자동으로 됨.
#pragma endregion



	// @------------------------------------------------------------------------------@
	// | Rendering																	  |
	// @------------------------------------------------------------------------------@
	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	win = new MyGlWindow(width, height);

	glm::vec3 camPos = glm::vec3(5, 5, 5);
	glm::vec3 Look = glm::vec3(0, 0, 0);
	glm::vec3 Up = glm::vec3(0, 1, 0);

	win->lookAt(camPos, Look, Up);

	while (!glfwWindowShouldClose(window))
	{
		win->draw(); // 매번 호출

		glfwSwapBuffers(window); // Swap front and back buffers

		glfwPollEvents(); // Poll forand process events
	}

	// @------------------------------------------------------------------------------@
	// | Terminate																	  |
	// @------------------------------------------------------------------------------@
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}



// @------------------------------------------------------------------------------@
// | Callbacks																	  |
// @------------------------------------------------------------------------------@
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
		printf("r");
}
static void window_resize(GLFWwindow* window, int width, int height) {
	win->resize(width, height);
}
