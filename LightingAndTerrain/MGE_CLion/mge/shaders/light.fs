#version 400

in vec2 texCoord;
in vec3 fragmentPosition;
in vec3 fragmentNormal;

//object texture
uniform sampler2D diffuseTexture;

//ambient
uniform vec3 ambientLightColor; //world space
uniform float ambientIntensity;

//diffuse
uniform vec3 lightPos; //world space
uniform vec3 lightColor;
uniform float lightIntensity;

//specular
uniform vec3 cameraPos;
uniform float specularStrength;
uniform float shininess;

uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

out vec4 fragColor;

void main()
{

    float distance = length(lightPos - fragmentPosition);
    float attenuation = 1.0 / (constantAttenuation + linearAttenuation * distance + quadraticAttenuation * distance * distance);

    vec3 objectColor = texture(diffuseTexture, texCoord).rgb;
    //ambient light = color * intensity * object color
    vec3 ambient = ambientIntensity * ambientLightColor * objectColor * attenuation;

    //direction from surface to light
    vec3 lightDirection = normalize(lightPos - fragmentPosition);

    //make unit vector
    vec3 normalized = normalize(fragmentNormal);

    //angle between surface and light
    //dot calculates difference normal and light direction (max removes negative so it's between 0 and 1)
    float difference = max(dot(normalized, lightDirection), 0.0);

    //multiply for final diffuse color
    vec3 diffuse = difference * lightColor * objectColor * lightIntensity;

    //specular
    vec3 viewDirection = normalize(cameraPos - fragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normalized);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    //combine ambient and diffuse
    vec3 result = ambient + (diffuse + specular) * attenuation;

    //output
    fragColor = vec4(result, 1.0);
}