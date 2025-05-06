#include "snake_segment.h"
#include <cmath>
#include "constants.h"

namespace Models {

    SnakeSegment snakeSegment;

    SnakeSegment::SnakeSegment() {
        vertices = SnakeSegmentInternal::vertices;
        normals = SnakeSegmentInternal::normals;
        vertexNormals = SnakeSegmentInternal::vertexNormals;
        texCoords = SnakeSegmentInternal::texCoords;
        colors = SnakeSegmentInternal::colors;
        vertexCount = SnakeSegmentInternal::vertexCount;
    }

    SnakeSegment::~SnakeSegment() {
    }

    void SnakeSegment::drawSolid(bool smooth) {
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, vertices);
        glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, smooth ? vertexNormals : normals);
        glVertexAttribPointer(2, 4, GL_FLOAT, false, 0, texCoords);
        glVertexAttribPointer(3, 4, GL_FLOAT, false, 0, colors);

        glDrawArrays(GL_TRIANGLES, 0, vertexCount);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
    }

    namespace SnakeSegmentInternal {

        constexpr int slices = 20;
        constexpr int stacks = 1;
        constexpr float radius = 0.5f;
        constexpr float height = 1.0f;
        constexpr int vertexPerQuad = 6;
        constexpr int totalQuads = slices * stacks;
        constexpr int maxVertices = totalQuads * vertexPerQuad;

        float vertices[maxVertices * 4];
        float normals[maxVertices * 4];
        float vertexNormals[maxVertices * 4];
        float texCoords[maxVertices * 4];
        float colors[maxVertices * 4];
        unsigned int vertexCount = maxVertices;

        void generateCylinder() {
            int index = 0;
            for (int i = 0; i < slices; ++i) {
                float theta = (2.0f * PI * i) / slices;
                float nextTheta = (2.0f * PI * (i + 1)) / slices;

                float x0 = cosf(theta) * radius;
                float z0 = sinf(theta) * radius;
                float x1 = cosf(nextTheta) * radius;
                float z1 = sinf(nextTheta) * radius;

                for (int j = 0; j < stacks; ++j) {
                    float y0 = -height / 2 + j * (height / stacks);
                    float y1 = -height / 2 + (j + 1) * (height / stacks);

                    float quadVerts[6][4] = {
                        {x0, y0, z0, 1.0f}, {x1, y1, z1, 1.0f}, {x0, y1, z0, 1.0f},
                        {x0, y0, z0, 1.0f}, {x1, y0, z1, 1.0f}, {x1, y1, z1, 1.0f}
                    };

                    for (int k = 0; k < 6; ++k) {
                        float* v = quadVerts[k];
                        vertices[index * 4 + 0] = v[0];
                        vertices[index * 4 + 1] = v[1];
                        vertices[index * 4 + 2] = v[2];
                        vertices[index * 4 + 3] = 1.0f;

                        normals[index * 4 + 0] = v[0];
                        normals[index * 4 + 1] = 0.0f;
                        normals[index * 4 + 2] = v[2];
                        normals[index * 4 + 3] = 0.0f;

                        float len = sqrtf(v[0] * v[0] + v[2] * v[2]);
                        vertexNormals[index * 4 + 0] = v[0] / len;
                        vertexNormals[index * 4 + 1] = 0.0f;
                        vertexNormals[index * 4 + 2] = v[2] / len;
                        vertexNormals[index * 4 + 3] = 0.0f;

                        texCoords[index * 4 + 0] = (float)i / slices;
                        texCoords[index * 4 + 1] = (float)j / stacks;
                        texCoords[index * 4 + 2] = 0.0f;
                        texCoords[index * 4 + 3] = 0.0f;

                        colors[index * 4 + 0] = 0.0f;
                        colors[index * 4 + 1] = 0.8f;
                        colors[index * 4 + 2] = 0.2f;
                        colors[index * 4 + 3] = 1.0f;

                        ++index;
                    }
                }
            }
        }

        struct StaticConstructor {
            StaticConstructor() {
                generateCylinder();
            }
        } staticConstructor;

    } // namespace SnakeSegmentInternal

} // namespace Models
