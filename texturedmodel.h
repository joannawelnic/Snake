#ifndef TEXTUREDMODEL_H
#define TEXTUREDMODEL_H

#include <GL/glew.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class TexturedModel {
private:
    GLuint vao;
    GLuint vboVertices, vboUVs;
    GLuint texture;
    int vertexCount;

public:
    TexturedModel();
    ~TexturedModel();

    void readFromOBJ(const std::string& filename);   // Wczytuje model .obj
    void loadTexture(const std::string& filename);   // �aduje tekstur� .jpg/.png
    void drawTextured(bool smooth);                  // Rysuje model
    void cleanup();                                  // Czy�ci pami��
};

#endif
#pragma once
