#version 430 core

uniform sampler2D tex;
uniform vec2 center;
uniform float scale;
uniform int iter;

in vec4 pos;

out vec4 fColor;

void main() {
    vec2 z, c;

    c.x = 1.3333 * (pos.x - 0.5) * scale - center.x;
    c.y = (pos.y - 0.5) * scale - center.y;

    int i;
    z = c;
    for(i=0; i<iter; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
    }

    fColor = texture(tex, vec2((i == iter ? 0.0 : float(i)) / 100.0, 0.0));
}