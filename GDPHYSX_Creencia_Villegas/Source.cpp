#pragma once
#include "projectile.h"
#include "ObjectPooling.h"

projectile::projectileData projectile::createBullet(int type)
{

	float randX = rand() % 30 + -30;
	float randY = rand() % 15 + -5;
	float randZ = rand() % 10 + -10;
	projectileData newData;
	switch (type) {
	case 0:
		newData.velo = glm::vec3(0.0f, 0.0f, 0.0f);
		newData.accel = glm::vec3(0.0f, 0.0f, 0.0f);
		newData.mass = 5.0f;
		newData.damp = 1.0f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.count = 1;
		newData.type = 0;
		break;
	case 1:
		newData.velo = glm::vec3(35.0f, 0.0f, 0.0f);
		newData.accel = glm::vec3(0.0f, -1.0f, 0.0f);
		newData.mass = 2.0f;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.count = 1;
		newData.type = 1;
		break;
	case 2:
		newData.velo = glm::vec3(40.0f, 30.0f, 0.0f);
		newData.accel = glm::vec3(0.0f, -20.0f, 0.0f);
		newData.mass = 200.0f;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.count = 1;
		newData.type = 2;
		break;
	case 3:
		newData.velo = glm::vec3(10.0f, 0.0f, 0.0f);
		newData.accel = glm::vec3(0.0f, 0.6f, 0.0f);
		newData.mass = 1.0f;
		newData.damp = 0.9f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.count = 1;
		newData.type = 3;
		break;
	case 4:
		newData.velo = glm::vec3(100.0f, 0.0f, 0.0f);
		newData.accel = glm::vec3(0.0f, 0.0f, 0.0f);
		newData.mass = 0.1f;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.count = 1;
		newData.type = 4;
		break;
	case 5:
		newData.velo = glm::vec3(randX, 50.0f, 0.0f);
		newData.accel = glm::vec3(0.0f, 0.0f, 0.0f);
		newData.mass = 1.0;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.count = 3;
		newData.type = 5;
		break;
	case 6:
		newData.velo = glm::vec3(randX, randY, randZ);
		newData.accel = glm::vec3(0.0f, -2.0, 0.0f);
		newData.mass = 1.0;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.count = 5;
		newData.type = 6;
		break;
	case 7:
		newData.velo = glm::vec3(randX, randY, randZ);
		newData.accel = glm::vec3(0.0f, -5.0f, 0.0f);
		newData.mass = 1.0;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.count = 0;
		newData.type = 6;
		break;
	}

	return newData;
}

projectile::projectileData projectile::fireBullet(
	std::vector<ObjData>* particleArray, std::vector<glm::mat4>* particleTrans,
	std::vector<glm::mat4>* normalTransArray, std::vector<GLuint>* textureArray,
	GLuint modelTransLoc, GLuint normTransLoc, int bType, glm::vec3 currParticlePos)
{
	projectile newparticle;
	newparticle.pData = newparticle.createBullet(bType);


	ObjData particle;
	LoadObjFile(&particle, "earth/Earth.obj");
	GLfloat particleOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&particle,
		1.0f,
		particleOffsets
	);

	glm::mat4 trans = glm::mat4(1.0f); // identity
	trans = glm::mat4(1.0f); // identity
	if (bType == 6) trans = glm::translate(trans, currParticlePos);
	else if (bType == 7) trans = glm::translate(trans, currParticlePos);
	if (bType == 0) trans = glm::scale(trans, glm::vec3(0.0f, 0.0f, 0.0f));
	else trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 normalTrans;
	GLuint particleTexture;

	particleArray->push_back(particle);
	particleTrans->push_back(trans);
	normalTransArray->push_back(normalTrans);
	textureArray->push_back(particleTexture);


	/*
	std::cout << std::endl
		<< "particleArray = " << particleArray->size() << std::endl
		<< "particleTrans = " << particleTrans->size() << std::endl
		<< "normalTransArray = " << normalTransArray->size() << std::endl
		<< "textureArray = " << textureArray->size() << std::endl << std::endl << std::endl;
	*/

	return newparticle.pData;
}

