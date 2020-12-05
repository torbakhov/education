#version 330 core

layout(location = 0) in vec3 positionMS;
layout(location = 1) in vec3 normalMS;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

out vec3 fragPositionMS;
out vec3 fragPositionWS;
out vec3 fragNormalWS;

void main(void)
{
	vec4 positionWS = modelMatrix * vec4(positionMS, 1.0f);
	vec3 normalWS = normalize(normalMatrix * normalMS);
	
	fragPositionMS = positionMS;
    fragPositionWS = positionWS.xyz;

    fragNormalWS = normalWS;
	
    gl_Position = projectionMatrix * viewMatrix * positionWS;
}
