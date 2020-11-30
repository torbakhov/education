#version 330 core

layout(location = 0) in vec3 positionMS;
layout(location = 1) in vec3 normalMS;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

void main(void)
{
	vec4 positionWS = modelMatrix * vec4(positionMS, 1.0f);
	
    gl_Position = projectionMatrix * viewMatrix * positionWS;
}