void projectile::deleteBullet(
	std::vector<ObjData>* particleArray, std::vector<glm::mat4>* particleTrans,
	std::vector<glm::mat4>* normalTransArray, std::vector<GLuint>* textureArray,
	std::vector<projectileData>* particleDatas, int index)
{
	particleArray->erase(particleArray->begin() + index);
	particleTrans->erase(particleTrans->begin() + index);
	normalTransArray->erase(normalTransArray->begin() + index);
	textureArray->erase(textureArray->begin() + index);
	particleDatas->erase(particleDatas->begin() + index);
}

float getDistance(float xPos1, float yPos1, float zPos1, float xPos2, float yPos2, float zPos2) {
	//std::cout	<< xPos1 << ", " << yPos1 << ", " << zPos1 << std::endl << xPos2 << ", " << yPos2 << ", " << zPos2 << std::endl << std::endl;
	return sqrt(pow(xPos2 - xPos1, 2) + pow(yPos2 - yPos1, 2) + pow(zPos2 - zPos1, 2));
}

#pragma region CAMERA VARIABLES
//YouTube. (2019). OpenGL - camera movement. YouTube. https://www.youtube.com/watch?v=AWM4CUfffos.
//camera
glm::vec3 cameraPos = glm::vec3(10.0f, 30.0f, 30.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

//mouse state
bool firstMouse = true;
float lastX = 1024 / 2.0;
float lastY = 768 / 2.0;

#pragma endregion

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float mouseOffsetX = xpos - lastX;
	float mouseOffsetY = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	mouseOffsetX *= sensitivity;
	mouseOffsetY *= sensitivity;

	yaw += mouseOffsetX;
	pitch += mouseOffsetY;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}



	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int main() {

	
	ObjectPool <int>projectilePool1(3);

	int* myProjectile1 = projectilePool1.alloc();
	int* myProjectile2 = projectilePool1.alloc();
	int* myProjectile3 = projectilePool1.alloc();
	int* myProjectile4 = projectilePool1.alloc(); 

	projectilePool1.dealloc(myProjectile1);
	projectilePool1.dealloc(myProjectile2);
	projectilePool1.dealloc(myProjectile3);





	stbi_set_flip_vertically_on_load(true);
#pragma region Initialization
	//initialize glfw
	if (glfwInit() != GLFW_TRUE) {
		fprintf(stderr, "Failed to initialized! \n");
		return -1;
	}

	// set opengl version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window 
	GLFWwindow* window;
	window = glfwCreateWindow(1024, 768, "Creencia_Alain", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to load window! \n");
		return -1;
	}
	glfwMakeContextCurrent(window);

	//initialize glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
#pragma endregion


#pragma region Particle Physics Declarations

	std::vector<ObjData> particleArray;
	std::vector<glm::mat4> particleTrans;
	std::vector<glm::mat4> normalTransArray;
	std::vector<GLuint> textureArray;
	std::vector<projectile::projectileData> particleDatas;
	projectile tempparticle;
	int currType = 1;
	bool isCollide = false;
	float boxRadius = 5.5;
	float boxMass = 25;
	float cooldown = 1;
	float force = 0;
	float stopper = 0;

#pragma endregion

#pragma region Mesh Loading

	ObjData box;
	LoadObjFile(&box, "box/12279_Container_v1_l1.obj");
	GLfloat boxOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&box,
		0.2f,
		boxOffsets
	);

	std::vector<std::string> faces
	{
		"right.png",
		"left.png",
		"bottom.png",
		"top.png",
		"front.png",
		"back.png"
	};
	//SkyBoxData skybox = LoadSkybox("Assets/skybox", faces);

#pragma endregion

