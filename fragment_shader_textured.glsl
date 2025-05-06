#version 330 core

out vec4 FragColor;  // Kolor fragmentu

in vec2 TexCoord;    // Wsp�rz�dne tekstury z vertex shader

uniform sampler2D texture1;  // Tekstura

void main() {
    FragColor = texture(texture1, TexCoord);  // U�ywamy tekstury do wy�wietlania koloru fragmentu
}
