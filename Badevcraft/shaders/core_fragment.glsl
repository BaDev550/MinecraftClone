#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in float FogFactor;

uniform sampler2D textureAtlas;

void main() {
    vec4 textureColor = texture(textureAtlas, TexCoord);
    if (textureColor.a < 0.1)
        discard;

    FragColor = textureColor;
}
