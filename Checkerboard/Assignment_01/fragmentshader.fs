#version 330

in vec3 fColor;
out vec4 sColor;

uniform float columns;
uniform float rows;

//mousePos is in pixels from SFML, needs to be normalized for OpenGL
uniform vec2 mousePos;
uniform vec2 screenSize;

uniform float rotation;

uniform int checkerType;


void main (void) {
    //sColor = vec4(fColor,1);
    //return;

         //using fColor.r / fColor.b as UV's
         vec2 uv = vec2(fColor.r, fColor.b);

         // shift so center of rotation is middle of the quad (0.5, 0.5)
         uv -= 0.5;

         // apply rotation around the center
         float c = cos(rotation);
         float s = sin(rotation);
         mat2 R = mat2(c, -s, s, c);
         uv = R * uv;

         // shift back
         uv += 0.5;

    if(checkerType == 0) { //regular checkerboard
        //scale and round with floor before checking with modulo so its whole numbers
        float u = floor(uv.x * columns);
        float v = floor(uv.y * rows);

        //mod takes value and modulo remainder
        if (mod(u+v, 2.0) == 0.0) {
            sColor = vec4(0.5, 0.5, 0.5, 1.0);
        }
        else {
            sColor = vec4(0.0, 0.0, 0.0, 1.0);
        }
    }
    else { //radial checkerboard
        //convert from cartesian to polar coords
        //-0.5 UV's to move center
        float radius = sqrt(pow(uv.x - 0.5,2) + pow(uv.y - 0.5,2));
        float angle = atan((uv.y - 0.5) / (uv.x - 0.5)); //angle of UV vector

        float u = floor(radius * columns);
        float v = floor(angle * rows);

        if (mod(u+v, 2.0) == 0.0) {
            sColor = vec4(0.5, 0.5, 0.5, 1.0);
        }
        else {
            sColor = vec4(0.0, 0.0, 0.0, 1.0);
        }

        //sColor = vec4(angle, mod(angle*10,1),0,1); // show angle
        //sColor = vec4(radius,mod(radius*10,1),0,1); // show radius
    }

    //SFML has pixels screen space, top left is 0,0 and there is width/height for bottom right
    //OpenGL has Normalized Device Coords (NDC),center of the screen is 0,0 and positions go from -1 to 1
    //in OpenGL left and bottom are -1 & right and top are +1

    //normalize positions from SFML's pixel space to OpenGL's NDC space
    //pixelPosition / screenSize to get range of (0,1)
    //then * 2 to scale to (0,2)
    //and finally -1 to shift pos down to NDC range
    vec2 fragmentPosNorm = (gl_FragCoord.xy / screenSize) * 2.0 - 1.0;
    vec2 mousePosNorm = (mousePos / screenSize) * 2.0 - 1.0;
    //flip y-axis to convert because SFML (0,0) top left and OpenGL gl_FragCoord is (0,0) bottom left
    mousePosNorm.y = -mousePosNorm.y;

    //calculate brightness based on mouse distance
    //clamp so the checker pattern doesn't disappear when the mouse isn't on it (min 0.2 visibility)
    //get distance between mouse and fragment pos and make it less bright when far
    //formula 1 - length(distance - mousePos)
    //3.0 is the radius of the brightness (higher smaller radius)
    float brightness = clamp(1.0 - length(fragmentPosNorm - mousePosNorm) * 1.5, 0.2, 1.0);

    //lighting effect
    //mix takes current colors (checkers), target color (white) and factor (blend amount between 0.0 and 1.0)
    //brightness controls the blend amount (more or less white mixed with the original colors)
    sColor.rgb = mix(sColor.rgb, vec3(1.0, 1.0, 1.0), brightness * 0.5);
}

