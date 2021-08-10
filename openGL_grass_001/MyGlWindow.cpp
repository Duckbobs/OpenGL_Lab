#include "MyGlWindow.h"

// Compute Shader /////////////////////////////////////////
std::string readFile(const char* fileName)
{
	std::string fileContent;
	std::ifstream fileStream(fileName, std::ios::in);
	if (!fileStream.is_open()) {
		printf("File %s not found\n", fileName);
		return "";
	}
	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		fileContent.append(line + "\n");
	}
	fileStream.close();
	return fileContent;
}
void printProgramLog(GLuint program)
{
	GLint result = GL_FALSE;
	int logLength;

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar* strInfoLog = new GLchar[logLength + 1];
		glGetProgramInfoLog(program, logLength, NULL, strInfoLog);
		printf("programlog: %s\n", strInfoLog);
	};
}
void printShaderLog(GLuint shader)
{
	GLint result = GL_FALSE;
	int logLength;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar* strInfoLog = new GLchar[logLength + 1];
		glGetShaderInfoLog(shader, logLength, NULL, strInfoLog);
		printf("shaderlog: %s\n", strInfoLog);
	};
}
GLuint loadComputeShader(const char* computeShaderFile)
{

	GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);

	// Read shaders
	std::string computeShaderStr = readFile(computeShaderFile);
	const char* computeShaderSrc = computeShaderStr.c_str();

	GLint result = GL_FALSE;

	std::cout << "Compiling compute shader." << std::endl;
	glShaderSource(computeShader, 1, &computeShaderSrc, NULL);
	glCompileShader(computeShader);

	printShaderLog(computeShader);

	std::cout << "Linking program" << std::endl;
	GLuint program = glCreateProgram();
	glAttachShader(program, computeShader);
	glLinkProgram(program);
	printProgramLog(program);

	glDeleteShader(computeShader);

	return program;
}
// Compute Shader ////////////////////////////////////////

// Utils /////////////////////////////////////////////////
float rad2deg(double radian)
{
	return radian * 180 / 3.141592;
}
float deg2rad(double degree)
{
	return degree * 3.141592 / 180;
}
float getSign(float x) {
	if (x > 0) return 1;
	if (x < 0) return -1;
	return 0;
}
glm::vec3 rad2normal(float rad) {
	glm::vec3 velocity = glm::vec3(0, 1, 1) * glm::rotate(glm::mat3(1.0f), rad);
	velocity = glm::vec3(velocity.x, 0, velocity.y);
	return glm::normalize(velocity);
}
glm::vec3 deg2normal(float degree) {
	float rad = deg2rad(degree);
	glm::vec3 velocity = glm::vec3(0, 1, 1) * glm::rotate(glm::mat3(1.0f), rad);
	velocity = glm::vec3(velocity.x, 0, velocity.y);
	return glm::normalize(velocity);
}
float angle_difference(int a, int b) {
	// 두 각도의 차이를 -180~180 사이의 값으로 반환
	int d = std::abs(a - b) % 360; 
	int r = ( d > 180 ) ? 360 - d : d;

	//calculate sign 
	int sign = (a - b >= 0 && a - b <= 180) || (a - b <= -180 && a - b >= -360) ? 1 : -1;
	r *= sign;
	return r;
}
// Utils /////////////////////////////////////////////////

// TODO //////////////////////////////////////////////////
/*
	# 그림자

	float constant;
	float linear;
	float quadratic;
	# 거리 값 추가

	# 거리값 자동 계산, 샘플

	# gui 빛 조정

	# trans rotate scale 조절 ui

	# 라이트 보완 https://heinleinsgame.tistory.com/19?category=757483
	# gui 라이트 성능 조절

	# 멀티 쉐이더
	# 멀티 라이팅

	# 머터리얼 샘플
*/
// TODO //////////////////////////////////////////////////


