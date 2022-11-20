#pragma once
#include "PhysicsWorld.h"

#include "AnchoredSpring.h"
#include "BungeeSpring.h"
#include "Rod.h"

#include <algorithm>
#include "main.h"

// CHANGE NORMALTRANSARRAY AND PARTICLETRANS IN PHYSICSWORLD TO MYPARTICLE

// TRY CHANGING MYVECTOR USES TO GLM::VEC3

// CHANGE PARTICLESPRING CLASS NAME TO PARTICLELINKS

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

	// var for rdeltaTime
	float currentTime = glfwGetTime();
	float prevTime = 0.0f;
	float deltaTime = 0.0f;


#pragma region Particle Physics Declarations

	PhysicsWorld pWorld;

	MyVector initialPos = MyVector(0, 0, 0);
	int currType = 1;
	bool isCollide = false;
	float boxRadius = 4;
	float boxMass = 25;
	float cooldown = 1;
	float force = 0;
	float stopper = 0;

#pragma endregion

#pragma region MASS AGGREGATE BOX

	/// MASS AGGREGATE BOX
	for (int i = 0; i < 8; i++)
	{
		pWorld.addParticle(0, initialPos);
	}
	cout << "particles size = " << pWorld.particles.size() << endl;

	pWorld.particles[0]->position = MyVector(25, 5, 0);
	pWorld.particles[1]->position = MyVector(25, 5, 10);
	pWorld.particles[2]->position = MyVector(25, -5, 0);
	pWorld.particles[3]->position = MyVector(25, -5, 10);

	pWorld.particles[4]->position = MyVector(35, 5, 0);
	pWorld.particles[5]->position = MyVector(35, 5, 10);
	pWorld.particles[6]->position = MyVector(35, -5, 0);
	pWorld.particles[7]->position = MyVector(35, -5, 10);

	for (int i = 0; i < pWorld.particles.size(); i++)
	{
		pWorld.particleTrans[i] = glm::mat4(1.0f);
		pWorld.particleTrans[i] = glm::scale(pWorld.particleTrans[i], glm::vec3(0.5f, 0.5f, 0.5f));
		pWorld.particleTrans[i] = glm::translate(pWorld.particleTrans[i], glm::vec3(pWorld.particles[i]->getVec3Pos()));
		cout << "particle trans pos = " << pWorld.particleTrans[i][3].x << ", " << pWorld.particleTrans[i][3].y << ", " << pWorld.particleTrans[i][3].z << endl;
	}

	/// NOTES

	//pWorld.particles[0] -- FRONT TOP LEFT
	//pWorld.particles[1] -- FRONT TOP RIGHT
	//pWorld.particles[2] -- FRONT BOT LEFT
	//pWorld.particles[3] -- FRONT BOT RIGHT

	//pWorld.particles[4] -- BACK TOP LEFT
	//pWorld.particles[5] -- BACK TOP RIGHT
	//pWorld.particles[6] -- BACK BOT LEFT
	//pWorld.particles[7] -- BACK BOT RIGHT

	//					  PAIRS
	//------------------------------------------------
	//		  FRONT		  BACK		 CONNECT
	//		0 ----- 1	4 ----- 5	0 ----- 4
	//		|       |	|       |	1 ----- 5
	//		|       |	|       |	2 ----- 6
	//		2 ----- 3	6 ----- 7	3 ----- 7
	
	// FRONT SQUARE
	Rod* Pair1 = new Rod();
	Pair1->particles[0] = pWorld.particles[0];
	Pair1->particles[1] = pWorld.particles[1];
	Pair1->length = glm::distance(pWorld.particles[0]->getVec3Pos(), pWorld.particles[1]->getVec3Pos());
	pWorld.links.push_back(Pair1);
	cout << "Pair1 length = " << Pair1->length << endl;

	Rod* Pair2 = new Rod();
	Pair2->particles[0] = pWorld.particles[2];
	Pair2->particles[1] = pWorld.particles[3];
	Pair2->length = glm::distance(pWorld.particles[2]->getVec3Pos(), pWorld.particles[3]->getVec3Pos());;
	pWorld.links.push_back(Pair2);
	cout << "Pair2 length = " << Pair2->length << endl;

	Rod* Pair3 = new Rod();
	Pair3->particles[0] = pWorld.particles[0];
	Pair3->particles[1] = pWorld.particles[2];
	Pair3->length = glm::distance(pWorld.particles[0]->getVec3Pos(), pWorld.particles[2]->getVec3Pos());;
	pWorld.links.push_back(Pair3);
	cout << "Pair3 length = " << Pair3->length << endl;

	Rod* Pair4 = new Rod();
	Pair4->particles[0] = pWorld.particles[1];
	Pair4->particles[1] = pWorld.particles[3];
	Pair4->length = glm::distance(pWorld.particles[1]->getVec3Pos(), pWorld.particles[3]->getVec3Pos());;
	pWorld.links.push_back(Pair4);
	cout << "Pair4 length = " << Pair4->length << endl;

	// BACK SQUARE
	Rod* Pair5 = new Rod();
	Pair5->particles[0] = pWorld.particles[4];
	Pair5->particles[1] = pWorld.particles[5];
	Pair5->length = glm::distance(pWorld.particles[4]->getVec3Pos(), pWorld.particles[5]->getVec3Pos());;
	pWorld.links.push_back(Pair5);
	cout << "Pair5 length = " << Pair5->length << endl;

	Rod* Pair6 = new Rod();
	Pair6->particles[0] = pWorld.particles[6];
	Pair6->particles[1] = pWorld.particles[7];
	Pair6->length = glm::distance(pWorld.particles[6]->getVec3Pos(), pWorld.particles[7]->getVec3Pos());;
	pWorld.links.push_back(Pair6);
	cout << "Pair6 length = " << Pair6->length << endl;

	Rod* Pair7 = new Rod();
	Pair7->particles[0] = pWorld.particles[4];
	Pair7->particles[1] = pWorld.particles[6];
	Pair7->length = glm::distance(pWorld.particles[4]->getVec3Pos(), pWorld.particles[6]->getVec3Pos());;
	pWorld.links.push_back(Pair7);
	cout << "Pair7 length = " << Pair7->length << endl;

	Rod* Pair8 = new Rod();
	Pair8->particles[0] = pWorld.particles[5];
	Pair8->particles[1] = pWorld.particles[7];
	Pair8->length = glm::distance(pWorld.particles[5]->getVec3Pos(), pWorld.particles[7]->getVec3Pos());;
	pWorld.links.push_back(Pair8);
	cout << "Pair8 length = " << Pair8->length << endl;

	// CONNECTING FRONT AND BACK SQUARES
	Rod* Pair9 = new Rod();
	Pair9->particles[0] = pWorld.particles[0];
	Pair9->particles[1] = pWorld.particles[4];
	Pair9->length = glm::distance(pWorld.particles[0]->getVec3Pos(), pWorld.particles[4]->getVec3Pos());;
	pWorld.links.push_back(Pair9);
	cout << "Pair9 length = " << Pair9->length << endl;

	Rod* Pair10 = new Rod();
	Pair10->particles[0] = pWorld.particles[1];
	Pair10->particles[1] = pWorld.particles[5];
	Pair10->length = glm::distance(pWorld.particles[1]->getVec3Pos(), pWorld.particles[5]->getVec3Pos());;
	pWorld.links.push_back(Pair10);
	cout << "Pair10 length = " << Pair10->length << endl;

	Rod* Pair11 = new Rod();
	Pair11->particles[0] = pWorld.particles[2];
	Pair11->particles[1] = pWorld.particles[6];
	Pair11->length = glm::distance(pWorld.particles[2]->getVec3Pos(), pWorld.particles[6]->getVec3Pos());;
	pWorld.links.push_back(Pair11);
	cout << "Pair11 length = " << Pair11->length << endl;

	Rod* Pair12 = new Rod();
	Pair12->particles[0] = pWorld.particles[3];
	Pair12->particles[1] = pWorld.particles[7];
	Pair12->length = glm::distance(pWorld.particles[3]->getVec3Pos(), pWorld.particles[7]->getVec3Pos());;
	pWorld.links.push_back(Pair12);
	cout << "Pair12 length = " << Pair12->length << endl;

	// FRONT FACE
	Rod* Pair13 = new Rod();
	Pair13->particles[0] = pWorld.particles[0];
	Pair13->particles[1] = pWorld.particles[3];
	Pair13->length = glm::distance(pWorld.particles[0]->getVec3Pos(), pWorld.particles[3]->getVec3Pos());;
	pWorld.links.push_back(Pair13);
	cout << "Pair13 length = " << Pair13->length << endl;

	Rod* Pair14 = new Rod();
	Pair14->particles[0] = pWorld.particles[1];
	Pair14->particles[1] = pWorld.particles[2];
	Pair14->length = glm::distance(pWorld.particles[1]->getVec3Pos(), pWorld.particles[2]->getVec3Pos());;
	pWorld.links.push_back(Pair14);
	cout << "Pair14 length = " << Pair14->length << endl;

	// BACK FACE
	Rod* Pair15 = new Rod();
	Pair15->particles[0] = pWorld.particles[4];
	Pair15->particles[1] = pWorld.particles[7];
	Pair15->length = glm::distance(pWorld.particles[4]->getVec3Pos(), pWorld.particles[7]->getVec3Pos());;
	pWorld.links.push_back(Pair15);
	cout << "Pair15 length = " << Pair15->length << endl;

	Rod* Pair16 = new Rod();
	Pair16->particles[0] = pWorld.particles[5];
	Pair16->particles[1] = pWorld.particles[6];
	Pair16->length = glm::distance(pWorld.particles[5]->getVec3Pos(), pWorld.particles[6]->getVec3Pos());;
	pWorld.links.push_back(Pair16);
	cout << "Pair16 length = " << Pair16->length << endl;

	// TOP FACE
	Rod* Pair17 = new Rod();
	Pair17->particles[0] = pWorld.particles[0];
	Pair17->particles[1] = pWorld.particles[5];
	Pair17->length = glm::distance(pWorld.particles[0]->getVec3Pos(), pWorld.particles[5]->getVec3Pos());;
	pWorld.links.push_back(Pair17);
	cout << "Pair17 length = " << Pair17->length << endl;

	Rod* Pair18 = new Rod();
	Pair18->particles[0] = pWorld.particles[1];
	Pair18->particles[1] = pWorld.particles[4];
	Pair18->length = glm::distance(pWorld.particles[1]->getVec3Pos(), pWorld.particles[4]->getVec3Pos());;
	pWorld.links.push_back(Pair18);
	cout << "Pair18 length = " << Pair18->length << endl;

	// BOTTOM FACE
	Rod* Pair19 = new Rod();
	Pair19->particles[0] = pWorld.particles[2];
	Pair19->particles[1] = pWorld.particles[7];
	Pair19->length = glm::distance(pWorld.particles[2]->getVec3Pos(), pWorld.particles[7]->getVec3Pos());;
	pWorld.links.push_back(Pair19);
	cout << "Pair19 length = " << Pair19->length << endl;

	Rod* Pair20 = new Rod();
	Pair20->particles[0] = pWorld.particles[3];
	Pair20->particles[1] = pWorld.particles[6];
	Pair20->length = glm::distance(pWorld.particles[3]->getVec3Pos(), pWorld.particles[6]->getVec3Pos());;
	pWorld.links.push_back(Pair20);
	cout << "Pair20 length = " << Pair20->length << endl;

	// LEFT FACE
	Rod* Pair21 = new Rod();
	Pair21->particles[0] = pWorld.particles[0];
	Pair21->particles[1] = pWorld.particles[6];
	Pair21->length = glm::distance(pWorld.particles[0]->getVec3Pos(), pWorld.particles[6]->getVec3Pos());;
	pWorld.links.push_back(Pair21);
	cout << "Pair21 length = " << Pair21->length << endl;

	Rod* Pair22 = new Rod();
	Pair22->particles[0] = pWorld.particles[2];
	Pair22->particles[1] = pWorld.particles[4];
	Pair22->length = glm::distance(pWorld.particles[2]->getVec3Pos(), pWorld.particles[4]->getVec3Pos());;
	pWorld.links.push_back(Pair22);
	cout << "Pair22 length = " << Pair22->length << endl;

	// RIGHT FACE
	Rod* Pair23 = new Rod();
	Pair23->particles[0] = pWorld.particles[1];
	Pair23->particles[1] = pWorld.particles[7];
	Pair23->length = glm::distance(pWorld.particles[1]->getVec3Pos(), pWorld.particles[7]->getVec3Pos());;
	pWorld.links.push_back(Pair23);
	cout << "Pair23 length = " << Pair23->length << endl;

	Rod* Pair24 = new Rod();
	Pair24->particles[0] = pWorld.particles[3];
	Pair24->particles[1] = pWorld.particles[5];
	Pair24->length = glm::distance(pWorld.particles[3]->getVec3Pos(), pWorld.particles[5]->getVec3Pos());;
	pWorld.links.push_back(Pair24);
	cout << "Pair24 length = " << Pair24->length << endl;



	cout << "links size = " << pWorld.links.size() << endl;

