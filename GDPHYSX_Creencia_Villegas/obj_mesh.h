#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct VertexData {
	glm::vec3 position;
	glm::vec2 uv;
};

// data class for obj data
struct ObjData {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	GLulong numFaces;
	GLuint vaoId;

	std::string baseDir;
	std::vector<tinyobj::material_t> materials;
	std::map<std::string, GLuint> textures;
};

std::string GetBaseDir(const std::string& filepath) {
	if (filepath.find_last_of("/\\") != std::string::npos)
		return filepath.substr(0, filepath.find_last_of("/\\"));
	return "";
}

bool FileExists(const std::string& absFilename) {
	struct stat buffer;
	return (stat(absFilename.c_str(), &buffer) == 0);
}

void LoadTextureData(ObjData* objData) {
	int width, height;
	std::string baseDir = objData->baseDir;
	for (size_t m = 0; m < objData->materials.size(); m++) {
		tinyobj::material_t* mp = &objData->materials[m];
		if (mp->diffuse_texname.length() > 0) {
			if (objData->textures.find(mp->diffuse_texname) == objData->textures.end()) {
				GLuint textureId;
				int comp;

				std::string textureFileName = baseDir + "/" + mp->diffuse_texname;
				if (!FileExists(textureFileName)) {
					std::cerr << "Unable to find file: " << textureFileName << std::endl;
					exit(1);
				}
				unsigned char* image = stbi_load(
					textureFileName.c_str(),
					&width,
					&height,
					&comp,
					STBI_default
				);

				if (!image) {
					std::cerr << "Unable to load texture: " << textureFileName << std::endl;
					exit(1);
				}

				std::cout << "Loaded texture: " << textureFileName << std::endl;

				glGenTextures(1, &textureId);
				glBindTexture(GL_TEXTURE_2D, textureId);

				GLenum format = GL_RGBA;
				if (comp == 3) {
					format = GL_RGB;
				}
				else if (comp == 4) {
					format = GL_RGBA;
				}

				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					format,
					width,
					height,
					0,
					format,
					GL_UNSIGNED_BYTE,
					image);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(image);
				objData->textures.insert(std::make_pair(mp->diffuse_texname, textureId));
			}
		}
	}
}


// function used mainly for loading obj models
void LoadObjFile(ObjData* objData, std::string filename) {
	std::string warn;
	std::string err;

	std::string basepath = "Assets/";
	std::string inputfile = basepath + filename;

	std::string baseDir = GetBaseDir(inputfile);
	objData->baseDir = baseDir;

	bool isSuccess = tinyobj::LoadObj(&objData->attrib,
		&objData->shapes,
		&objData->materials,
		&warn,
		&err,
		inputfile.c_str(),
		baseDir.c_str()
	);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	std::cout << "Loaded " << filename << std::endl;
	std::cout << "with # of shapes " << objData->shapes.size() << std::endl;
	std::cout << "with # of vertices " << objData->attrib.vertices.size() << std::endl;
	std::cout << "-------------------" << std::endl;
}



void LoadObjToMemory(ObjData* objData, GLfloat scaleFactor, GLfloat tOffset[]) {

	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<VertexData> vertexList;

	/*for (int i = 0; i < objData->attrib.vertices.size() / 3; i++) {
		VertexData vertexData;
		vertexData.position = {
			objData->attrib.vertices[i * 3] * scaleFactor + tOffset[0],// x
			objData->attrib.vertices[i * 3 + 1] * scaleFactor + tOffset[1],// y
			objData->attrib.vertices[i * 3 + 2] * scaleFactor + tOffset[2]// z
		};


	}*/

	for (int i = 0; i < objData->shapes.size(); i++) {
		tinyobj::shape_t shape = objData->shapes[i];
		for (int j = 0; j < shape.mesh.indices.size(); j++) {
			tinyobj::index_t idx = shape.mesh.indices[j];

			VertexData vertexData;

			vertexData.position = {
				objData->attrib.vertices[size_t(idx.vertex_index) * 3] * scaleFactor + tOffset[0],// x
				objData->attrib.vertices[size_t(idx.vertex_index) * 3 + 1] * scaleFactor + tOffset[1],// y
				objData->attrib.vertices[size_t(idx.vertex_index) * 3 + 2] * scaleFactor + tOffset[2]// z
			};

			if (idx.texcoord_index >= 0) {
				vertexData.uv = {
					objData->attrib.texcoords[size_t(idx.texcoord_index) * 2 + 0],
					objData->attrib.texcoords[size_t(idx.texcoord_index) * 2 + 1]
				};
			}

			vertexList.push_back(vertexData);
			indices.push_back(indices.size());
		}
	}

	objData->numFaces = indices.size();

	// generate VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	std::cout << "Vertex Array ID: " << VAO << std::endl;



	// generate VBO (vertices)
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	std::cout << "Vertex Buffer ID: " << VBO << std::endl;
	glBufferData(GL_ARRAY_BUFFER, vertexList.size() * sizeof(VertexData), &vertexList[0], GL_STATIC_DRAW);

	//set attributes for vertice buffer
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexData),  //stride, we can give 0 to let opengl handle it, we can also give 3 * sizeof(GLfloat).
		(void*)0
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexData),
		(void*)offsetof(VertexData, uv)
	);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(GLuint),
		&indices[0],
		GL_STATIC_DRAW
	);

	LoadTextureData(objData);

	objData->vaoId = VAO;
}