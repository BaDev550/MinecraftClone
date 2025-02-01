#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D textureAtlas;

void main() {
    vec4 texColor = texture(textureAtlas, TexCoord);
    FragColor = texColor * vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
