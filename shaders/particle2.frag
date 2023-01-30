#version 430 core

in vec4 v_color;
in vec2 v_texCoords;
in float v_textureSlot;

layout(location = 0) out vec4 fragColor;

uniform sampler2D u_particle;
uniform float u_size;

uniform int u_smoothing;

#define KERNEL 20
void main() {
    if(texture(u_particle, v_texCoords).a < 0.1)
        discard;

    vec4 color = texture(u_particle, v_texCoords);

    float count = 0.;
    for (int x = -u_smoothing; x <= u_smoothing; x++) {
        for (int y = -u_smoothing; y <= u_smoothing; y++) {
            if(texture(u_particle, v_texCoords).a >= 0.1){
                color += texture(u_particle, v_texCoords + vec2(x / u_size, y / u_size));
                count++;
            }
        }
    }

    color /= count;

    fragColor = vec4(color.rgb, 1.);
}