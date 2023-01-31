#version 430 core

in vec4 v_color;
in vec2 v_texCoords;
in float v_textureSlot;

layout(location = 0) out vec4 fragColor;

uniform sampler2D u_sampler;
uniform float u_zoom;
uniform vec2 u_cell_count;
uniform vec2 u_resolution;

void main() {
    vec2 pos = vec2(gl_FragCoord) / u_resolution * u_zoom;
    pos += .5 - u_zoom / 2.; // Correcting zoom for middle not (0|0)
    pos += 1 / u_cell_count; // Wierd Offset idk
    pos += vec2(0, (1 - u_cell_count.x / u_cell_count.y) * (1 - u_cell_count.y / u_cell_count.x)); // Fitting for cell ratio
    pos *= vec2(max(u_cell_count.x, u_cell_count.y));
    float r = floor(pos.y);
    float c = floor(pos.x);

    vec2 min = vec2(c, r);
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            vec2 c_pos = vec2(c + x, r + y);
            if(texture(u_sampler, min / u_cell_count).b >= texture(u_sampler, c_pos / u_cell_count).b && texture(u_sampler, c_pos / u_cell_count).a != 0){
                if(distance(pos, c_pos + vec2(.5, .5)) < .8)
                    min = c_pos;
            }
        }
    }

    fragColor = texture(u_sampler, min / u_cell_count);
}