void MyGlWindow::setupBuffer()
{
	shaderProgram = new ShaderProgram();
	shaderProgram->initFromFiles("sampler.vert", "sampler.frag"); // 쉐이더 지정
	for (int i = 0; i < 1; i++)
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
	shaderProgram->addUniform("u_viewProjection");

	shaderProgram_plane = new ShaderProgram();
	shaderProgram_plane->initFromFiles("plane.vert", "plane.frag"); // 쉐이더 지정
	for (int i = 0; i < 1; i++)
	{
		std::string name;
		name = "Light[" + std::to_string(i) + "].Position";
		shaderProgram_plane->addUniform(name);
		name = "Light[" + std::to_string(i) + "].Intensity";
		shaderProgram_plane->addUniform(name);
	}
	shaderProgram_plane->addUniform("Material.Ka");
	shaderProgram_plane->addUniform("Material.Kd");
	shaderProgram_plane->addUniform("Material.Ks");
	shaderProgram_plane->addUniform("Material.Shiness");
	shaderProgram_plane->addUniform("u_viewProjection");


	//Create the buffer the compute shader will write to
	computeShaderProgram_test = loadComputeShader("compute.comp"); // 쉐이더 지정

	char* directory;
	directory = (char*)"assets/Grass.fbx";
	m_model = new Model(directory);
	m_plane = new Plane(1000.0f, 1000.0f, 20, 20);
	//m_line = new Line(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
}


// 청크 사이즈
float chunkSize = 10.0f; // 가로 세로
int chunkWidth = 150;
std::vector<int> chunks[150][150]; // 100X100
// 주어진 좌표에 해당하는 청크 반환하는 함수
std::vector<int>* GetChunk(float x, float z)
{
	return &chunks[(int)glm::ceil(chunkWidth / 2 + x / chunkSize)][(int)glm::ceil(chunkWidth / 2 + z / chunkSize)];
};
// 주어진 좌표와 범위 내에 해당되는 인스턴스들의 ID 반환하는 함수
std::vector<std::vector<int>*> rangedChunks; // 계산용 변수
std::vector<std::vector<int>*>* GetInstancesOnChunks(float x, float z, float range) {
	rangedChunks.clear();

	int offset = chunkWidth / 2;

	int xBegin = glm::clamp(offset + (int)glm::ceil((x - range) / chunkSize), 0, chunkWidth - 1);
	int zBegin = glm::clamp(offset + (int)glm::ceil((z - range) / chunkSize), 0, chunkWidth - 1);

	int xEnd = glm::clamp(offset + (int)glm::ceil((x + range) / chunkSize), 0, chunkWidth - 1);
	int zEnd = glm::clamp(offset + (int)glm::ceil((z + range) / chunkSize), 0, chunkWidth - 1);

	for (int i = xBegin; i <= xEnd; i++) {
		for (int j = zBegin; j <= zEnd; j++) {
			if(chunks[i][j].size() > 0)
				rangedChunks.push_back(&(chunks[i][j]));
		}
	}
	return &rangedChunks;
};
// 바람 구조체
struct Wind {
	glm::vec3 windPosition = glm::vec3(0);
	glm::vec3 windVector = glm::vec3(0.2f, 0, 0);
	float size = 150.0f;
};
std::vector<Wind> winds;
Wind windObject;

// MyGlWindow 초기 설정
void MyGlWindow::initialize() {
	MyGlWindow::setupBuffer();
	// 생성
	srand(clock()); // initialize random seed	
	float radius = 200.0;
	float offset = 200.0f;
	for (int i = 0; i < max_amount; i++)
	{
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)max_amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = 0; // keep height of field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;

		int ran = rand();
		float rotAngle1 = (ran % 360);
		float scale = 15.0f +rand() % 20;

		// 인스턴스 생성
		Instance instance;
		instance.setPosition(glm::vec3(x, y, z));
		instance.setRotation(glm::vec3(0, rotAngle1, 0));
		instance.setScale(glm::vec3(scale));
		instance.windVelocity = glm::vec3(0.0f);
		Instances.push_back(instance);

		GetChunk(x, z)->push_back(i);
	}

	glGenBuffers(1, &ssboHandle_model);
	glGenBuffers(1, &ssboHandle_wind);

	glGenBuffers(1, &ssboHandle_compute_test);
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

	// 바람 추가
	{ Wind wind; winds.push_back(wind); }
	//{ Wind wind; winds.push_back(wind); }
	//{ Wind wind; winds.push_back(wind); }
}

