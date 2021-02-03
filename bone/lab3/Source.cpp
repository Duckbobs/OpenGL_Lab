
//#define GLFW_INCLUDE_GLU

#include <GL/gl3w.h>

#include "MyGlWindow.h"
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/dual_quaternion.hpp>




MyGlWindow * win;

bool lbutton_down;
bool rbutton_down;
bool mbutton_down;
double m_lastMouseX;
double m_lastMouseY;
double cx, cy;



vector<glm::mat4> Transforms;
vector<glm::fdualquat> dualQuaternions;
vector<glm::mat2x4> DQs;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float animationTime = 0.0f;



void mouseDragging(double width, double height);

void window_size_callback(GLFWwindow* window, int width, int height)
{
	win->setSize(width, height);
	win->setAspect(width / (float)height);

}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}


static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	cx = xpos;
	cy = ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ||
		glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS ||
		glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS
		)
	{
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		mouseDragging(w, h);
		win->changeCamera();
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		m_lastMouseX = xpos;
		m_lastMouseY = ypos;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action)
			lbutton_down = true;
		else if (GLFW_RELEASE == action)
			lbutton_down = false;
	}

	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (GLFW_PRESS == action)
			rbutton_down = true;
		else if (GLFW_RELEASE == action)
			rbutton_down = false;
	}

	else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
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



int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{

	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	int width = 800;
	int height = 800;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "OpenGL FrameWork", NULL, NULL);


	if (!window)
	{
		glfwTerminate();
		return -1;
	}



	glfwMakeContextCurrent(window);


	/* Make the window's context current */

	if (gl3wInit()) {
		fprintf(stderr, "failed to initialize OpenGL\n");
		return -1;
	}


	if (!gl3wIsSupported(4, 3)) {
		fprintf(stderr, "OpenGL 4.5 not supported\n");
		return -1;
	}

	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));


	glfwSwapInterval(1);  //enable vsync

	


	win = new MyGlWindow(width, height);

	bool show_test_window = true;
	bool show_another_window = false;
	

	double previousTime = glfwGetTime();
	int frameCount = 0;
	double lastTime;



	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetKeyCallback(window, key_callback);


	glfwSetWindowTitle(window, "myGlWindow");


	
	
	float startFrame = glfwGetTime();


	win->changeCamera();


	double fps_previousTime = glfwGetTime();
	double fps_currentTime = glfwGetTime();
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		fps_currentTime = glfwGetTime();
		glfwSetWindowTitle(window, std::to_string(fps_currentTime - fps_previousTime).c_str());

		

		// Rendering
		float currentFrame = glfwGetTime();
		//deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		animationTime = (currentFrame - startFrame);


		
		glClearColor(0.2f, 0.2f, .2f, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glEnable(GL_DEPTH_TEST);
		glEnable(GL_PROGRAM_POINT_SIZE);


		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);



		win->draw(animationTime);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
		//glfwWaitEvents();
			

		fps_previousTime = fps_currentTime;
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}


