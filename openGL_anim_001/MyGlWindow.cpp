#include "MyGlWindow.h"

// Compute Shader //////////////////////////////////////////////////
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
// Compute Shader //////////////////////////////////////////////////

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

float angle_difference(int a, int b) {
	// ?? ?????? ?????? -180~180 ?????? ?????? ????
	int d = std::abs(a - b) % 360; 
	int r = ( d > 180 ) ? 360 - d : d;

	//calculate sign 
	int sign = (a - b >= 0 && a - b <= 180) || (a - b <= -180 && a - b >= -360) ? 1 : -1;
	r *= sign;
	return r;
}
// TODO
/*
	# ??????

	float constant;
	float linear;
	float quadratic;
	# ???? ?? ????

	# ?????? ???? ????, ????

	# gui ?? ????

	# trans rotate scale ???? ui

	# ?????? ???? https://heinleinsgame.tistory.com/19?category=757483
	# gui ?????? ???? ????

	# ???? ??????
	# ???? ??????

	# ???????? ????
*/





void MyGlWindow::setupBuffer()
{
	shaderProgram = new ShaderProgram();
	shaderProgram->initFromFiles("sampler.vert", "sampler.frag"); // ?????? ????

	shaderProgram_plane = new ShaderProgram();
	shaderProgram_plane->initFromFiles("plane.vert", "plane.frag"); // ?????? ????

	shaderProgram_gizmo = new ShaderProgram();
	shaderProgram_gizmo->initFromFiles("gizmo.vert", "gizmo.frag"); // ?????? ????

	//Create the buffer the compute shader will write to

	computeShaderProgram_test = loadComputeShader("compute.comp"); // ?????? ????

	glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	char* directory;
	directory = (char*)"assets/human/human2.fbx";
	//directory = (char*)"assets/fast2.fbx";
	//directory = (char*)"assets/suit/scene.fbx";
	m_model = new Model(directory);
	m_plane = new Plane(5000.0f, 5000.0f, 20, 20);
	m_line = new Line(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
}

int lab_instance = 0;
float duration1;
float INDEX_PER_FRAME = 30;
void MyGlWindow::initialize() {
	MyGlWindow::setupBuffer();

	shaderProgram_gizmo->addUniform("mvp");

	/////////////////////////////////////////////
	// ??????
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
	shaderProgram->addUniform("viewProjection");
	shaderProgram->addUniform("dqsize");
	/////////////////////////////////////////////
	// ??????
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
	shaderProgram_plane->addUniform("viewProjection");
	/////////////////////////////////////////////


	// ?????????? dqs ????

// Duration mTime ???? ?????
	// m_model->getDuration() = 11.8000002
	// getDuration = ???? ?????

	duration1 = m_model->getDuration() * INDEX_PER_FRAME;
	for (float animationTime = 0; animationTime < duration1; animationTime++) {
		int index = fmod(animationTime, duration1);

		if (m_model->modelData.animationMatricesExists[index] == false) {
			if (m_model->BoneTransform(fmod(animationTime, duration1) / duration1, dualQuaternions)) {
				max_index = index;
				std::cout << index << std::endl;
				for (unsigned int i = 0; i < dualQuaternions.size(); ++i) {
					glm::highp_mat2x4 mat = glm::mat2x4_cast(dualQuaternions[i]);
					(m_model->modelData.animationMatrices[index]).push_back(mat);
				}
				m_model->modelData.animationMatricesExists[index] = true;
			}
			else {
				break;
			}
		}
	}

	/*
	duration1 = m_model->getDuration() * INDEX_PER_FRAME;
	for (float animationTime = 0; animationTime <= duration1; animationTime++) {
		//int index = std::max(1, (int)(glm::round(fmod(animationTime, duration1))));
		int index = fmod(animationTime, duration1+0.001f);
		if(index > max_index)
			max_index = index;
		std::cout << index << std::endl;

		if (m_model->modelData.animationMatricesExists[index] == false) {
			// ?? ???? ????, lerp ?????? 2????
			m_model->BoneTransform(animationTime, dualQuaternions);
			//m_model->BoneTransform(index / INDEX_PER_FRAME, dualQuaternions2);
			//m_model->BoneTransform(std::fmax(1, fmod(animationTime + INDEX_PER_FRAME, duration1)) / INDEX_PER_FRAME, dualQuaternions2);

			// ?? ???? ????, lerp ???? push
			for (unsigned int i = 0; i < dualQuaternions.size(); ++i) {
				glm::highp_mat2x4 mat = glm::mat2x4_cast(dualQuaternions[i]);
				(m_model->modelData.animationMatrices[index]).push_back(mat);

				//glm::quat v1 = glm::slerp(dualQuaternions[i][0], dualQuaternions2[i][0], (float)fmod(index, INDEX_PER_FRAME) / INDEX_PER_FRAME);
				//glm::quat v2 = glm::slerp(dualQuaternions[i][1], dualQuaternions2[i][1], (float)fmod(index, INDEX_PER_FRAME) / INDEX_PER_FRAME);
				//glm::highp_mat2x4 mat = { {v1.x, v1.y, v1.z, v1.w}, {v2.x, v2.y, v2.z, v2.w} };
				//std::cout << (float)fmod(index, INDEX_PER_FRAME) / INDEX_PER_FRAME << std::endl;
				//(m_model->modelData.animationMatrices[index]).push_back(mat);
			}
			m_model->modelData.animationMatricesExists[index] = true;
		}
	}*/
	// ?????????? dqs ????
	/*int total_index = 1;
	float duration1 = m_model->getDuration();
	for (float animationTime = 0; animationTime < duration1; animationTime++) {
		int index = std::max(1, (int)(fmod(animationTime, duration1)));
		//max_index = index;
		//std::cout << index << std::endl;

		m_model->BoneTransform(index, dualQuaternions);
		if (m_model->modelData.animationMatricesExists[total_index] == false) {
			for (float ind = 0; ind < INDEX_PER_FRAME; ++ind) {
				for (unsigned int i = 0; i < dualQuaternions.size(); ++i) {
					//glm::highp_mat2x4 mat = glm::mat2x4_cast(dualQuaternions[i]);
					glm::quat v1 = glm::slerp(dualQuaternions[i][0], dualQuaternions[i][0], ind / INDEX_PER_FRAME);
					glm::quat v2 = glm::slerp(dualQuaternions[i][1], dualQuaternions[i][1], ind / INDEX_PER_FRAME);
					glm::highp_mat2x4 mat = { {v1.x, v1.y, v1.z, v1.w}, {v2.x, v2.y, v2.z, v2.w} };

					(m_model->modelData.animationMatrices[total_index]).push_back(mat);
				}
				m_model->modelData.animationMatricesExists[total_index] = true;

				std::cout << total_index << std::endl;
				total_index += 1;
				max_index = total_index - 2;
			}
		}
	}*/
	/////////////////////////////////////////////
	// ???????? ????
	{
		Instance instance;
		instance.setAnimationOffset(rand());
		instance.setPosition(glm::vec3(0, 0, 0));
		instance.setScale(glm::vec3(0.1f));
		instance.speed = 0.5f;
		instance.setVelocity(glm::vec3(0));
		Instances.push_back(instance);
	}
	// ???????? ????
	{
		Instance instance;
		instance.setAnimationOffset(rand());
		instance.setPosition(glm::vec3(10, 0, 0));
		instance.setScale(glm::vec3(0.1f));
		instance.speed = 1.0f;
		instance.setVelocity(glm::vec3(0));
		Instances.push_back(instance);
	}
	// ???????? ????
	{
		Instance instance;
		instance.setAnimationOffset(rand());
		instance.setPosition(glm::vec3(20, 0, 0));
		instance.setScale(glm::vec3(0.1f));
		instance.speed = 2.0f;
		instance.setVelocity(glm::vec3(0));
		Instances.push_back(instance);
	}
	// ???????? ????
	{
		Instance instance;
		instance.setAnimationOffset(rand());
		instance.setPosition(glm::vec3(30, 0, 0));
		instance.setScale(glm::vec3(0.1f));
		instance.speed = 3.0f;
		instance.setVelocity(glm::vec3(0));
		Instances.push_back(instance);
	}
	/////////////////////////////////////////////
	lab_instance = Instances.size();
	// ????
	srand(clock()); // initialize random seed	
	float radius = 200.0;
	float offset = 100.0f;
	for (unsigned int i = lab_instance; i < max_amount; i++)
	{
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)max_amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = 0; // keep height of field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;

		float scale = .1f;
		float rotAngle = (rand() % 360);

		// ???????? ????
		Instance instance;
		instance.setAnimationOffset(rand());
		instance.setPosition(glm::vec3(x, y, z));
		instance.setRotation(glm::vec3(0.0f, rotAngle, 0.0f));
		instance.setScale(glm::vec3(0.1f));
		instance.speed = 0.5f + (rand() % 100000) * 0.00001f;

		instance.setVelocity(rad2normal(deg2rad(rotAngle)) * instance.speed);
		Instances.push_back(instance);


		glm::vec3 destDir = glm::normalize(instance.getPosition());
		float destRadian = atan2f(destDir.x, destDir.z);
		destRadian = deg2rad(rad2deg(destRadian) + 90);
		instance.setVelocity(rad2normal(destRadian) * instance.speed);
		instance.setRotation(glm::vec3(0, destRadian, 0));
	}
	dqsMatrices = new glm::mat2x4[max_amount * m_model->modelData.m_NumBones];

	glGenBuffers(1, &ssboHandle_t);  // dq mat buffer
	glGenBuffers(1, &ssboHandle_ins);  //transformation

	glGenBuffers(1, &ssboHandle_compute_test);

// TODO
// ?????? ?????????? offset(0.1?? ????)?? ?????????? ?????? instancing ????
// ???? ?????????? ???? ????
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

std::vector<InsData> insDatas;
glm::vec3 Dest = glm::vec3(0, 0, 0);
glm::mat4 view;
glm::mat4 projection;
const glm::mat4 _mat = glm::mat4(1.0f);
const glm::vec3 _vec = glm::vec3(1, 1, 1);
// ?? ?????? ???? ??
void MyGlWindow::draw(float animationTime) {
	// @------------------------------------------------------------------------------@
	// | Settings																	  |
	// @------------------------------------------------------------------------------@
#pragma region Settings
	glClearColor(0.2, 0.2, 0.2, 1); // ?????????? ???? ????
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height); //??????(viewport) ????
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE); //Cullface?? ??
	//glCullFace(GL_BACK); //?????????? back ?????? Culling
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#pragma endregion
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
		//Radian???? ????, ?? ???? ??????? Glm???? ????
		float ang = (i * glm::pi<float>() / 180.0f);

		float x = 5.0f * cosf(ang);
		float z = 5.0f * sinf(ang);
		glm::vec4 lightPos;
		global::lightPos[j++] = glm::vec4(x, 5.2, z, 1.0);
	}

	glm::vec3 Ka(0.5, 0.5, 0.5);
	glm::vec3 Kd(0.1, 0.1, 0.1);
	glm::vec3 Ks(0.01, 0.01, 0.01);
	GLfloat shiness = 1.0f;



