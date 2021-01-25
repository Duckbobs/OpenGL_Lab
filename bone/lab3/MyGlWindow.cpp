
#define GLM_ENABLE_EXPERIMENTAL


#define CASCADES 3

#include "MyGlWindow.h"
#include <vector>
#include <chrono>
#include <fstream>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>




#include "Texture.hpp"

#include "Splines.h"


static float DEFAULT_VIEW_POINT[3] = { 100, 100, 100 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };

GLuint ssboHandle_t = 0;




unsigned int amount = 2000;


ShaderProgram *shaderProgram;
ShaderProgram *modelShader;

glm::mat4 *modelMatrices;



std::vector<Texture*> textures;

std::vector< SplineLib::Vec2f> cps;

MyGlWindow::MyGlWindow(int w, int h)
//==========================================================================
{

	m_width = w;
	m_height = h;

	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float) h);
	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);
	

	m_plane = new Plane(1000.0f, 1000.0f, 20, 20);


	static float gap = 1.0;
	m_model.glPushMatrix();
	m_model.glScale(1, 1, 1);
	m_model.glTranslate(0, 0, gap);
	modelPlane = m_model.getMatrix();
	m_model.glPopMatrix();

	initialize();


}

glm::mat4 lookAt(glm::vec3 pos, glm::vec3 look, glm::vec3 up)
{
	glm::vec3 zaxis = glm::normalize(pos - look);
	glm::vec3 xaxis = glm::normalize(glm::cross(up,zaxis));
	glm::vec3 yaxis = glm::normalize(glm::cross(zaxis,xaxis));

	glm::mat4 R;
	
	R[0] = glm::vec4(xaxis.x, yaxis.x, zaxis.x, 0.0f);
	R[1] = glm::vec4(xaxis.y, yaxis.y, zaxis.y, 0.0f);
	R[2] = glm::vec4(xaxis.z, yaxis.z, zaxis.z, 0.0f);
	R[3] = glm::vec4(0.0f,0.0f,0.0f,1.0f);
	
	glm::mat4 T;

	T[0] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	T[1] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	T[2] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	T[3] = glm::vec4(-pos.x, -pos.y, -pos.z, 1.0f);

	
	return R*T;
}


glm::mat4 perspective(float fov, float aspect, float n, float f)
{
	glm::mat4 P(0.0f);

	const float tanHalfFOV = tan(glm::radians(fov) / 2.0f);

	float A = (-f - n) / (f - n);
	float B = (2 * (n * f)) / ( n - f);

	P[0] = glm::vec4(1.0f / (aspect * tanHalfFOV), 0, 0, 0);
	P[1] = glm::vec4(0, 1.0 / tanHalfFOV, 0, 0);
	P[2] = glm::vec4(0, 0, A, -1.0f);
	P[3] = glm::vec4(0, 0, B, 0.0f);

	return P;
}



void MyGlWindow::draw(float animationTime)
{
			

	drawPlanes();
		
	
	drawMesh(animationTime , modelPlane);
	

//	gap += 8.0f;
}

MyGlWindow::~MyGlWindow()
{
	delete m_meshModel;
	delete m_meshModel2;
}


