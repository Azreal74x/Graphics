#version 400

in vec2 texCoord;
in vec3 fragmentNormal;
in vec3 fragmentPosition;

uniform sampler2D splatMap;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;

out vec4 fragColor;

void main() {
    // get splatmap values
    vec4 weights = texture(splatMap, texCoord).rgba;

    // normalize to make total 1
    weights = normalize(weights);

    // sample each texture
    vec3 tex1 = texture(texture1, texCoord * 20.0).rgb; // repeated tiling
    vec3 tex2 = texture(texture2, texCoord * 20.0).rgb;
    vec3 tex3 = texture(texture3, texCoord * 20.0).rgb;
    vec3 tex4 = texture(texture4, texCoord * 20.0).rgb;

    // combine textures depending on splatmap weight
    vec3 blendedColor = tex1 * weights.r + tex2 * weights.g + tex3 * weights.b + tex4 * weights.a;

    // without light
    fragColor = vec4(blendedColor, 1.0);
}