#version 330 core

in vec3 fragPositionMS;
in vec3 fragPositionWS;
in vec3 fragNormalWS;
in vec2 fragTexCoords;

in mat3 fragTBN;

uniform vec3 cameraPosWS;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D heightTexture;

out vec4 fragColor;

vec3 getBumpedNormal(vec2 texCoords)
{
    vec3 bumpedNormal = texture(normalTexture, texCoords).xyz;
    bumpedNormal = 2.0f * bumpedNormal - 1.0f;
    bumpedNormal = normalize(fragTBN * bumpedNormal);
    return bumpedNormal;
}

vec2 getParallaxTexCoords()
{
    vec3 viewDirTS = normalize(transpose(fragTBN) * (cameraPosWS - fragPositionWS));
    
    float minNumLayers = 10.0f;
    float maxNumLayers = 20.0f;
    float numLayers = mix(maxNumLayers, minNumLayers, abs(viewDirTS.z));
    float layerDepth = 1.0f / numLayers;

    float heightScale = 0.04f;
    vec2 parallaxOffset = heightScale * viewDirTS.xy / viewDirTS.z;
    vec2 deltaTexCoords = parallaxOffset / numLayers;

    vec2 currentTexCoords = fragTexCoords;
    float currentDepthValue = texture(heightTexture, currentTexCoords).r;
    float currentLayerDepth = 0.0f;
    while (currentLayerDepth < currentDepthValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthValue = texture(heightTexture, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
    float depthBeforeCollision = texture(heightTexture, currentTexCoords).r - currentLayerDepth + layerDepth;
    float depthAfterCollision = currentDepthValue - currentLayerDepth;

    float weight = depthAfterCollision / (depthAfterCollision - depthBeforeCollision);
    vec2 texCoords = mix(currentTexCoords, prevTexCoords, weight);
    if (texCoords.x < 0.0f || texCoords.x > 1.0f || texCoords.y < 0.0f || texCoords.y > 1.0f)
    {
        discard;
    }
    return texCoords;
}

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
	vec2 texCoords = getParallaxTexCoords();
	vec3 bumpedNormal = getBumpedNormal(texCoords);
	fragColor = texture(diffuseTexture, texCoords) * calculate_lighting(bumpedNormal, lightPosWS, cameraPosWS);
}