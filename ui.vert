#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform vec4 rect; // x, y, width, height

out vec2 TexCoord;

void main() {
    vec2 pos = aPos * rect.zw + rect.xy; // skalowanie i przesuni?cie
    gl_Position = vec4(pos * 2.0 - 1.0, 0.0, 1.0); // konwersja na clip space
    TexCoord = aTexCoord;
}
