#version 430 core

in vec4 v_color;
in vec2 v_texCoords;
in float v_textureSlot;

layout(location = 0) out vec4 fragColor;

uniform sampler2D u_sampler;

void main() {
    fragColor = vec4(v_color.rgb, texture(u_sampler, v_texCoords).a);
}