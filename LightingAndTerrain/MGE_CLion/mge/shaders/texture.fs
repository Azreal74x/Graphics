// //DIFFUSE TEXTURE FRAGMENT SHADER
// #version 330 // for glsl version (12 is for older versions , say opengl 2.1

// uniform sampler2D diffuseTexture;
// in vec2 texCoord;
// out vec4 fragment_color;

// void main( void ) {
// 	fragment_color = texture(diffuseTexture,texCoord);
// }
//DIFFUSE TEXTURE FRAGMENT SHADER
#version 400

uniform sampler2D diffuseTexture;

uniform sampler2D diffuse1;
uniform sampler2D diffuse2;
uniform sampler2D diffuse3;
uniform sampler2D diffuse4;
uniform sampler2D splatMap;

in vec2 texCoord;

out vec4 fragment_color;

void main( void ) {
	fragment_color = texture(diffuseTexture,texCoord);
}