#pragma region Shader Loading
	//GLuint skyboxShaderProgram = LoadShaders("Shaders/skybox_vertex.shader", "Shaders/skybox_fragment.shader");
	//GLuint shaderProgram = LoadShaders("Shaders/phong_vertex.shader", "Shaders/phong_fragment.shader");
	//glUseProgram(shaderProgram2);
	GLuint shaderProgram = LoadShaders("Shaders/vertex.shader", "Shaders/fragment.shader");
	glUseProgram(shaderProgram);

	GLuint colorLoc = glGetUniformLocation(shaderProgram, "u_color");
	glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f);


	// initialize MVP
	GLuint modelTransformLoc = glGetUniformLocation(shaderProgram, "u_model");
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "u_view");
	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "u_projection");

	GLuint normalTransformLoc = glGetUniformLocation(shaderProgram, "u_normal");
	GLuint cameraPosLoc = glGetUniformLocation(shaderProgram, "u_camera_pos");
	GLuint ambientColorLoc = glGetUniformLocation(shaderProgram, "u_ambient_color");
	glUniform3f(ambientColorLoc, 0.1f, 0.1f, 0.1f);

	particleDatas.push_back(tempparticle.fireBullet(&particleArray, &particleTrans, &normalTransArray, &textureArray, modelTransformLoc, normalTransformLoc, 0, glm::vec3(0.0f, 0.0f, 0.0f)));

	glm::mat4 boxTrans = glm::mat4(1.0f); // identity
	boxTrans = glm::translate(boxTrans, glm::vec3(30.0f, 0.0f, -4.0f));
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(boxTrans));


	// define projection matrix
	glm::mat4 projection = glm::mat4(1.0f);
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "u_light_pos");
	GLuint lightDirLoc = glGetUniformLocation(shaderProgram, "u_light_dir");
	glUniform3f(lightDirLoc, 0.0f, 0.0f, 0.0f);

#pragma endregion

	// set bg color to green
	glClearColor(0.5f, 0.5f, 0.0f, 0.0f);


	// var for rotations
	float xFactor = 0.0f;
	float xSpeed = 1.0f;
	float currentTime = glfwGetTime();
	float prevTime = 0.0f;
	float deltaTime = 0.0f;
	float rotFactor = 0.0f;


	//depth testing
	glEnable(GL_DEPTH_TEST);

	//face culling
	glEnable(GL_CULL_FACE);

	while (!glfwWindowShouldClose(window)) {

#pragma region Viewport

		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);

#pragma endregion

#pragma region Projection

		// Orthographic with corection for stretching, resize window to see difference with previous example
		projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);

		// Perspective Projection
		projection = glm::perspective(glm::radians(90.0f), ratio, 0.1f, 300.0f),
			// Set projection matrix in shader
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

#pragma endregion

#pragma region View

		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


		//source: https://www.youtube.com/watch?v=AWM4CUfffos
		float cameraSpeed = deltaTime * 10;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			cameraPos += cameraSpeed * cameraFront;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			cameraPos -= cameraSpeed * cameraFront;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
			cameraPos += cameraSpeed * cameraUp;
		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
			cameraPos -= cameraSpeed * cameraUp;
		}

		glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.y);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


#pragma endregion

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//toggle to render wit GL_FILL or GL_LINE
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

#pragma region Draw

		//DrawSkybox(skybox, skyboxShaderProgram, view, projection);
		//glUseProgram(shaderProgram);


#pragma region Box Object

		glBindVertexArray(box.vaoId);
		glUseProgram(shaderProgram);

		glm::mat4 boxNormalTrans = glm::transpose(glm::inverse(boxTrans));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(boxNormalTrans));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(boxTrans));

		glActiveTexture(GL_TEXTURE0);
		//GLuint boxTexture = box.textures[box.materials[0].diffuse_texname];
		//glBindTexture(GL_TEXTURE_2D, boxTexture);
		glDrawElements(GL_TRIANGLES, box.numFaces, GL_UNSIGNED_INT, (void*)0);

#pragma endregion

