#version 330

in vec3 vertex;
in vec3 color;
uniform vec2 offset;
//uniform float rotation;
//uniform int checkerType;

out vec3 fColor;

void main (void) {
    //float cosAngle = cos(rotation);
    //float sinAngle = sin(rotation);

    //mat2 rotationMatrix = mat2(
            //cosAngle, -sinAngle,
            //sinAngle, cosAngle
    //);

    //vertex.xy is the original pos of vertex and we multiply with the matrix to add the rotation
    //vec2 rotatedPosition = rotationMatrix * vertex.xy;

    //offset to keep moving around the center but now with rotation
    //z can be 0 since we're working in 2D
    //w is 1 so there won't be perspective scaling (openGL uses homogeneous coords (x,y,z,w))
    //gl_Position = vec4(rotatedPosition + offset, 0.0, 1.0);

    gl_Position = vec4(vertex.xy + offset, vertex.z, 1.0);
    fColor = color;
}