shaderProgram_plane->use();
	for (int i = 0; i < 1; i++)
	{
		std::string name;
		name = "Light[" + std::to_string(i) + "].Position";
		glUniform4fv(shaderProgram_plane->uniform(name), 1, glm::value_ptr(lightPos[i]));
		name = "Light[" + std::to_string(i) + "].Intensity";
		glUniform3fv(shaderProgram_plane->uniform(name), 1, glm::value_ptr(lightIntensity[i]));
	}
	glUniform3fv(shaderProgram_plane->uniform("Material.Ka"), 1, glm::value_ptr(Ka));
	glUniform3fv(shaderProgram_plane->uniform("Material.Kd"), 1, glm::value_ptr(Kd));
	glUniform3fv(shaderProgram_plane->uniform("Material.Ks"), 1, glm::value_ptr(Ks));
	glUniform1fv(shaderProgram_plane->uniform("Material.Shiness"), 1, &shiness);
	glUniformMatrix4fv(shaderProgram_plane->uniform("viewProjection"), 1, GL_FALSE, glm::value_ptr(projection * view));
	if (m_plane)
	{
		m_plane->draw();
	}
shaderProgram_plane->disable();



	// 0.03?? = ???????? ????
	// 0.06?? = ???????? ????
	// 0.1?? = ???????? ?????? ????, ???????? ????
	// 0.2?? = ???????? ????
	{
		ImGui::Begin("Window");
		ImGui::SliderInt("Amount###SliderInt", &amount, 0, max_amount);
		ImGui::SliderFloat("AnimationSpeed###SliderFloat", &animationSpeed, 0, 10);

		ImGui::SliderFloat("AnimationSpeed-1###SliderFloat-1", &animationSpeed_1, 0, 10);
		ImGui::SliderFloat("AnimationSpeed-2###SliderFloat-2", &animationSpeed_2, 0, 10);
		ImGui::SliderFloat("AnimationSpeed-3###SliderFloat-3", &animationSpeed_3, 0, 10);

		ImGui::SliderInt("Index###SliderIntIndex", &lab_index, 1, max_index);
		
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
		ImGui::Begin("Window");
		ImGui::InputFloat("x###InputFloatX", &Dest[0]);
		ImGui::InputFloat("y###InputFloatY", &Dest[1]);
		ImGui::InputFloat("z###InputFloatZ", &Dest[2]);
		ImGui::End();
	}

	int size = amount;

	float diff;
	float angleSpeed = 1;
	glm::mat3 mat = glm::mat3(1.0f);
	insDatas.clear();
	for (int ins = 0; ins < size; ins++)
	{
		// ???? ???????? ( Velocity )
		Instances[ins].Update();
		
		if (ins >= lab_instance) {
			// ???? ???????? ???????? ????
			glm::vec3 destDir = glm::normalize(Dest - Instances[ins].getPosition());
			float destRadian = atan2f(destDir.x, destDir.z);
			destRadian = deg2rad(rad2deg(destRadian) + 90);
			Instances[ins].setVelocity(rad2normal(destRadian) * Instances[ins].speed);
			Instances[ins].setRotation(glm::vec3(0, destRadian, 0));
		}
		else if(ins != 0) {
			Instances[ins].setVelocity(glm::vec3(0, 0, 1) * Instances[ins].speed);
			glm::vec3 pos = Instances[ins].getPosition();
			if (pos.z > 50) {
				Instances[ins].setPosition(glm::vec3(pos.x, pos.y, 0));
			}
		}
		
		/*
		glm::vec3 oldVelocity = Instances[ins].getVelocity();
		glm::vec3 destDir = glm::normalize(Dest - Instances[ins].getPosition());
// Steering Rotation
		float oldRadian = atan2f(oldVelocity.x, oldVelocity.z);
		float destRadian = atan2f(destDir.x, destDir.z);
		float newRadian = oldRadian;

		if (oldRadian != destRadian) {
			diff = angle_difference(rad2deg(newRadian), rad2deg(destRadian));
			if (std::abs(diff) < angleSpeed) {
				newRadian = destRadian;
			}
			else {
				if (diff < 0) {
					newRadian = deg2rad(rad2deg(newRadian) + angleSpeed);
				}
				else {
					newRadian = deg2rad(rad2deg(newRadian) - angleSpeed);
				}
			}
			// Update
			Instances[ins].setVelocity(rad2normal(newRadian) * Instances[ins].speed);
			Instances[ins].setRotation(glm::vec3(0, newRadian, 0));
		}
		else {
			// ???? X
		}
		*/

// TODO // ???? ????????, ???????? ???? ????, ???? ???????? ?????????? ???? ??????.
		// ???? ???????? ???????? ( ???? ???? ???? ???? )
		if (Instances[ins].updateMatrix()) {
			insDatas.push_back(Instances[ins].getData());
		}

		// ?????????? ????????
		float TimeInTicks = (animationTime + Instances[ins].getAnimationOffset()) * animationSpeed * Instances[ins].speed;
		if (ins == 1) {
			TimeInTicks = (animationTime + Instances[ins].getAnimationOffset()) * animationSpeed_1 * Instances[ins].speed;
		}
		if (ins == 2) {
			TimeInTicks = (animationTime + Instances[ins].getAnimationOffset()) * animationSpeed_2 * Instances[ins].speed;
		}
		if (ins == 3) {
			TimeInTicks = (animationTime + Instances[ins].getAnimationOffset()) * animationSpeed_3 * Instances[ins].speed;
		}

		int index = std::max(1, (int)(fmod(TimeInTicks * INDEX_PER_FRAME * 20, max_index))); // ???? ?????? ????
		if (ins == 0) {
			index = std::max(1, lab_index);
		}
		if (ins == 1) {
			std::cout << index << std::endl;
		}
		//index = std::max(1, index);
		std::vector<glm::mat2x4>* animationMatrices = &(m_model->modelData.animationMatrices[index]);
		for (unsigned int i = 0; i < dualQuaternions.size(); ++i)
			dqsMatrices[dualQuaternions.size() * ins + i] = (*animationMatrices)[i];
	}

