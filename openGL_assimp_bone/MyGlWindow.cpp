#include "MyGlWindow.h"
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp> // translate()
#include <glm/gtc/type_ptr.hpp> // value_ptr()

#include <glm/gtx/string_cast.hpp> // to_string()

#include <vector>
#include "Splines.h"

std::vector< SplineLib::Vec2f> cps;
void MyGlWindow::setupBuffer()
{
	shaderProgram = new ShaderProgram();
	shaderProgram->initFromFiles("sampler.vert", "sampler.frag"); // 쉐이더 지정

	glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	char* directory;
	directory = (char*)"assets/suit/scene.fbx";
	m_model = new Model(directory);
}
void MyGlWindow::initialize() {
	MyGlWindow::setupBuffer();
	for (int i = 0; i < 5; i++)
	{
		std::string name;
		name = "Light[" + std::to_string(i) + "].Position";
		shaderProgram->addUniform(name);
		name = "Light[" + std::to_string(i) + "].Intensity";
		shaderProgram->addUniform(name);
	}

	shaderProgram->addUniform("Material.Ka");
	shaderProgram->addUniform("Material.Kd");
	shaderProgram->addUniform("Material.Ks");
	shaderProgram->addUniform("Material.Shiness");
	shaderProgram->addUniform("viewProjection");

	/*for (unsigned int i = 0; i < m_model->modelData.m_NumBones; ++i)
	{
		std::string name = "gBones[" + std::to_string(i) + "]";
		shaderProgram->addUniform(name.c_str());
	}*/
	for (unsigned int i = 0; i < m_model->modelData.m_NumBones; ++i)
	{
		std::string name = "dqs[" + std::to_string(i) + "]";
		shaderProgram->addUniform(name.c_str());
	}

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


	std::ofstream myfile;
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
	shaderProgram->use();
	glGenBuffers(1, &ssboHandle_t);  //transformation
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboHandle_t);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * amount, modelMatrices, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboHandle_t);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	shaderProgram->disable();
}
MyGlWindow::MyGlWindow(int w, int h)
{
	width = w;
	height = h;
	static float DEFAULT_VIEW_POINT[3] = { 100, 100, 100 };
	static float DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };
	static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };
	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float)h);
	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);

	initialize();
}
const glm::mat4 _mat = glm::mat4(1.0f);
const glm::vec3 _vec = glm::vec3(1, 1, 1);
// 매 프레임 호출 됨
void MyGlWindow::draw(float animationTime) {
	// @------------------------------------------------------------------------------@
	// | Settings																	  |
	// @------------------------------------------------------------------------------@
#pragma region Settings
	glClearColor(0.2, 0.2, 0.2, 1); // 백그라운드 색상 선택
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height); //뷰포트(viewport) 설정
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE); //Cullface를 켬
	//glCullFace(GL_BACK); //오브젝트의 back 부분을 Culling
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#pragma endregion
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 mvp;
	glm::mat4 mview;

	glm::mat4 imvp;
	glm::mat4 imp;
	glm::mat3 nmat;

	glm::vec3 eye = m_viewer->getViewPoint();
	glm::vec3 look = m_viewer->getViewCenter();
	glm::vec3 up = m_viewer->getUpVector();
	view = lookAt(eye, look, up);

	glm::mat4 trans;
	glm::mat4 rot;
	glm::mat4 scale;
	projection = perspective(45.0f, 1.0f * width / height, 0.1f, 5000.0f);
	
	shaderProgram->use(); // shader 호출
	glm::vec4 lightPos[] = {
		glm::vec4(global::lightPos[0], 1),
		glm::vec4(global::lightPos[1], 1),
		glm::vec4(global::lightPos[2], 1),
		glm::vec4(global::lightPos[3], 1),
		glm::vec4(global::lightPos[4], 1)
	};
	glm::vec3 lightIntensity[] = {
		global::lightIntensity[0],
		global::lightIntensity[1],
		global::lightIntensity[2],
		global::lightIntensity[3],
		global::lightIntensity[4]
	};
	int j = 0;
	for (int i = -180; i < 180; i += 72) {
		//Radian으로 변경, 더 쉬운 방법은? Glm함수 이용
		float ang = (i * glm::pi<float>() / 180.0f);

		float x = 5.0f * cosf(ang);
		float z = 5.0f * sinf(ang);
		glm::vec4 lightPos;
		global::lightPos[j++] = view * glm::vec4(x, 5.2, z, 1.0);
	}

	glm::vec3 Ka(0.1, 0.1, 0.1);
	glm::vec3 Kd(0.4, 0.4, 0.4);
	glm::vec3 Ks(0.9, 0.9, 0.9);
	GLfloat shiness = 180.0f;

		for (int i = 0; i < 5; i++)
		{
			std::string name;
			name = "Light[" + std::to_string(i) + "].Position";
			glUniform4fv(shaderProgram->uniform(name), 1, glm::value_ptr(lightPos[i]));
			name = "Light[" + std::to_string(i) + "].Intensity";
			glUniform3fv(shaderProgram->uniform(name), 1, glm::value_ptr(lightIntensity[i]));
		}

		glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(Ka));
		glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(Kd));
		glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(Ks));
		glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &shiness);

		trans = glm::translate(_mat, glm::vec3(1.0f, 0.15f, 0));
		rot = glm::rotate(_mat, glm::radians(0.0f), glm::vec3(1.0f, 0, 0));
		scale = glm::scale(_mat, glm::vec3(1.0f, 1.0f, 1.0f));
		model = trans * rot * scale;
		mvp = projection * view * model;
		mview = view * model;
		imvp = glm::inverse(mview);
		nmat = glm::mat3(glm::transpose(imvp));
		glUniformMatrix4fv(shaderProgram->uniform("viewProjection"), 1, GL_FALSE, glm::value_ptr(projection * view));
		
		/*float duration1 = m_model->getDuration();
		//반복 하도록 설정
		float TimeInTicks = animationTime * 1.0f;
		float AnimationTime1 = fmod(TimeInTicks, duration1);  //fast
	
		m_model->BoneTransform(AnimationTime1, Transforms, dualQuaternions);
		DQs.resize(dualQuaternions.size());
		for (unsigned int i = 0; i < dualQuaternions.size(); ++i) {
		
			DQs[i] = glm::mat2x4_cast(dualQuaternions[i]);
			const std::string name = "dqs[" + std::to_string(i) + "]";
			glUniformMatrix2x4fv(shaderProgram->uniform(name.c_str()), 1, GL_FALSE, glm::value_ptr(DQs[i]));
		}*/

		if (m_model) {
			m_model->Draw(shaderProgram, amount);
		}

	shaderProgram->disable();
}
void MyGlWindow::resize(int w, int h) {
	width = w;
	height = h;
}
void MyGlWindow::setSize(int w, int h) {
	width = w;
	height = h;
}
glm::mat4 MyGlWindow::lookAt(glm::vec3 campos, glm::vec3 look, glm::vec3 up) {
	glm::vec3 zc = glm::normalize(campos - look);
	glm::vec3 xc = glm::normalize(glm::cross(up, zc));
	glm::vec3 yc = glm::normalize(glm::cross(zc, xc));

	glm::mat4 Mat4 = {
		{xc.x, yc.x, zc.x, 0},
		{xc.y, yc.y, zc.y, 0},
		{xc.z, yc.z, zc.z, 0},
		{0, 0, 0, 1}
	};
	glm::mat4 Mat4_1 = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{-campos.x, -campos.y, -campos.z, 1}
	};
	return Mat4 * Mat4_1;
}
glm::mat4 MyGlWindow::perspective(float fov, float aspect, float n, float f)
{
	glm::mat4 P(0.0f);

	const float tanHalfFOV = tan(glm::radians(fov) / 2.0f);

	float A = (-f - n) / (f - n);
	float B = (2 * (n * f)) / (n - f);

	P[0] = glm::vec4(1.0f / (aspect * tanHalfFOV), 0, 0, 0);
	P[1] = glm::vec4(0, 1.0 / tanHalfFOV, 0, 0);
	P[2] = glm::vec4(0, 0, A, -1.0f);
	P[3] = glm::vec4(0, 0, B, 0.0f);

	return P;
}