glm::mat4 view;
glm::mat4 projection;
// 매 프레임 호출 됨
void MyGlWindow::draw(float animationTime) {
	// @------------------------------------------------------------------------------@
	// | Settings																	  |
	// @------------------------------------------------------------------------------@
#pragma region Settings
	//glClearColor(0.2, 0.2, 0.2, 1); // 백그라운드 색상 선택
	glClearColor(154.0f / 255.0f, 203.0f / 255.0f, 255.0f / 255.0f, 1); // 백그라운드 색상 선택
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height); //뷰포트(viewport) 설정
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE); //Cullface를 켬
	//glCullFace(GL_BACK); //오브젝트의 back 부분을 Culling
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::vec3 eye = m_viewer->getViewPoint();
	glm::vec3 look = m_viewer->getViewCenter();
	glm::vec3 up = m_viewer->getUpVector();
	view = lookAt(eye, look, up);
	projection = perspective(45.0f, 1.0f * width / height, 0.1f, 5000.0f);

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
		global::lightPos[j++] = glm::vec4(x, 5.2, z, 1.0);
	}
	{
		ImGui::Begin("Window");
		ImGui::SliderInt("Amount###SliderInt", &amount, 0, max_amount);
		ImGui::InputInt("Amount###InputInt", &amount, 0, max_amount);

		ImGui::End();
	}

	{
		ImGui::Begin("Window");
		vgm::Quat qRot = vgm::Quat(1.f, 0.f, 0.f, 0.f);
		ImGui::gizmo3D("##gizmo1", qRot /*, size,  mode */);
		mat4 modelMatrix = mat4_cast(qRot);
		ImGui::End();
	}
	{
		ImGui::Begin("Wind Position");
		for (int i = 0; i < winds.size(); i++) {
			std::string str;
			str = "windVector.x";
			ImGui::SliderFloat((str + std::to_string(i)).c_str(), &winds[i].windVector.x, -0.3f, 0.3f);
			str = "windVector.z";
			ImGui::SliderFloat((str + std::to_string(i)).c_str(), &winds[i].windVector.z, -0.3f, 0.3f);
			str = "size";
			ImGui::SliderFloat((str + std::to_string(i)).c_str(), &winds[i].size, 0.0f, 400.0f);
		}
		ImGui::End();
	}
	{
		ImGui::Begin("Object Position");
		std::string str;
		str = "Object.x";
		ImGui::SliderFloat((str).c_str(), &windObject.windPosition.x, -500.0f, 500.0f);
		str = "Object.z";
		ImGui::SliderFloat((str).c_str(), &windObject.windPosition.z, -500.0f, 500.0f);
		ImGui::End();
	}
#pragma endregion

shaderProgram_plane->use();
	if (m_plane)
	{
		for (int i = 0; i < 1; i++)
		{
			std::string name;
			name = "Light[" + std::to_string(i) + "].Position";
			glUniform4fv(shaderProgram_plane->uniform(name), 1, glm::value_ptr(lightPos[i]));
			name = "Light[" + std::to_string(i) + "].Intensity";
			glUniform3fv(shaderProgram_plane->uniform(name), 1, glm::value_ptr(lightIntensity[i]));
		}
		//glm::vec3 Ka(70.0f / 255.0f, 46.0f / 255.0f, 26.0f / 255.0f);
		glm::vec3 Ka(0.7f);
		glm::vec3 Kd(0.01f);
		glm::vec3 Ks(0.01f);
		GLfloat shiness = 0.1f;
		glUniform3fv(shaderProgram_plane->uniform("Material.Ka"), 1, glm::value_ptr(Ka));
		glUniform3fv(shaderProgram_plane->uniform("Material.Kd"), 1, glm::value_ptr(Kd));
		glUniform3fv(shaderProgram_plane->uniform("Material.Ks"), 1, glm::value_ptr(Ks));
		glUniform1fv(shaderProgram_plane->uniform("Material.Shiness"), 1, &shiness);
		glUniformMatrix4fv(shaderProgram_plane->uniform("u_viewProjection"), 1, GL_FALSE, glm::value_ptr(projection * view));
		m_plane->draw();
	}