void MyGlWindow::changeCamera()
{

	glm::vec3 eye = m_viewer->getViewPoint(); // m_viewer->getViewPoint().x(), m_viewer->getViewPoint().y(), m_viewer->getViewPoint().z());
	glm::vec3 look = m_viewer->getViewCenter();   //(m_viewer->getViewCenter().x(), m_viewer->getViewCenter().y(), m_viewer->getViewCenter().z());
	glm::vec3 up = m_viewer->getUpVector(); // m_viewer->getUpVector().x(), m_viewer->getUpVector().y(), m_viewer->getUpVector().z());


	glm::mat4 view = lookAt(eye, look, up);
	glm::mat4 projection = perspective(45.0f, 1.0f * m_width / m_height, 0.1f, 5000.0f);




	glm::vec4 lightPos(10, 10, 10, 1);

	glm::mat4 model(1.0f);
	glm::mat4 mview = view;
	glm::mat4 mvp = projection * view;


	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));

	shaderProgram->use();

	glUniformMatrix4fv(shaderProgram->uniform("modelView"), 1, GL_FALSE, glm::value_ptr(mview));
	glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));

	glUniform4fv(shaderProgram->uniform("LightPosition"), 1, glm::value_ptr( lightPos));
		
	shaderProgram->disable();



	lightPos = glm::vec4(0, -10, 0, 1);

	modelShader->use();
	glUniformMatrix4fv(modelShader->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(modelShader->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(modelShader->uniform("viewProjection"), 1, GL_FALSE, glm::value_ptr(projection * view));

	glUniform3fv(modelShader->uniform("lightPos"), 1, glm::value_ptr(glm::vec3(lightPos)));
	glUniform3fv(modelShader->uniform("viewPos"), 1, glm::value_ptr(eye));

	modelShader->disable();
}



void MyGlWindow::drawMesh(float animationTime, glm::mat4 & model)
{

	

//	glm::mat4 imvp = glm::inverse(model);
//	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));

	
	modelShader->use();

//	glUniformMatrix4fv(modelShader->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
//	glUniformMatrix3fv(modelShader->uniform("normalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));


	float duration1 = m_meshModel->getDuration();
	float duration2 = m_meshModel2->getDuration();



	//반복 하도록 설정
	float TimeInTicks = animationTime * 30.0f;
	
	float gap = duration1 / duration2;
	float AnimationTime1 = fmod(TimeInTicks, duration1);  //fast
	float AnimationTime2 = fmod(TimeInTicks, duration1);   //slow
	
	//time curve 
	int index;
	for (int i = 0; i < cps.size()-1; i++) {
		if (cps[i].x < AnimationTime1 && cps[i + 1].x >= AnimationTime1) {
			index = i;
			break;
		}
	}
	
	//Animation 1 일때(slow) dap 이  fast
	float dap = (cps[index].y * AnimationTime1) / (cps[index].x+0.000001);
	float d = (dap)/(AnimationTime1);


	int type = m_meshModel->BoneTransform(animationTime*gap, Transforms, dualQuaternions);  //fast
	type = m_meshModel2->BoneTransform(animationTime, Transforms2, dualQuaternions2);   //slow
	type = 0;

	
	vector<glm::fdualquat> dualQuaternions3;
	for (unsigned int i = 0; i < Transforms.size(); ++i)
	{
		glm::fdualquat a = dualQuaternions[i];
		glm::fdualquat b = dualQuaternions2[i];
		glm::fdualquat c = glm::lerp(a, b, 0.9f);
		dualQuaternions3.push_back(c);
	}


	for (unsigned int i = 0; i < Transforms.size(); ++i)
	{
		const std::string name = "gBones[" + std::to_string(i) + "]";
		glUniformMatrix4fv(modelShader->uniform(name.c_str()), 1, GL_FALSE, glm::value_ptr(Transforms[i]));

	}

	DQs.resize(dualQuaternions.size());
	DQs2.resize(dualQuaternions2.size());



	for (unsigned int i = 0; i < dualQuaternions.size(); ++i) {
		
		DQs[i] = glm::mat2x4_cast(dualQuaternions3[i]);
		const std::string name = "dqs[" + std::to_string(i) + "]";
		glUniformMatrix2x4fv(modelShader->uniform(name.c_str()), 1, GL_FALSE, glm::value_ptr(DQs[i]));
	}
	

	
	glUniform1iv(modelShader->uniform("type"), 1, &type);
	

	m_meshModel->Draw(modelShader, amount);


	modelShader->disable();
	
}

void MyGlWindow::drawPlanes()
{
	

	shaderProgram->use();

	if (m_plane) m_plane->draw();

	shaderProgram->disable();



}
///////////////////////////////////////////////////



void MyGlWindow::initialize()
{


	//	m_meshModel = new Model("VictoryMonster.fbx", true,0);
	m_meshModel = new Model("fast.fbx", true, 0);
	m_meshModel->setConstraints(5, 9);
	m_meshModel->setConstraints(13, 15);

	m_meshModel2 = new Model("slow.fbx", true, 0);
	m_meshModel2->setConstraints(7, 12);
	m_meshModel2->setConstraints(19, 22);

	/////////////////////////////////////////////

	const SplineLib::Vec2f points[] =
	{
		{ 0.0f, 0.0f },
		{ 7.0f, 5.0f },
		{ 12.0f, 9.0f },
		{ 19.0, 13.0f },
		{ 22.0, 15.0f },
	};

	const int numPoints = sizeof(points) / sizeof(points[0]);

	SplineLib::cSpline2 splines[numPoints + 1];

	int numSplines = SplineLib::SplinesFromPoints(numPoints, points, numPoints + 1, splines);
	float sumLen = 0.0f;


	ofstream myfile;
	myfile.open("example.txt");



	for (int i = 0; i < 23; i++)
	{
		SplineLib::Vec2f qp(i, i * 16.0f / 23.0f);
		int index;
		float t = SplineLib::FindClosestPoint(qp, numSplines, splines, &index);

		SplineLib::Vec2f cp = SplineLib::Position(splines[index], t);
		cps.push_back(cp);
		myfile << cp.x << " " << cp.y << std::endl;

		printf("Closest point to [%6.1f, %6.1f]: index = %2d, t = %4.2f, point = [%6.1f, %6.1f]\n", qp.x, qp.y, index, t, cp.x, cp.y);
	}


	myfile.close();

	//	m_meshModel = new Model("test.fbx", true, 0);
	//	m_meshAnimation = new Model("Idle_HardDay.fbx", true,1);

	shaderProgram = new ShaderProgram();
	//load shaders
	shaderProgram->initFromFiles("model.vert", "model.frag");


	shaderProgram->addUniform("modelView");
	shaderProgram->addUniform("mvp");
	shaderProgram->addUniform("NormalMatrix");

	shaderProgram->addUniform("LightPosition");



	modelShader = new ShaderProgram();
	//load shaders
	modelShader->initFromFiles("vertex.vert", "fragment.frag");

	modelShader->addUniform("model");
	modelShader->addUniform("view");
	modelShader->addUniform("projection");
	modelShader->addUniform("normalMatrix");

	modelShader->addUniform("viewProjection");


	modelShader->addUniform("lightPos");
	modelShader->addUniform("viewPos");

	modelShader->addUniform("type");

	modelShader->addUniform("lightColor");



	for (unsigned int i = 0; i < m_meshModel->m_NumBones; ++i)
	{
		std::string name = "gBones[" + std::to_string(i) + "]";
		modelShader->addUniform(name.c_str());
	}
	for (unsigned int i = 0; i < m_meshModel->m_NumBones; ++i)
	{
		std::string name = "dqs[" + std::to_string(i) + "]";
		modelShader->addUniform(name.c_str());
	}




	glm::vec3 LightIntensity(1, 1, 1);



	modelShader->use();
	glUniform3fv(modelShader->uniform("lightColor"), 1, glm::value_ptr(LightIntensity));
	modelShader->disable();



	textures.push_back(new Texture("./Models/textures/default.png", GL_TEXTURE_2D));




	modelMatrices = new glm::mat4[amount];

	srand(clock()); // initialize random seed	
	float radius = 200.0;
	float offset = 100.5f;

	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = 0; // keep height of field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		float scale = .1f;
		model = glm::scale(model, glm::vec3(scale));

		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrices[i] = model;
	}


	modelShader->use();

	glGenBuffers(1, &ssboHandle_t);  //transformation
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboHandle_t);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4)*amount, modelMatrices, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboHandle_t);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	
	modelShader->disable();


//	unsigned int shaderTask = glCreateShader(GL_TASK_SHADER_NV);
//	unsigned int shaderMesh = glCreateShader(GL_MESH_SHADER_NV);
	   	

	/////////////////////////////////

	delete[] modelMatrices;



	
}

