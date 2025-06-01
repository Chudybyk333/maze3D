#version 330 core
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform float emissiveStrength;

out vec4 FragColor;

void main() {
vec3 baseColor = texture(texture_diffuse1, TexCoords).rgb;
vec3 emissive = emissiveStrength * vec3(0.2, 1.0, 0.4); // jasnozielony blask
FragColor = vec4(baseColor + emissive, 1.0);
}