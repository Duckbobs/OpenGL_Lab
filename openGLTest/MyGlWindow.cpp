#include "MyGlWindow.h"

ShaderProgram* shaderProgram;

MyGlWindow::MyGlWindow(int w, int h)
{
	m_width = w;
	m_height = h;

	setupBuffer();
}
