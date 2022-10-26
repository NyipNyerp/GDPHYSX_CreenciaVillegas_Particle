#pragma once
#include "Particle.h"
#include "main.h"



// Gets the distance between 2 objects for collision checking
float getDistance(float xPos1, float yPos1, float zPos1, float xPos2, float yPos2, float zPos2) {
	//std::cout	<< xPos1 << ", " << yPos1 << ", " << zPos1 << std::endl << xPos2 << ", " << yPos2 << ", " << zPos2 << std::endl << std::endl;
	return sqrt(pow(xPos2 - xPos1, 2) + pow(yPos2 - yPos1, 2) + pow(zPos2 - zPos1, 2));
}



#pragma region CAMERA VARIABLES
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

#pragma endregion

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
	std::vector<particle::particleData> particleDatas;
	particle tempParticle;
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
	//GLuint shaderProgram = LoadShaders("Shaders/phong_vertex.shader", "Shaders/phong_fragment.shader");
	//glUseProgram(shaderProgram2);
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

	// Create dummy Particle to avoid crashing when accessing vector arrays in Draw region
	particleDatas.push_back(tempParticle.fireBullet(&particleTrans, &normalTransArray, modelTransformLoc, normalTransformLoc, 0, glm::vec3(0.0f, 0.0f, 0.0f)));

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

		// Collision checking and resolution
		for (int i = 1; i < particleDatas.size(); i++)
		{
			particleDatas[i].lifeSpan -= 1 * deltaTime;
			if (getDistance(particleTrans[i][3].x, particleTrans[i][3].y, particleTrans[i][3].z, boxTrans[3].x, boxTrans[3].y, boxTrans[3].z) <= particleDatas[i].radius + boxRadius) {
				force = (particleDatas[i].mass * particleDatas[i].velo.x) / boxMass;
				stopper = force;
				if (particleDatas[i].type != 4)
					tempParticle.deleteBullet(&particleTrans, &normalTransArray, &particleDatas, i);
			}
			else if (particleDatas[i].lifeSpan <= 0)
			{
				if (particleDatas[i].type == 5)
				{
					for (int j = 0; j < particleDatas[i].count; j++)
						particleDatas.push_back(tempParticle.fireBullet(&particleTrans, &normalTransArray, modelTransformLoc, normalTransformLoc, 6, particleTrans[i][3]));
					for (int l = 0; l < particleDatas[i].count * 3; l++)
						particleDatas.push_back(tempParticle.fireBullet(&particleTrans, &normalTransArray, modelTransformLoc, normalTransformLoc, 7, particleTrans[i][3]));
				}
				else if (particleDatas[i].type == 6)
				{
					for (int k = 0; k < particleDatas[i].count; k++)
						particleDatas.push_back(tempParticle.fireBullet(&particleTrans, &normalTransArray, modelTransformLoc, normalTransformLoc, 7, particleTrans[i][3]));
				}
				tempParticle.deleteBullet(&particleTrans, &normalTransArray, &particleDatas, i);
			}
		}
		boxTrans = glm::translate(boxTrans, glm::vec3(force, 0.0f, 0.0f) * deltaTime);
		if (force >= 0) force -= stopper * deltaTime;
		else force = 0;

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


		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		// Firing cooldown
		cooldown += cooldown * deltaTime;
		if (cooldown >= 1.5)
		{
			if (state == GLFW_PRESS) {
				particleDatas.push_back(tempParticle.fireBullet(&particleTrans, &normalTransArray, modelTransformLoc, normalTransformLoc, currType, glm::vec3(0.0f, 0.0f, 0.0f)));
				cooldown = 1;
			}
		}

		// Updating and Drawing Particles
		for (int i = 1; i < particleDatas.size(); i++) {
			particleDatas[i].velo += particleDatas[i].accel * deltaTime;
			particleTrans[i] = glm::translate(particleTrans[i], particleDatas[i].velo * deltaTime);

			normalTransArray[i] = glm::transpose(glm::inverse(particleTrans[i]));
			glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTransArray[i]));
			glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(particleTrans[i]));

			glActiveTexture(GL_TEXTURE0);
			GLuint particleTexture = particle.textures[particle.materials[particleDatas[i].material].diffuse_texname];
			glBindTexture(GL_TEXTURE_2D, particleTexture);
			glDrawElements(GL_TRIANGLES, particle.numFaces, GL_UNSIGNED_INT, (void*)0);
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