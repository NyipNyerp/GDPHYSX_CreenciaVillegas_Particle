#pragma once
#include "MyParticle.h"
#include "ForceGenerator.h"
#include "ForceRegistry.h"
#include "GravityForceGenerator.h"

#include "ParticleSpring.h"
#include "AnchoredSpring.h"
#include "BungeeSpring.h"
#include "CableSpring.h"

#include "main.h"

// Determines Particle data for new Particles
MyParticle particleType(int type)
{
	float randX = rand() % 60 + -30;
	float randY = rand() % 30 + -5;
	float randZ = rand() % 30 + -15;
	float randAge = rand() % 3 + 1;

	MyParticle newData;
	switch (type) {
	case 0:
		newData.velo = MyVector();
		newData.accel = MyVector();
		newData.mass = 5.0f;
		newData.damp = 1.0f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 0;
		newData.count = 1;
		newData.lifeSpan = 10000;
		break;
	case 1:
		newData.velo = MyVector(35.0f, 0.0f, 0.0f);
		newData.accel = MyVector(0.0f, -1.0f, 0.0f);
		newData.mass = 2.0f;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 1;
		newData.count = 1;
		newData.lifeSpan = 5;
		break;
	case 2:
		newData.velo = MyVector(40.0f, 30.0f, 0.0f);
		newData.accel = MyVector(0.0f, -20.0f, 0.0f);
		newData.mass = 200.0f;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 2;
		newData.count = 1;
		newData.lifeSpan = 5;
		break;
	case 3:
		newData.velo = MyVector(10.0f, 0.0f, 0.0f);
		newData.accel = MyVector(0.0f, 0.6f, 0.0f);
		newData.mass = 1.0f;
		newData.damp = 0.9f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 3;
		newData.count = 1;
		newData.lifeSpan = 5;
		break;
	case 4:
		newData.velo = MyVector(100.0f, 0.0f, 0.0f);
		newData.accel = MyVector(0.0f, 0.0f, 0.0f);
		newData.mass = 0.1f;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 4;
		newData.count = 1;
		newData.lifeSpan = 5;
		break;
	case 5: // FIREWORKS TYPE A
		newData.velo = MyVector(randX, 50.0f, 0.0f);
		newData.accel = MyVector(0.0f, 0.0f, 0.0f);
		newData.mass = 1.0;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 5;
		newData.count = 5;
		newData.lifeSpan = 3;
		newData.material = 3;
		break;
	case 6: // FIREWORKS TYPE B
		newData.velo = MyVector(randX, randY, randZ);
		newData.accel = MyVector(0.0f, -5.0f, 0.0f);
		newData.mass = 1.0;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 6;
		newData.count = 10;
		newData.lifeSpan = randAge;
		newData.material = 2;
		break;
	case 7: // FIREWORKS TYPE C
		newData.velo = MyVector(randX, randY, randZ);
		newData.accel = MyVector(0.0f, -10.0f, 0.0f);
		newData.mass = 1.0;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 6;
		newData.count = 0;
		newData.lifeSpan = randAge;
		newData.material = 1;
		break;
	case 8: // PARTICLE SPRING
		newData.velo = MyVector(50, -10, 0);
		newData.accel = MyVector(0.0f, -10.0f, 0.0f);
		newData.mass = 10.0;
		newData.damp = 1.0f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 8;
		newData.count = 0;
		newData.lifeSpan = 10;
		newData.material = 1;
		break;
	case 9: // ANCHORED SPRING
		newData.velo = MyVector(50, 0, 0);
		newData.accel = MyVector(0.0f, 0.0f, 0.0f);
		newData.mass = 10.0;
		newData.damp = 1.0f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 9;
		newData.count = 0;
		newData.lifeSpan = 10;
		newData.material = 1;
		break;
	case 10: // CABLE SPRING
		newData.velo = MyVector(50, -10, 0);
		newData.accel = MyVector(0.0f, -10.0f, 0.0f);
		newData.mass = 10.0;
		newData.damp = 1.0f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 10;
		newData.count = 0;
		newData.lifeSpan = 10;
		newData.material = 1;
		break;
	}

	return newData;
}

