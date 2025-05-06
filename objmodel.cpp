#include "objmodel.h"
#include <iostream>

using namespace Models;

ObjModel::ObjModel() : vao(0), vbo(0), vertexCount(0) {}

void ObjModel::load(const std::string& filename) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());

    if (!ret) {
        std::cerr << "Failed to load OBJ file: " << err << std::endl;
        return;
    }

    std::vector<float> data;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            int vIdx = 3 * index.vertex_index;
            data.push_back(attrib.vertices[vIdx]);
            data.push_back(attrib.vertices[vIdx + 1]);
            data.push_back(attrib.vertices[vIdx + 2]);

            if (index.normal_index >= 0) {
                int nIdx = 3 * index.normal_index;
                data.push_back(attrib.normals[nIdx]);
                data.push_back(attrib.normals[nIdx + 1]);
                data.push_back(attrib.normals[nIdx + 2]);
            }
            else {
                data.push_back(0.0f); data.push_back(0.0f); data.push_back(1.0f);
            }
        }
    }

    vertexCount = data.size() / 6;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // pozycje
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1); // normalne
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
}

void ObjModel::drawSolid(bool /*smooth*/) {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
    if (vao == 0 || vertexCount == 0) {
        std::cerr << "B³¹d: VAO = 0 lub vertexCount = 0!" << std::endl;
    }

}

ObjModel::~ObjModel() {
    if (vbo) glDeleteBuffers(1, &vbo);
    if (vao) glDeleteVertexArrays(1, &vao);
}
