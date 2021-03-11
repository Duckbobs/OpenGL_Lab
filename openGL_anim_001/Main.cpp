#include <iostream> // 순서 상관 없음
#include <glm/glm.hpp> // 순서 상관 없음

#include <GL/glew.h> // 순서가 바뀌면 안됨
#include <GLFW/glfw3.h> // 순서가 바뀌면 안됨
#include "MyGlWindow.h"

#include "Gui.h"

GLFWwindow* window;
MyGlWindow* win;

bool lbutton_down;
bool rbutton_down;
bool mbutton_down;
double m_lastMouseX;
double m_lastMouseY;
double cx, cy;
static void key_callback(GLFWwindow*, int, int, int, int);
static void window_resize(GLFWwindow* window, int width, int height);
static void window_size_callback(GLFWwindow* window, int width, int height);
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouseDragging(double width, double height);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float animationTime = 0.0f;

bool show_test_window = false;

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
	//////////////////////////////////////////////////////////
	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	const char* glsl_version = "#version 450";

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	//ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	ImGui::StyleColorsLight();
	//////////////////////////////////////////////////////////



	// @------------------------------------------------------------------------------@
	// | Callbacks																	  |
	// @------------------------------------------------------------------------------@
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, window_resize);
	glfwSetWindowSizeCallback(window, window_size_callback);

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);



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
	float startFrame = glfwGetTime();

	double fps_previousTime = glfwGetTime();
	double fps_currentTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		fps_currentTime = glfwGetTime();
		glfwSetWindowTitle(window, std::to_string(fps_currentTime - fps_previousTime).c_str());
		// Rendering
		float currentFrame = glfwGetTime();
		//deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		animationTime = (currentFrame - startFrame);
		//////////////////////////////////////////////////////////
		// @------------------------------------------------------------------------------@
		// | imgui																		  |
		// @------------------------------------------------------------------------------@
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//adding widgets here
		//ImGui::ShowDemoWindow(&show_test_window);

		//////////////////////////////////////////////////////////
		Gui::Test();
		//////////////////////////////////////////////////////////



		win->draw(animationTime); // 매번 호출

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



		//////////////////////////////////////////////////////////
		glfwSwapBuffers(window); // Swap front and back buffers
		glfwPollEvents(); // Poll forand process events

		if (io.WantCaptureMouse == false) {
			mouseDragging(width, height);
		}


		fps_previousTime = fps_currentTime;
	}

	// @------------------------------------------------------------------------------@
	// | Terminate																	  |
	// @------------------------------------------------------------------------------@
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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

static void window_size_callback(GLFWwindow* window, int width, int height) {
	win->setSize(width, height);
	win->setAspect(width / (float)height);
}
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	cx = xpos;
	cy = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		std::cout << "GLFW_PRESS" << std::endl;
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		m_lastMouseX = xpos;
		m_lastMouseY = ypos;

		win->mouse_button_click(xpos, ypos);
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		//std::cout << "GLFW_MOUSE_BUTTON_LEFT" << std::endl;
		if (GLFW_PRESS == action)
			lbutton_down = true;
		else if (GLFW_RELEASE == action)
			lbutton_down = false;
	}

	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		//std::cout << "GLFW_MOUSE_BUTTON_RIGHT" << std::endl;
		if (GLFW_PRESS == action)
			rbutton_down = true;
		else if (GLFW_RELEASE == action)
			rbutton_down = false;
	}

	else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		//std::cout << "GLFW_MOUSE_BUTTON_MIDDLE" << std::endl;
		if (GLFW_PRESS == action)
			mbutton_down = true;
		else if (GLFW_RELEASE == action)
			mbutton_down = false;
	}
}
void mouseDragging(double width, double height)
{
	if (lbutton_down) {
		float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
		win->m_viewer->rotate(fractionChangeX, fractionChangeY);
	}
	else if (mbutton_down) {
		float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
		win->m_viewer->zoom(fractionChangeY);
	}
	else if (rbutton_down) {
		float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
		win->m_viewer->translate(-fractionChangeX, -fractionChangeY, 1);
	}
	m_lastMouseX = cx;
	m_lastMouseY = cy;
}
