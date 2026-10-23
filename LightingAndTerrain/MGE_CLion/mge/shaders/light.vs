#version 400
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

//uniform mat4 mvpMatrix;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 texCoord;
out vec3 fragmentPosition;
out vec3 fragmentNormal;

void main( void ){
    //transform vertex position (object space to world space)
    vec4 worldPos = modelMatrix * vec4(vertex, 1.0);
    fragmentPosition = worldPos.xyz;

    //transform normal (object space to world space)
    //using rotation part of matrix (3x3), skip translation (4x4)
    fragmentNormal = mat3(modelMatrix) * normal;

    //gl_Position = mvpMatrix * vec4(vertex, 1.f);
    gl_Position = projectionMatrix * viewMatrix * worldPos;
	texCoord = uv;
}