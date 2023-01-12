#version 430 core

in vec4 v_color;
in vec2 v_texCoords;
in float v_textureSlot;

layout(location = 0) out vec4 fragColor;

uniform sampler2D u_texture[gl_MaxTextureImageUnits];

void main() {
    if(v_textureSlot != -1 && texture(u_texture[int(v_textureSlot)], v_texCoords).a < 0.4)
        discard;

    if(v_textureSlot == 1){
        fragColor = v_color;
    }
    else{
        fragColor = texture(u_texture[int(v_textureSlot)], v_texCoords);
    }
}