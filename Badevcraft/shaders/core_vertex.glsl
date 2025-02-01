#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in mat4 instanceMatrix;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoord;
out float FogFactor;

void main() {
    vec4 worldPosition =  model * vec4(aPos, 1.0);

    TexCoord = aTexCoord;
    gl_Position = projection * view * worldPosition;
}
