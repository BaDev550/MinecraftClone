#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 cameraPos;

out vec2 TexCoord;
out float FogFactor;

void main() {
    vec4 worldPosition = model * vec4(aPos, 1.0);
    vec3 viewDirection = cameraPos - worldPosition.xyz;
    FogFactor = length(viewDirection);

    TexCoord = aTexCoord;
    gl_Position = projection * view * worldPosition;
}
