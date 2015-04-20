#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <string>
#include <iostream>


class Model
{
public:
	Model();

	Model(float x, float y, float z);
	
	~Model();

	void Render();

	GLuint vbuffer;
	GLuint colourLoc;
	std::vector< glm::vec3 > vertices;
	float X, Y, Z, rotation;
	
private:
	
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	std::vector< glm::vec4 > colours;
	bool loadOBJ(
		const char* path,
		std::vector < glm::vec3 > & out_vertices,
		std::vector < glm::vec2 > & out_uvs,
		std::vector < glm::vec3 > & out_normals,
		std::vector<glm::vec4> & out_colours);
	
};