//////////////////////// ?????? ???????? ???? START
	unsigned int loopsize = 500;
	std::vector<InsData> insDatas_temp;
	unsigned int loop = 0;
	insDatas_temp.clear();
	for (unsigned int pos = 0; pos < size; pos++) {
		insDatas_temp.push_back(insDatas[pos]);
		if (insDatas_temp.size() >= loopsize) {
			glUseProgram(computeShaderProgram_test);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboHandle_compute_test);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(InsData) * insDatas_temp.size(), insDatas_temp.data(), GL_DYNAMIC_DRAW);

			glDispatchCompute(loop + 1, 1, 1);

			InsData* ptr;
			ptr = (InsData*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
			glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
			glUseProgram(0);
			for (unsigned int ins = 0; ins < insDatas_temp.size(); ins++) {
				Instances[ins + loop * loopsize].setInstanceMatrix(ptr[ins].aInstanceMatrix);
			}
			insDatas_temp.clear();
			loop++;
		}
	}
	if (insDatas_temp.size() != 0) {
		glUseProgram(computeShaderProgram_test);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboHandle_compute_test);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(InsData) * insDatas_temp.size(), insDatas_temp.data(), GL_DYNAMIC_DRAW);

		glDispatchCompute(loop + 1, 1, 1);

		InsData* ptr;
		ptr = (InsData*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glUseProgram(0);
		for (unsigned int ins = 0; ins < insDatas_temp.size(); ins++) {
			Instances[ins + loop * loopsize].setInstanceMatrix(ptr[ins].aInstanceMatrix);
		}
	}
	insDatas.clear();
	insDatas_temp.clear();
//////////////////////// ?????? ???????? ???? END


shaderProgram->use(); // shader ????
	for (int i = 0; i < 1; i++)
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
	glUniformMatrix4fv(shaderProgram->uniform("viewProjection"), 1, GL_FALSE, glm::value_ptr(projection * view));

	glUniform1ui(shaderProgram->uniform("dqsize"), dualQuaternions.size());

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboHandle_t);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat2x4) * dualQuaternions.size() * size, dqsMatrices, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssboHandle_t);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	modelMatrices = new glm::mat4[size];
	for (int ins = 0; ins < size; ins++)
	{
		modelMatrices[ins] = Instances[ins].getInstanceMatrix();
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboHandle_ins);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * size, modelMatrices, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboHandle_ins);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	if (m_model)
	{
		m_model->Draw(shaderProgram, size);
	}

shaderProgram->disable();
delete[] modelMatrices;

shaderProgram_gizmo->use();
	glUniformMatrix4fv(shaderProgram_gizmo->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f) * projection * view));

	if (m_line)
	{
		//m_line->draw();
	}
shaderProgram_gizmo->disable();





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