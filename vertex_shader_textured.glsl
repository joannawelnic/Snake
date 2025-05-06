#version 330 core

layout (location = 0) in vec3 aPos;       // Pozycje wierzcho�k�w
layout (location = 1) in vec2 aTexCoord;  // Wsp�rz�dne tekstur

out vec2 TexCoord;   // Przekazanie wsp�rz�dnych tekstury do fragment shader

uniform mat4 M;  // Macierz modelu
uniform mat4 V;  // Macierz widoku
uniform mat4 P;  // Macierz projekcji

void main() {
    gl_Position = P * V * M * vec4(aPos, 1.0);  // Obliczanie pozycji w przestrzeni ekranu
    TexCoord = aTexCoord;  // Przekazanie wsp�rz�dnych tekstury do fragment shader
}
