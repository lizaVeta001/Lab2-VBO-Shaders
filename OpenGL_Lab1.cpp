#define GLEW_DLL
#define GLFW_DLL

#include <iostream>
#include <cmath>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "shader.h"

// Текст вершинного шейдера
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

// Текст фрагментного шейдера
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 ourColor;
void main() {
    FragColor = ourColor;
}
)";

int main() {
    // Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // ========== ИСПРАВЛЕННЫЕ НАСТРОЙКИ ==========
    // Используем OpenGL 3.3 (вместо 4.6)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(800, 600, "Variant 12 - Rectangle", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        std::cerr << "Check if your GPU supports OpenGL 3.3" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Инициализация GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    // Вывод информации об OpenGL
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // Данные прямоугольника (вариант 12)
    float vertices[] = {
        // Первый треугольник
        -0.6f, -0.4f, 0.0f,
         0.6f, -0.4f, 0.0f,
        -0.6f,  0.4f, 0.0f,
        // Второй треугольник
         0.6f, -0.4f, 0.0f,
         0.6f,  0.4f, 0.0f,
        -0.6f,  0.4f, 0.0f
    };

    // VAO, VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Создание шейдерной программы через библиотеку
    Shader shaderProgram(vertexShaderSource, fragmentShaderSource);

    // Цвет фона (белый)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Основной цикл
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // ЗАДАНИЕ №1: изменение цвета от времени
        float timeValue = (float)glfwGetTime();
        float r = 0.2f + 0.3f * sin(timeValue);
        float g = 0.8f + 0.5f * cos(timeValue * 0.7f);
        float b = 0.8f + 0.5f * sin(timeValue * 0.5f);

        // Используем шейдер и передаем цвет
        shaderProgram.use();
        shaderProgram.setVec4("ourColor", r, g, b, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Очистка
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}