#pragma region particle

		glBindVertexArray(particleArray[0].vaoId);
		glUseProgram(shaderProgram);
		glActiveTexture(GL_TEXTURE0);

		// Collider
		for (int i = 1; i < particleArray.size(); i++)
		{
			particleDatas[i].ageLimit -= 1 * deltaTime;
			if (getDistance(particleTrans[i][3].x, particleTrans[i][3].y, particleTrans[i][3].z, boxTrans[3].x, boxTrans[3].y, boxTrans[3].z) <= particleDatas[i].radius + boxRadius) {
				std::cout << std::endl << "NAGCOLLIDE NA" << std::endl;

				///////////////////////////////// CHANGE TO E=MC^2 FORMULA
				force = (particleDatas[i].mass * particleDatas[i].velo.x) / boxMass;
				stopper = force;
				/////////////////////////////////

				tempparticle.deleteBullet(&particleArray, &particleTrans, &normalTransArray, &textureArray, &particleDatas, i);

			}
			else if (particleDatas[i].ageLimit <= 0)
			{
				std::cout << "type = " << particleDatas[i].type << std::endl;
				std::cout << "count = " << particleDatas[i].count << std::endl << std::endl;

				if (particleDatas[i].type == 5)
				{
					for (int j = 0; j < particleDatas[i].count; j++)
					{
						particleDatas.push_back(tempparticle.fireBullet(&particleArray, &particleTrans, &normalTransArray, &textureArray, modelTransformLoc, normalTransformLoc, 6, particleTrans[i][3]));
					}
				}
				else if (particleDatas[i].type == 6)
				{
					for (int k = 0; k < particleDatas[i].count; k++)
					{
						particleDatas.push_back(tempparticle.fireBullet(&particleArray, &particleTrans, &normalTransArray, &textureArray, modelTransformLoc, normalTransformLoc, 7, particleTrans[i][3]));
					}
				}

				tempparticle.deleteBullet(&particleArray, &particleTrans, &normalTransArray, &textureArray, &particleDatas, i);
			}
		}

		///////////////////////////////// CHANGE TO E=MC^2 FORMULA
		boxTrans = glm::translate(boxTrans, glm::vec3(force, 0.0f, 0.0f) * deltaTime);
		if (force >= 0) force -= stopper * deltaTime;
		else force = 0;
		/////////////////////////////////


		// transforms
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) currType = 1;
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) currType = 2;
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) currType = 3;
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) currType = 4;
		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) currType = 5;

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) particleTrans[0] = glm::rotate(particleTrans[0], glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix

		cooldown += cooldown * deltaTime;
		if (cooldown >= 1.5)
		{
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				particleDatas.push_back(tempparticle.fireBullet(&particleArray, &particleTrans, &normalTransArray, &textureArray, modelTransformLoc, normalTransformLoc, currType, glm::vec3(0.0f, 0.0f, 0.0f)));
				cooldown = 1;
			}
		}

		for (int i = 1; i < particleArray.size(); i++) {
			particleDatas[i].velo += particleDatas[i].accel * deltaTime;
			particleTrans[i] = glm::translate(particleTrans[i], particleDatas[i].velo * deltaTime); // matrix * translate_matrix
			//std::cout << std::endl << "currVELOS" << i << " = " << particleDatas[i].velo.x << ", " << particleDatas[i].velo.y << ", " << particleDatas[i].velo.z << std::endl;
			//std::cout << "particleTRANS" << i << " = " << particleTrans[i][3].x << ", " << particleTrans[i][3].y << ", " << particleTrans[i][3].z << std::endl;

			normalTransArray[i] = glm::transpose(glm::inverse(particleTrans[i]));
			glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTransArray[i]));
			glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(particleTrans[i]));

			//textureArray[i] = particleArray[i].textures[particleArray[i].materials[0].diffuse_texname];
			glBindTexture(GL_TEXTURE_2D, textureArray[i]);
			glDrawElements(GL_TRIANGLES, particleArray[i].numFaces, GL_UNSIGNED_INT, (void*)0);
		}
#pragma endregion


		//unbindtexture after rendering
		glBindTexture(GL_TEXTURE_2D, 0);

		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		rotFactor += deltaTime * 900;
		if (rotFactor > 360.0f) {
			rotFactor -= 360.0f;
		}
		prevTime = currentTime;

		//--- stop drawing here ---
#pragma endregion

		glfwSwapBuffers(window);
		//listen for glfw input events
		glfwPollEvents();
	}
	return 0;
}