shaderProgram_plane->disable();

int size = amount;
shaderProgram->use(); // shader 호출
	for (int i = 0; i < 1; i++)
	{
		std::string name;
		name = "Light[" + std::to_string(i) + "].Position";
		glUniform4fv(shaderProgram->uniform(name), 1, glm::value_ptr(lightPos[i]));
		name = "Light[" + std::to_string(i) + "].Intensity";
		glUniform3fv(shaderProgram->uniform(name), 1, glm::value_ptr(lightIntensity[i]));
	}
	glm::vec3 Ka(0.5f);
	glm::vec3 Kd(0.01f);
	glm::vec3 Ks(0.01f);
	GLfloat shiness = 0.1f;
	// 유니폼 변수 전달
	glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(Ka));
	glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(Kd));
	glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(Ks));
	glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &shiness);
	glUniformMatrix4fv(shaderProgram->uniform("viewProjection"), 1, GL_FALSE, glm::value_ptr(projection * view));
	glUniformMatrix4fv(shaderProgram->uniform("u_viewProjection"), 1, GL_FALSE, glm::value_ptr(projection * view));

	
// 모델 Draw 시작
	if (m_model)
	{
		// 바람 이동
		for (int i = 0; i < winds.size(); i++) {
			winds[i].windPosition += winds[i].windVector * 100.0f;
			if (glm::abs(winds[i].windPosition.x) > 800.0f)
				winds[i].windPosition.x = -glm::sign(winds[i].windPosition.x) * 500.0f;
			if (glm::abs(winds[i].windPosition.z) > 800.0f)
				winds[i].windPosition.z = -glm::sign(winds[i].windPosition.z) * 500.0f;
		}

		// 청크 적용 방식
		for (int i = 0; i < winds.size(); i++) {
			// 영향권 내의 청크 구하기
			std::vector<std::vector<int>*>* rangedChunks = GetInstancesOnChunks(winds[i].windPosition.x, winds[i].windPosition.z, winds[i].size);
			// 청크 루프
			for (int j = 0; j < rangedChunks->size(); j++) {
				std::vector<int>* chunk = (*rangedChunks)[j];
				// 흔들림 계산
				float len = glm::length(Instances[(*chunk)[0]].getPosition() - winds[i].windPosition);
				float vel = glm::max(0.0f, 1.0f - len / winds[i].size);
				glm::vec3 windVel = winds[i].windVector * vel;
				// 풀 개별 루프
				for (int k = 0; k < chunk->size(); k++) {
					int ins = (*chunk)[k];
					if (ins < size) {
						// 흔들림 적용
						Instances[ins].windVelocity += windVel;
					}
				}
			}
		}
		// 풀과의 거리 측정
		for (int now = 0; now < size; now++) {
			// 기존의 개별 계산 방식
			/*for (int i = 0; i < winds.size(); i++) {
				float len = glm::length(Instances[now].getPosition() - winds[i].windPosition);
				float vel = glm::max(0.0f, 1.0f - len / winds[i].size);
				if(vel != 0.0f)
					Instances[now].windVelocity += winds[i].windVector * vel;
			}*/
			Instances[now].windVelocity = Instances[now].windVelocity * 0.95f;
		}

		// 물체 충돌
		// 영향권 내의 청크 구하기
		std::vector<std::vector<int>*>* rangedChunks = GetInstancesOnChunks(windObject.windPosition.x, windObject.windPosition.z, windObject.size);
		// 청크 루프
		for (int j = 0; j < rangedChunks->size(); j++) {
			std::vector<int>* chunk = (*rangedChunks)[j];
			// 흔들림 계산
			glm::vec3 velocity = (Instances[(*chunk)[0]].getPosition() - windObject.windPosition);
			float len = glm::length(velocity);
			float vel = glm::max(0.0f, 1.0f - len / windObject.size);
			glm::vec3 windVel = glm::normalize(velocity) * vel * 0.6f;
			// 풀 개별 루프
			for (int k = 0; k < chunk->size(); k++) {
				int ins = (*chunk)[k];
				if (ins < size) {
					// 흔들림 적용
					Instances[ins].windVelocity += windVel;
				}
			}
		}

		// 한번에 Draw할 개수
		int drawSize = 500;
		int now, nowSize = glm::min(drawSize, size);
		for (int offset = 0; offset < size; offset += nowSize) {
			// 모델 매트릭스 계산 ( 컴퓨트 )
			std::vector<int> computeIDs;
			std::vector<InsData> insDatas_temp;
			for (int start = 0; start < nowSize; start++) {
				now = offset + start;
				if (Instances[now].updateMatrix()) {
					insDatas_temp.push_back(Instances[now].getData());
					computeIDs.push_back(now);
				}
			}
			if (computeIDs.size() != 0) {
				// 컴퓨트 셰이더 전달
				glUseProgram(computeShaderProgram_test);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboHandle_compute_test);
				glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(InsData) * insDatas_temp.size(), insDatas_temp.data(), GL_DYNAMIC_DRAW);
				glDispatchCompute(offset / drawSize + 1, 1, 1);
				InsData* ptr;
				ptr = (InsData*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
				glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
				glUseProgram(0);
				// 결과값 입력
				for (int ins = 0; ins < computeIDs.size(); ins++) {
					Instances[computeIDs[ins]].setInstanceMatrix(ptr[ins].aInstanceMatrix);
					//std::cout << computeIDs[ins] << std::endl;
				}
			}

			// 메모리 할당
			modelMatrices = new glm::mat4[nowSize];
			windVectors = new glm::vec3[nowSize];
			// ssbo 입력
			for (int start = 0; start < nowSize; start++) {
				now = offset + start;
				// 모델 매트릭스 입력
				modelMatrices[start] = Instances[now].getInstanceMatrix();
				// 바람 정보 입력
				windVectors[start] = Instances[now].windVelocity;
			}
			// 모델 매트릭스 ssbo 전달
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboHandle_model);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * nowSize, modelMatrices, GL_STATIC_DRAW);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboHandle_model);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			// 바람 정보 ssbo 전달
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboHandle_wind);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec3) * nowSize, windVectors, GL_STATIC_DRAW);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssboHandle_wind);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

			// 드로우
			m_model->Draw(shaderProgram, nowSize);

			// 메모리 반환
			delete[] modelMatrices;
			delete[] windVectors;
			nowSize = glm::min(drawSize, size - offset);
		}
	}
// 모델 Draw 끝
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
void MyGlWindow::mouse_button_click(double xpos, double ypos) {
	float x = (2.0f * xpos) / width - 1.0f;
	float y = 1.0f - (2.0f * ypos) / height;
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);
	glm::vec4 ray_clip = glm::vec4(glm::vec2(ray_nds.x, ray_nds.y), -1.0, 1.0);
	glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
	ray_eye = glm::vec4(glm::vec2(ray_eye.x, ray_eye.y), -1.0, 0.0);
	glm::vec3 ray_wor = glm::vec3(inverse(view) * ray_eye);
	// don't forget to normalise the vector at some point
	ray_wor = glm::normalize(ray_wor);
	mouse_ray = ray_wor;
	//std::cout << glm::to_string(ray_wor) << std::endl;
}