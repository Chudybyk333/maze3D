#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform bool selected;

void main() {
    vec4 texColor = texture(texture1, TexCoord);
    if (selected) {
        texColor.rgb = mix(texColor.rgb, vec3(1.0, 1.0, 0.0), 0.5); // np. ?ó?ty efekt zaznaczenia
    }
    FragColor = texColor;
}
