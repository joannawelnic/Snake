// snake.h
#ifndef SNAKE_H
#define SNAKE_H

#include <glm/glm.hpp>
#include <vector>
#include "shaderprogram.h"
#include <utility>

class Snake {
public:
    Snake(glm::vec3 startPosition, glm::vec3 direction, int initialLength);

    void move();
    void grow();
    void render(ShaderProgram* sp, bool smooth);

    const std::vector<std::pair<glm::vec3, glm::vec3>>& getSegments() const;
    void setDirection(const glm::vec3& dir);
    glm::vec3 getHeadPosition() const;

private:
    std::vector<std::pair<glm::vec3, glm::vec3>> segments; // czyli: (pozycja, kierunek) dla kazdego segmentu
    glm::vec3 direction;
    std::vector<glm::vec3> directions;
};

#endif
#pragma once
