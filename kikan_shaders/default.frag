#version 430 core

in vec4 v_color;
in vec2 v_texCoords;
in float v_textureSlot;

layout(location = 0) out vec4 fragColor;

uniform sampler2D u_texture[gl_MaxTextureImageUnits];

void main() {
    vec4 color = v_color;

    color = mix(color, texture(u_texture[int(v_textureSlot)], v_texCoords), step(0., v_textureSlot));

    fragColor = color;
}