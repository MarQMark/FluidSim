#version 430 core

in vec4 v_color;
in vec2 v_texCoords;
in float v_textureSlot;

layout(location = 0) out vec4 fragColor;

uniform sampler2D u_sampler;

void main() {
    if(texture(u_sampler, v_texCoords).a < 0.1)
        discard;

    fragColor = vec4(texture(u_sampler, v_texCoords).rgb, 1.);
}