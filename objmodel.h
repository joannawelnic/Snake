#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "model.h"
#include "tiny_obj_loader.h"

namespace Models {
    class ObjModel : public Model {
    private:
        GLuint vao, vbo;

    public:
        ObjModel();
        void load(const std::string& filename);
        void drawSolid(bool smooth) override;
        ~ObjModel();

        int vertexCount = 0;
    };
}

#endif
