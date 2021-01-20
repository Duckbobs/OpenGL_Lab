//#define  FREEGLUT_LIB_PRAGMAS  0

#pragma warning(push)
#pragma warning(disable:4311)		// convert void* to long
#pragma warning(disable:4312)		// convert long to void*

#include <iostream>
#include <GL/gl3w.h>
#include <string>
#include <stack>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"


#include "Viewer.h"
#include "ModelView.h"


#include "Plane.h"



#include "Model.h"
#include "Skeleton.h"
#pragma warning(pop)


class MyGlWindow {
public:
	MyGlWindow(int w, int h);
	~MyGlWindow();
	void draw(float animationTime);
	void setSize(int w, int h) { m_width = w; m_height = h; }
	void setAspect(float r) { m_viewer->setAspectRatio(r); }

	void changeCamera();

	Viewer *m_viewer;
private:
	int m_width;
	int m_height;
	void initialize();
	
	void drawMesh(float animationTime, glm::mat4 & model);
	void drawPlanes();

	ModelMatrix m_model;
	Plane * m_plane;

	Skeleton * m_skeleton;
	Model * m_meshModel;
	Model * m_meshModel2;

	glm::mat4 modelPlane;
	

	vector<glm::mat4> Transforms;
	vector<glm::fdualquat> dualQuaternions;
	vector<glm::mat2x4> DQs;

	vector<glm::mat4> Transforms2;
	vector<glm::fdualquat> dualQuaternions2;
	vector<glm::mat2x4> DQs2;

	
};
