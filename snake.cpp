// snake.cpp
#include "snake.h"
#include "cube.h" // Twoja implementacja szeœcianu
#include "snake_segment.h"
#include <glm/gtc/matrix_transform.hpp> // dla translate i scale
#include <glm/gtc/type_ptr.hpp> // dla value_ptr u¿ywanego przy setUniform
#include "shaderprogram.h" // Potrzebne do u¿ycia metody u()
#include <utility>

#include <glm/gtx/quaternion.hpp>

Snake::Snake(glm::vec3 startPosition, glm::vec3 direction, int initialLength)
    : direction(direction)
{
    for (int i = 0; i < initialLength; ++i) {
        glm::vec3 segmentPosition = startPosition - (float)i * direction;
        segments.push_back({ segmentPosition, direction });
        directions.push_back(direction); // ka¿dy segment startuje z tym samym kierunkiem
    }
}

void Snake::move()
{
    glm::vec3 newHeadPos = segments.front().first + direction;
    // Wstaw now¹ g³owê
    segments.insert(segments.begin(), std::make_pair(newHeadPos, direction));
    // Usuñ ogon
    segments.pop_back();

    directions.insert(directions.begin(), direction);
    directions.pop_back();
}

void Snake::grow()
{
    segments.push_back(segments.back()); // Powiel pozycjê i kierunek ogona
}

void Snake::render(ShaderProgram* sp, bool smooth)
{
    sp->use(); // Upewnij siê, ¿e shader jest aktywny

    for (const auto& segment : segments)
    {
        // dodatnie pozycji kazdego segmentu aby obracaly sie po kolei od zmienienia kierunku
        glm::vec3 position = segment.first;
        glm::vec3 segDir = segment.second;

        glm::vec3 up(0.0f, 0.0f, 1.0f);
        glm::vec3 down(0.0f, 0.0f, -1.0f);
        glm::vec3 left(1.0f, 0.0f, 0.0f);
        glm::vec3 right(-1.0f, 0.0f, 0.0f);

        glm::mat4 M = glm::translate(glm::mat4(1.0f), position);
        int i = 0;
        glm::vec3 currentDir = directions[i];
        glm::vec3 nextDir;
        glm::vec3 segDir2;
        if (i < directions.size()) {
            nextDir = directions[i + 1];
            segDir2 = segments[i+1].second;
        }

        // Gdy kierunek siê zmienia (zakrêt)
        //if (currentDir != nextDir) {
            glm::vec3 turnAxis;
            /*
            if (segDir == left && segDir2 == up) { // lewo -> góra
                turnAxis = glm::vec3(0, -1, 0); // rotacja w prawo
                M = glm::rotate(M, glm::radians(45.0f), turnAxis); // FAKE zakrêt
            }
            else if (currentDir == glm::vec3(0, 0, 1) && nextDir == glm::vec3(1, 0, 0)) // góra -> lewo
                turnAxis = glm::vec3(0, 1, 0); // rotacja w lewo
            else if (currentDir == glm::vec3(-1, 0, 0) && nextDir == glm::vec3(0, 0, -1)) // prawo -> dó³
                turnAxis = glm::vec3(0, 1, 0);
            else if (currentDir == glm::vec3(0, 0, -1) && nextDir == glm::vec3(-1, 0, 0)) // dó³ -> prawo
                turnAxis = glm::vec3(0, -1, 0);
            else if (currentDir == glm::vec3(0, 0, 1) && nextDir == glm::vec3(-1, 0, 0)) // góra -> prawo
                turnAxis = glm::vec3(0, -1, 0);
            else if (currentDir == glm::vec3(-1, 0, 0) && nextDir == glm::vec3(0, 0, 1)) // prawo -> góra
                turnAxis = glm::vec3(0, 1, 0); 
            */
            // ...dodaj inne kombinacje jeœli potrzeba
            int index = 0;
            if (segDir == left && segDir2 == up) {
                index = i;
            }
            
       // }
       // else {
            int flag = 0;
            M = glm::scale(M, glm::vec3(1.0f)); // Skalowanie walca
            //obort aby walce byly w dobra strone
            if (segDir == left && segDir2 == up ) { // lewo -> góra
                //turnAxis = glm::vec3(0, -1, 0); // rotacja w prawo
                M = glm::rotate(M, glm::radians(45.0f), glm::vec3(0, -1, 0)); // FAKE zakrêt
                flag = 1;
            } 
            M = glm::scale(M, glm::vec3(1.0f)); // Skalowanie walca
            if (flag == 1) {
                M = glm::rotate(M, glm::radians(-45.0f), glm::vec3(0, -1, 0)); // byc moze po rotacji wyzej nastepne tez sie przerotowuja, musi byc reset wczesniejszej rotacji
            }
            if (segDir == up) {

                M = glm::rotate(M, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            if (segDir == down) {
                M = glm::rotate(M, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
            }
            if (segDir == left) {
                M = glm::rotate(M, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            }
            if (segDir == right) {
                M = glm::rotate(M, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            }
            else {
                M = glm::rotate(M, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            }
        //}

        // Pobierz lokalizacjê uniforma "M" z shaderów
        GLuint location = sp->u("M");
        // Przeka¿ macierz modelu do shaderów
        glUniformMatrix4fv(location, 1, GL_FALSE, &M[0][0]);

        // Rysuj segment wê¿a jako walec
        Models::snakeSegment.drawSolid(smooth);
        i++;
    }
}

const std::vector<std::pair<glm::vec3, glm::vec3>>& Snake::getSegments() const {
    return segments;
}

void Snake::setDirection(const glm::vec3& dir) {
    direction = dir;
}

glm::vec3 Snake::getHeadPosition() const {
    return segments.front().first;
}
