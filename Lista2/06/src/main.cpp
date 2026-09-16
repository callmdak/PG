#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::vector<float> vertices;

int triangulos = 0;

float cores[][3] =
{
    {1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.0f},
    {1.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 1.0f}
};

std::string loadShader(const char* path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir shader: " << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

GLuint createShaderProgram()
{
    std::string vertexSource = loadShader("shaders/vertex.glsl");
    std::string fragmentSource = loadShader("shaders/fragment.glsl");

    const char* vertexCode = vertexSource.c_str();
    const char* fragmentCode = fragmentSource.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(
        vertexShader,
        1,
        &vertexCode,
        nullptr
    );

    glCompileShader(vertexShader);

    GLint success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(
            vertexShader,
            512,
            nullptr,
            infoLog
        );

        std::cerr << "Erro no Vertex Shader:\n"
            << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentCode,
        nullptr
    );

    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(
            fragmentShader,
            512,
            nullptr,
            infoLog
        );

        std::cerr << "Erro no Fragment Shader:\n"
            << infoLog << std::endl;
    }

    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(
            shaderProgram,
            512,
            nullptr,
            infoLog
        );

        std::cerr << "Erro ao linkar shader:\n"
            << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void mouse_button_callback(
    GLFWwindow* window,
    int button,
    int action,
    int mods
)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT &&
        action == GLFW_PRESS)
    {
        double xpos;
        double ypos;

        glfwGetCursorPos(
            window,
            &xpos,
            &ypos
        );

        float x = static_cast<float>(xpos);
        float y = static_cast<float>(ypos);

        int cor = triangulos % 6;

        vertices.push_back(x);
        vertices.push_back(y);

        vertices.push_back(cores[cor][0]);
        vertices.push_back(cores[cor][1]);
        vertices.push_back(cores[cor][2]);

        int quantidadeVertices =
            static_cast<int>(vertices.size() / 5);

        std::cout
            << "Clique: "
            << x << ", "
            << y
            << " | Vertices: "
            << quantidadeVertices
            << std::endl;

        if (quantidadeVertices % 3 == 0)
        {
            triangulos++;

            std::cout
                << "Triangulo criado: "
                << triangulos
                << std::endl;
        }
    }
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Erro ao inicializar GLFW\n";
        return -1;
    }

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MAJOR,
        3
    );

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MINOR,
        3
    );

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE
    );

    GLFWwindow* window =
        glfwCreateWindow(
            800,
            600,
            "Lista 2 - Questao 6",
            nullptr,
            nullptr
        );

    if (!window)
    {
        std::cerr << "Erro ao criar janela\n";

        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(
        (GLADloadfunc)glfwGetProcAddress
    ))
    {
        std::cerr << "Erro ao inicializar GLAD\n";

        glfwDestroyWindow(window);
        glfwTerminate();

        return -1;
    }

    glfwSetMouseButtonCallback(
        window,
        mouse_button_callback
    );

    GLuint VAO;
    GLuint VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO
    );

    glBufferData(
        GL_ARRAY_BUFFER,
        0,
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(2 * sizeof(float))
    );

    glEnableVertexAttribArray(1);

    GLuint shaderProgram =
        createShaderProgram();

    glm::mat4 projection =
        glm::ortho(
            0.0f,
            800.0f,
            600.0f,
            0.0f
        );

    GLuint projectionLoc =
        glGetUniformLocation(
            shaderProgram,
            "projection"
        );

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(
            0.3f,
            0.3f,
            0.3f,
            1.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glUniformMatrix4fv(
            projectionLoc,
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );

        glBindVertexArray(VAO);

        glBindBuffer(
            GL_ARRAY_BUFFER,
            VBO
        );

        if (!vertices.empty())
        {
            glBufferData(
                GL_ARRAY_BUFFER,
                vertices.size() * sizeof(float),
                vertices.data(),
                GL_DYNAMIC_DRAW
            );

            glDrawArrays(
                GL_TRIANGLES,
                0,
                static_cast<GLsizei>(
                    vertices.size() / 5
                    )
            );
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(
        1,
        &VAO
    );

    glDeleteBuffers(
        1,
        &VBO
    );

    glDeleteProgram(
        shaderProgram
    );

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}