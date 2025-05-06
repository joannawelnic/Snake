#version 330 core

layout (location = 0) in vec3 aPos;       // Pozycje wierzcho³ków
layout (location = 1) in vec2 aTexCoord;  // Wspó³rzêdne tekstur

out vec2 TexCoord;   // Przekazanie wspó³rzêdnych tekstury do fragment shader

uniform mat4 M;  // Macierz modelu
uniform mat4 V;  // Macierz widoku
uniform mat4 P;  // Macierz projekcji

void main() {
    gl_Position = P * V * M * vec4(aPos, 1.0);  // Obliczanie pozycji w przestrzeni ekranu
    TexCoord = aTexCoord;  // Przekazanie wspó³rzêdnych tekstury do fragment shader
}
