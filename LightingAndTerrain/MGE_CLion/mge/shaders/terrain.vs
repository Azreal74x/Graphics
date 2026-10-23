#version 400

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform sampler2D heightMap;
uniform float heightScale;

out vec3 fragmentNormal;
out vec3 fragmentPosition;
out vec2 texCoord;


void main() {

    // sample height value from height map using vertex UV
    float heightValue = texture(heightMap, uv).r;

    // add height scale to make terrain bigger or smaller
    float displacement = heightValue * heightScale;

    // move vertex up in world space
    vec3 displacedVertex = vertex;
    displacedVertex.y += displacement;

    // transform vertex to world/clip space
    vec4 worldPos = modelMatrix * vec4(displacedVertex, 1.0);
    fragmentPosition = worldPos.xyz;
    fragmentNormal = mat3(modelMatrix) * normal;
    texCoord = uv;

    gl_Position = projectionMatrix * viewMatrix * worldPos;
}