#pragma endregion

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
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			cameraPos += cameraSpeed * cameraFront;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			cameraPos -= cameraSpeed * cameraFront;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
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

		// Change bullet type
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) currType = 1;
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) currType = 2;
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) currType = 3;
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) currType = 4;
		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) currType = 5;

		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		prevTime = currentTime;

		// Fire particle key
		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

		// Firing cooldown
		cooldown += cooldown * deltaTime;
		if (cooldown >= 1.5)
		{
			if (state == GLFW_PRESS) {
				pWorld.addParticle(currType, initialPos);
				cooldown = 1;
			}
		}

		if (!pWorld.particles.empty())
		{
			// Collision detection
			for (int i = 8; i < pWorld.particles.size(); i++)
			{
				if (pWorld.getDistance(pWorld.particleTrans[i][3].x, pWorld.particleTrans[i][3].y, pWorld.particleTrans[i][3].z, boxTrans[3].x, boxTrans[3].y, boxTrans[3].z) <= pWorld.particles[i]->radius + boxRadius) {
					force = (pWorld.particles[i]->mass * pWorld.particles[i]->velocity.x) / boxMass;
					stopper = force;

					if (pWorld.particles[i]->type != 4)
					{
						pWorld.particles[i]->isDestroyed = true;
					}
					cout << endl << "COLLIDED" << endl;
				}
				else if (pWorld.particles[i]->isDestroyed)
				{
					if (pWorld.particles[i]->type == 5)
					{
						for (int j = 0; j < pWorld.particles[i]->count; j++)
						{
							pWorld.addParticle(6, pWorld.particles[i]->position);
						}
						for (int l = 0; l < pWorld.particles[i]->count * 3; l++)
						{
							pWorld.addParticle(7, pWorld.particles[i]->position);
						}
					}
					else if (pWorld.particles[i]->type == 6)
					{
						for (int k = 0; k < pWorld.particles[i]->count; k++)
						{
							pWorld.addParticle(7, pWorld.particles[i]->position);
						}
					}
				}
			}

			// Update physics world
			pWorld.update(deltaTime);

			// Drawing Particles
			for (int i = 0; i < pWorld.particles.size(); i++) {
				pWorld.normalTransArray[i] = glm::transpose(glm::inverse(pWorld.particleTrans[i]));
				glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(pWorld.normalTransArray[i]));
				glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(pWorld.particleTrans[i]));

				glActiveTexture(GL_TEXTURE0);
				GLuint particleTexture = particle.textures[particle.materials[pWorld.particles[i]->material].diffuse_texname];
				glBindTexture(GL_TEXTURE_2D, particleTexture);
				glDrawElements(GL_TRIANGLES, particle.numFaces, GL_UNSIGNED_INT, (void*)0);
			}


		}
		
		// Move box when hit
		boxTrans = glm::translate(boxTrans, glm::vec3(force, 0.0f, 0.0f) * deltaTime);
		if (force >= 0) force -= stopper * deltaTime;
		else force = 0;

		//system("pause");

#pragma endregion

		//unbindtexture after rendering
		glBindTexture(GL_TEXTURE_2D, 0);

		

		//--- stop drawing here ---
#pragma endregion

		glfwSwapBuffers(window);
		//listen for glfw input events
		glfwPollEvents();
	}
	return 0;
}