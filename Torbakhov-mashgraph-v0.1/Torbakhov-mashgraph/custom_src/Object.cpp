#include "../custom_include/Object.h"

void Object::drawObject(Camera* camera, ShaderProgram* overrideShader) const
{
	ShaderProgram* usedShader = overrideShader;
	if (!usedShader)
		usedShader = shader;

	usedShader->StartUseShader();
	GL_CHECK_ERRORS;
	int modelMatrixID = usedShader->uniformLocation("modelMatrix");
	int normalMatrixID = usedShader->uniformLocation("normalMatrix");

	glm::mat4 model = glm::translate(glm::mat4(1.0), position) * glm::toMat4(rotation) * glm::scale(glm::mat4(1.0), glm::vec3(scale, scale, scale));
	glm::mat3 normal = glm::mat3(glm::transpose(glm::inverse(model)));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, &normal[0][0]);
	GL_CHECK_ERRORS;
	if (camera) {
		int cameraPosWSID = usedShader->uniformLocation("cameraPosWS");
		int viewMatrixID = usedShader->uniformLocation("viewMatrix");
		int projectionMatrixID = usedShader->uniformLocation("projectionMatrix");

		glm::mat4 view = camera->viewMatrix();
		glm::mat4 projection = camera->projectionMatrix();
		glUniform3f(cameraPosWSID, camera->pos.x, camera->pos.y, camera->pos.z);
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &projection[0][0]);
	}
	GL_CHECK_ERRORS;
	
	GL_CHECK_ERRORS;
	mesh->render();
	GL_CHECK_ERRORS;
	usedShader->StopUseShader();
}