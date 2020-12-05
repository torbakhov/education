#version 330 core

in vec3 fragPositionMS;
in vec3 fragPositionWS;
in vec3 fragNormalWS;

uniform vec3 cameraPosWS;

out vec4 fragColor;

vec4 calculate_lighting(vec3 normalWS, vec3 lightPositionWS, vec3 cameraPositionWS)
{
	float ambient = 0.1f;
	
	vec3 lightDirUnnormWS = lightPositionWS - fragPositionWS;
    vec3 lightDirWS = normalize(lightDirUnnormWS);
    float diffuse = max(0.0f, dot(normalWS, lightDirWS));

    vec3 viewDirWS = normalize(cameraPositionWS - fragPositionWS);
    vec3 reflectDirWS = reflect(-lightDirWS, normalWS);
    float specular = 0.75f * pow(max(0.0f, dot(viewDirWS, reflectDirWS)), 32);

	float shadow = 0.0f;

	float light_val = ambient + (1.0f - shadow) * (diffuse + specular);
	return vec4(light_val, light_val, light_val, 1.0f);
}

void main()
{
	vec3 lightPosWS = vec3(5,5,5);
	fragColor = vec4(1.0f,0.0f,0.0f,1.0f) * calculate_lighting(fragNormalWS, lightPosWS, cameraPosWS);
}