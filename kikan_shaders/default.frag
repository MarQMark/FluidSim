#version 430 core

in vec4 v_color;
in vec2 v_texCoords;
in float v_textureSlot;

layout(location = 0) out vec4 fragColor;

uniform sampler2D u_sampler;

void main() {
    vec4 color = v_color;

    color = mix(color, texture(u_sampler, v_texCoords), step(0., v_textureSlot));

    fragColor = color;
}