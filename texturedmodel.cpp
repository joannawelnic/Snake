#include "texturedmodel.h"
#include <fstream>
#include <sstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TexturedModel::TexturedModel() : vao(0), vboVertices(0), vboUVs(0), texture(0), vertexCount(0) {}

TexturedModel::~TexturedModel() {
    cleanup();
}

void TexturedModel::readFromOBJ(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempUVs;
    std::vector<GLuint> indices;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;
        if (prefix == "v") {
            glm::vec3 v;
            ss >> v.x >> v.y >> v.z;
            tempVertices.push_back(v);
        }
        else if (prefix == "vt") {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            tempUVs.push_back(uv);
        }
        else if (prefix == "f") {
            for (int i = 0; i < 3; ++i) {
                std::string token;
                ss >> token;
                size_t slash1 = token.find('/');
                size_t slash2 = token.find('/', slash1 + 1);
                int vi = std::stoi(token.substr(0, slash1)) - 1;
                int ti = std::stoi(token.substr(slash1 + 1, slash2 - slash1 - 1)) - 1;
                vertices.push_back(tempVertices[vi]);
                uvs.push_back(tempUVs[ti]);
            }
        }
    }

    vertexCount = vertices.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vboUVs);
    glBindBuffer(GL_ARRAY_BUFFER, vboUVs);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void TexturedModel::loadTexture(const std::string& filename) {
    int width, height, channels;
    unsigned char* image = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb);

    if (!image) {
        std::cerr << "Error loading texture: " << filename << std::endl;
        return;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Ustawienia tekstury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image);
}

void TexturedModel::drawTextured(bool smooth) {
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

void TexturedModel::cleanup() {
    if (vboVertices) glDeleteBuffers(1, &vboVertices);
    if (vboUVs) glDeleteBuffers(1, &vboUVs);
    if (vao) glDeleteVertexArrays(1, &vao);
    if (texture) glDeleteTextures(1, &texture);
}
