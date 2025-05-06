#define GLM_FORCE_RADIANS 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "constants.h"
#include "allmodels.h"
#include "shaderprogram.h"
#include "snake.h"

Snake snake(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 5); // Start, kierunek, d³ugoœæ
glm::vec3 direction(1.0f, 0.0f, 0.0f);

float snakeSpeed = 2.0f;
float movementTimer = 0.0f;

void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_UP) direction = glm::vec3(0, 0, 1);
        if (key == GLFW_KEY_DOWN) direction = glm::vec3(0, 0, -1);
        if (key == GLFW_KEY_LEFT) direction = glm::vec3(1, 0, 0);
        if (key == GLFW_KEY_RIGHT) direction = glm::vec3(-1, 0, 0);
    }
}

void initModels() {
    // Modele jeœli bêd¹ wymagane
}

void freeModels() {
    // Zwolnienie modeli
}
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();
    // Tworzymy obiekt programu cieniuj¹cego
    /*spLambertTextured = new ShaderProgram(
        "C:/Users/Tomasz/Downloads/g3d_st_04_win/g3d_st_04_win/vertex_shader_textured.glsl",  // Vertex shader
        NULL,                                  // Brak geometry shader (mo¿na to pomin¹æ, jeœli nie potrzebujesz)
        "C:/Users/Tomasz/Downloads/g3d_st_04_win/g3d_st_04_win/fragment_shader_textured.glsl" // Fragment shader
    );*/

    //loadModels();  // £adowanie modeli i tekstur (jak wczeœniej)
    glClearColor(0.1f, 0.1f, 0.1f, 1);
    glEnable(GL_DEPTH_TEST);
    glfwSetKeyCallback(window, key_callback);
}


void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //freeModels();
    //delete spLambertTextured;
}

void drawApple(glm::vec3 position, ShaderProgram* spLambert, glm::mat4 P, glm::mat4 V, bool smooth = true) {
    spLambert->use();

    glUniformMatrix4fv(spLambert->u("P"), 1, GL_FALSE, glm::value_ptr(P));
    glUniformMatrix4fv(spLambert->u("V"), 1, GL_FALSE, glm::value_ptr(V));

    glm::mat4 M = glm::translate(glm::mat4(1.0f), position);
    M = glm::scale(M, glm::vec3(1.0f));  // Zamiast 0.5f

    glUniformMatrix4fv(spLambert->u("M"), 1, GL_FALSE, glm::value_ptr(M));

    glUniform4f(spLambert->u("color"), 1.0f, 0.0f, 0.0f, 1.0f); // Czerwone jab³ko!

    ModelResources::apple.drawSolid(smooth);
    glBindVertexArray(0);
}


void drawScene(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 P = glm::perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
    glm::mat4 V = glm::lookAt(glm::vec3(0, 5, -10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    spLambert->use();//Aktywacja programu cieniuj¹cego
    glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P)); //Za³adowanie macierzy rzutowania do programu cieniuj¹cego
    glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V)); //Za³adowanie macierzy widoku do programu cieniuj¹cego

    // U¿yj odpowiedniego shader programu
    //spLambertTextured->use();
    //glUniformMatrix4fv(spLambertTextured->u("projection"), 1, GL_FALSE, glm::value_ptr(P));
    //glUniformMatrix4fv(spLambertTextured->u("view"), 1, GL_FALSE, glm::value_ptr(V));

    // Rysowanie wê¿a
    snake.render(spLambert, true);
    //drawApple(glm::vec3(0, 0, 0), spLambert, P, V);
    glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    M = glm::scale(M, glm::vec3(1.0f));  // Zamiast 0.5f

    glUniformMatrix4fv(spLambert->u("M"), 1, GL_FALSE, glm::value_ptr(M));

    glUniform4f(spLambert->u("color"), 1.0f, 0.0f, 0.0f, 1.0f); // Czerwone jab³ko!

    ModelResources::apple.drawSolid(true);

    // Rysowanie kafelków
    int gridSize = 10;
    for (int x = -gridSize / 2; x < gridSize / 2; ++x) {
        for (int z = -gridSize / 2; z < gridSize / 2; ++z) {
            glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3((float)x, -0.5f, (float)z));
            //glUniformMatrix4fv(spLambertTextured->u("model"), 1, GL_FALSE, glm::value_ptr(M));
            glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));
            glUniform4f(spLambert->u("color"), 0, 1, 0, 1);
            Models::cube.drawSolid();
            // Rysuj kafelek z tekstur¹
            //ModelResources::tile.drawTextured(true);
        }
    }
    
    glfwSwapBuffers(window);
}


int main() {
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) exit(EXIT_FAILURE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake 3D", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "GLEW Init Failed\n");
        exit(EXIT_FAILURE);
    }

    initOpenGLProgram(window);  // Inicjalizacja shaderów i modeli

    loadModels();  // £adowanie modeli i tekstur

    glfwSetTime(0);
    while (!glfwWindowShouldClose(window)) {
        float deltaTime = glfwGetTime();
        glfwSetTime(0);

        movementTimer += deltaTime;
        if (movementTimer >= 1.0f / snakeSpeed) {
            snake.setDirection(direction);
            snake.move(); // Ruch wê¿a co tick
            // if (kolizja z jab³kiem) snake.grow();
            movementTimer = 0.0f;
        }

        drawScene(window);  // Rysowanie sceny (wê¿a i kafelków)
        glfwPollEvents();
    }

    freeOpenGLProgram(window);  // Zwolnienie zasobów
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

 