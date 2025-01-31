#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in float FogFactor;

uniform sampler2D textureAtlas;
uniform vec3 fogColor;
uniform float fogStart;
uniform float fogEnd;

void main() {
    vec4 textureColor = texture(textureAtlas, TexCoord);
    float fogDensity = exp(-FogFactor * FogFactor * 0.0001);
    fogDensity = clamp(fogDensity, 0.0, 1.0);
    vec3 foggedColor = mix(textureColor.rgb, fogColor, fogDensity);

    //FragColor = vec4(foggedColor, textureColor.a);
    FragColor = textureColor;
}