// Creates new Particles
void createParticle(std::vector<MyParticle>* particleList, std::vector<glm::mat4>* MyParticleTrans, std::vector<glm::mat4>* normalTransArray, GLuint modelTransLoc, GLuint normTransLoc, int bType, glm::vec3 currParticlePos, ForceRegistry* Registry, GravityForceGenerator Gravity)
{
	MyParticle newParticle = particleType(bType);;
	particleList->push_back(newParticle);


	glm::mat4 trans = glm::mat4(1.0f); // identity
	if (bType == 6) trans = glm::translate(trans, currParticlePos);
	else if (bType == 7) trans = glm::translate(trans, currParticlePos);
	if (bType == 0) trans = glm::scale(trans, glm::vec3(0.0f, 0.0f, 0.0f));
	else trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	MyParticleTrans->push_back(trans);

	glm::mat4 normalTrans;
	normalTransArray->push_back(normalTrans);


	// Assign GravityForceGenerator to newly created particles in the ForceRegistry
	Registry->Add(&newParticle, &Gravity);
	/*
	std::cout << std::endl
		<< "MyParticleTrans = " << MyParticleTrans->size() << std::endl
		<< "normalTransArray = " << normalTransArray->size() << std::endl
	*/
}

// Deletes an existing Particles
void deleteParticle(std::vector<glm::mat4>* MyParticleTrans, std::vector<glm::mat4>* normalTransArray, std::vector<MyParticle>* MyParticleDatas, int index)
{
	MyParticleTrans->erase(MyParticleTrans->begin() + index);
	normalTransArray->erase(normalTransArray->begin() + index);
	MyParticleDatas->erase(MyParticleDatas->begin() + index);
}

// Delete all Particles
void clearParticle(std::vector<glm::mat4>* MyParticleTrans, std::vector<glm::mat4>* normalTransArray, std::vector<MyParticle>* MyParticleDatas)
{
	MyParticleTrans->clear();
	normalTransArray->clear();
	MyParticleDatas->clear();
}

// Gets the distance between 2 objects for collision checking
float getDistance(float xPos1, float yPos1, float zPos1, float xPos2, float yPos2, float zPos2) {
	//std::cout	<< xPos1 << ", " << yPos1 << ", " << zPos1 << std::endl << xPos2 << ", " << yPos2 << ", " << zPos2 << std::endl << std::endl;
	return sqrt(pow(xPos2 - xPos1, 2) + pow(yPos2 - yPos1, 2) + pow(zPos2 - zPos1, 2));
}

#pragma region CAMERA 
//YouTube. (2019). OpenGL - camera movement. YouTube. https://www.youtube.com/watch?v=AWM4CUfffos.
//camera
glm::vec3 cameraPos = glm::vec3(10.0f, 10.0f, 30.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

//mouse state
bool firstMouse = true;
float lastX = 1024 / 2.0;
float lastY = 768 / 2.0;


// Mouse camera controls
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

#pragma endregion


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int main() {
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

	std::vector<glm::mat4> particleTrans;
	std::vector<glm::mat4> normalTransArray;
	std::vector<MyParticle> particleList;

	ForceRegistry registry;
	GravityForceGenerator gravity = GravityForceGenerator(MyVector(0.0f, 0.0f, 0.0f));

	AnchoredSpring aSpring = AnchoredSpring(MyVector(20,0,0), 5, 0.5);

	int currType = 1;
	bool isCollide = false;
	float boxRadius = 5.5;
	float boxMass = 25;
	float cooldown = 1;
	float force = 0;
	float stopper = 0;

#pragma endregion

#pragma region Mesh Loading

	ObjData particle;
	LoadObjFile(&particle, "earth/Earth.obj");
	GLfloat particleOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&particle,
		1.0f,
		particleOffsets
	);

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
	SkyBoxData skybox = LoadSkybox("Assets/skybox", faces);

#pragma endregion

#pragma region Shader Loading
	GLuint skyboxShaderProgram = LoadShaders("Shaders/skybox_vertex.shader", "Shaders/skybox_fragment.shader");
	GLuint shaderProgram = LoadShaders("Shaders/vertex.shader", "Shaders/fragment.shader");
	glUseProgram(shaderProgram);

	GLuint colorLoc = glGetUniformLocation(shaderProgram, "u_color");
	glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);


	// initialize MVP
	GLuint modelTransformLoc = glGetUniformLocation(shaderProgram, "u_model");
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "u_view");
	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "u_projection");

	GLuint normalTransformLoc = glGetUniformLocation(shaderProgram, "u_normal");
	GLuint cameraPosLoc = glGetUniformLocation(shaderProgram, "u_camera_pos");
	GLuint ambientColorLoc = glGetUniformLocation(shaderProgram, "u_ambient_color");
	glUniform3f(ambientColorLoc, 0.1f, 0.1f, 0.1f);


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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

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

		// Keyboard camera controls
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

		DrawSkybox(skybox, skyboxShaderProgram, view, projection);
		glUseProgram(shaderProgram);

