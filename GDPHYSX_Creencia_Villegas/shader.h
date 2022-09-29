#pragma once

void checkShaderCompileStatus(int success, GLuint shader) {
	char infoLog[512];
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cerr << "Shader compilation failed \n" << infoLog << std::endl;
	}
	else {
		std::cout << "Shader compile is success" << std::endl;
	}
}

GLuint LoadShaders(std::string vertexShaderPath, std::string fragmentShaderPath) {

	//load fromfile
	std::ifstream inStream;
	inStream.open(vertexShaderPath);

	if (!inStream.is_open()) {
		std::cerr << "Error loading shader " << vertexShaderPath << std::endl;
		return 0;
	}
	std::stringstream strStream;
	strStream << inStream.rdbuf();
	std::string sourceVertexString = strStream.str();
	const char* vertexShaderSource = sourceVertexString.c_str();
	inStream.close();

	inStream.open(fragmentShaderPath);
	if (!inStream.is_open()) {
		std::cerr << "Error loading shader " << fragmentShaderPath << std::endl;
		return 0;
	}
	strStream.clear();
	strStream.str("");
	strStream << inStream.rdbuf();
	std::string sourceFragmentString = strStream.str();
	const char* fragmentShaderSource = sourceFragmentString.c_str();
	inStream.close();


	// define our shaders
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//fragment shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	int success;

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	checkShaderCompileStatus(success, vertexShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	checkShaderCompileStatus(success, fragmentShader);

	// create a shader program
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	// attach and link our program to it
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//cleanup
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

