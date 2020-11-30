#pragma once

#include "common.h"
#include "Mesh.h"

#include "ShaderProgram.h"
#include "Camera.h"

class Object {
public:
	Mesh* mesh;

	ShaderProgram* shader;
	glm::vec3 position;
	glm::quat rotation;
	float scale;

	Object() : mesh(NULL), shader(NULL), scale(1.0f) {}

	void drawObject(Camera* camera, ShaderProgram* overrideShader = NULL) const;
};