#pragma region Box Object

		glBindVertexArray(box.vaoId);
		glUseProgram(shaderProgram);

		glm::mat4 boxNormalTrans = glm::transpose(glm::inverse(boxTrans));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(boxNormalTrans));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(boxTrans));

		glActiveTexture(GL_TEXTURE0);
		GLuint boxTexture = box.textures[box.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, boxTexture);
		glDrawElements(GL_TRIANGLES, box.numFaces, GL_UNSIGNED_INT, (void*)0);

#pragma endregion

#pragma region Particle Objects

		glBindVertexArray(particle.vaoId);
		glUseProgram(shaderProgram);


		registry.UpdateForces(deltaTime);

		for (std::vector<MyParticle>::iterator i = particleList.begin();
			i != particleList.end(); i++)
		{
			//(*i)->acceleration = gravity;
			(i)->Update(deltaTime);
		}

		// Change bullet type
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) currType = 1;
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) currType = 2;
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) currType = 3;
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) currType = 4;
		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) currType = 5;
		//if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) currType = 6;
		//if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) currType = 7;
		//if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) currType = 8;

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) particleTrans[0] = glm::rotate(particleTrans[0], glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix

		// Fire particle key
		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

		// Firing cooldown
		cooldown += cooldown * deltaTime;
		if (cooldown >= 1.5)
		{
			if (state == GLFW_PRESS) {
				createParticle(&particleList, &particleTrans, &normalTransArray, modelTransformLoc, normalTransformLoc, currType, glm::vec3(0.0f, 0.0f, 0.0f), &registry, gravity);
				cooldown = 1;
			}
		}

		if (!particleList.empty())
		{
			// Collision checking and resolution
			for (int i = 0; i < particleList.size(); i++)
			{
				particleList[i].lifeSpan -= 1 * deltaTime;
				if (getDistance(particleTrans[i][3].x, particleTrans[i][3].y, particleTrans[i][3].z, boxTrans[3].x, boxTrans[3].y, boxTrans[3].z) <= particleList[i].radius + boxRadius) {
					force = (particleList[i].mass * particleList[i].velo.x) / boxMass;
					stopper = force;
					if (particleList[i].type != 4)
						deleteParticle(&particleTrans, &normalTransArray, &particleList, i);
				}
				else if (particleList[i].lifeSpan <= 0)
				{
					if (particleList[i].type == 5)
					{
						for (int j = 0; j < particleList[i].count; j++)
							createParticle(&particleList, &particleTrans, &normalTransArray, modelTransformLoc, normalTransformLoc, 6, particleTrans[i][3], &registry, gravity);
						for (int l = 0; l < particleList[i].count * 3; l++)
							createParticle(&particleList, &particleTrans, &normalTransArray, modelTransformLoc, normalTransformLoc, 7, particleTrans[i][3], &registry, gravity);
					}
					else if (particleList[i].type == 6)
					{
						for (int k = 0; k < particleList[i].count; k++)
							createParticle(&particleList, &particleTrans, &normalTransArray, modelTransformLoc, normalTransformLoc, 7, particleTrans[i][3], &registry, gravity);
					}
					deleteParticle(&particleTrans, &normalTransArray, &particleList, i);
				}
			}

			// Move box
			boxTrans = glm::translate(boxTrans, glm::vec3(force, 0.0f, 0.0f) * deltaTime);
			if (force >= 0) force -= stopper * deltaTime;
			else force = 0;


			// Updating and Drawing Particles
			for (int i = 0; i < particleList.size(); i++) {
				glm::vec3 temp = glm::vec3(particleList[i].velo.x, particleList[i].velo.y, particleList[i].velo.z);

				particleList[i].velo += particleList[i].accel * deltaTime;
				particleTrans[i] = glm::translate(particleTrans[i], temp * deltaTime);

				normalTransArray[i] = glm::transpose(glm::inverse(particleTrans[i]));
				glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTransArray[i]));
				glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(particleTrans[i]));

				glActiveTexture(GL_TEXTURE0);
				GLuint particleTexture = particle.textures[particle.materials[particleList[i].material].diffuse_texname];
				glBindTexture(GL_TEXTURE_2D, particleTexture);
				glDrawElements(GL_TRIANGLES, particle.numFaces, GL_UNSIGNED_INT, (void*)0);
			}
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