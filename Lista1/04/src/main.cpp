#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

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

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentCode,
        nullptr
    );

    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void configurarVAO(
    GLuint& VAO,
    GLuint& VBO,
    float* vertices,
    size_t tamanho
)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        tamanho,
        vertices,
        GL_STATIC_DRAW
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

    glBindVertexArray(0);
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Erro ao inicializar GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        800,
        600,
        "Questao 4 - Desenho",
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

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cerr << "Erro ao inicializar GLAD\n";

        glfwDestroyWindow(window);
        glfwTerminate();

        return -1;
    }

    std::cout
        << "OpenGL: "
        << glGetString(GL_VERSION)
        << std::endl;

    float casa[] =
    {
        -0.6f, -0.5f,   0.8f, 0.5f, 0.2f,
         0.6f, -0.5f,   0.8f, 0.5f, 0.2f,
         0.6f,  0.2f,   0.8f, 0.5f, 0.2f,

         -0.6f, -0.5f,   0.8f, 0.5f, 0.2f,
          0.6f,  0.2f,   0.8f, 0.5f, 0.2f,
         -0.6f,  0.2f,   0.8f, 0.5f, 0.2f
    };

    float telhado[] =
    {
        -0.7f, 0.2f,    0.8f, 0.1f, 0.1f,
         0.7f, 0.2f,    0.8f, 0.1f, 0.1f,
         0.0f, 0.8f,    0.8f, 0.1f, 0.1f
    };

    float janela[] =
    {
        -0.4f, -0.1f,   0.1f, 0.6f, 1.0f,
         0.0f, -0.1f,   0.1f, 0.6f, 1.0f,
         0.0f,  0.2f,   0.1f, 0.6f, 1.0f,
        -0.4f,  0.2f,   0.1f, 0.6f, 1.0f
    };

    float porta[] =
    {
         0.15f, -0.5f,   0.3f, 0.15f, 0.05f,
         0.45f, -0.5f,   0.3f, 0.15f, 0.05f,
         0.45f,  0.05f,   0.3f, 0.15f, 0.05f,
         0.15f,  0.05f,   0.3f, 0.15f, 0.05f
    };

    float chao[] =
    {
        -0.9f, -0.5f,    0.2f, 0.8f, 0.2f,
         0.9f, -0.5f,    0.2f, 0.8f, 0.2f
    };

    GLuint VAO_Casa;
    GLuint VBO_Casa;

    GLuint VAO_Telhado;
    GLuint VBO_Telhado;

    GLuint VAO_Janela;
    GLuint VBO_Janela;

    GLuint VAO_Porta;
    GLuint VBO_Porta;

    GLuint VAO_Chao;
    GLuint VBO_Chao;


    configurarVAO(
        VAO_Casa,
        VBO_Casa,
        casa,
        sizeof(casa)
    );


    configurarVAO(
        VAO_Telhado,
        VBO_Telhado,
        telhado,
        sizeof(telhado)
    );


    configurarVAO(
        VAO_Janela,
        VBO_Janela,
        janela,
        sizeof(janela)
    );


    configurarVAO(
        VAO_Porta,
        VBO_Porta,
        porta,
        sizeof(porta)
    );


    configurarVAO(
        VAO_Chao,
        VBO_Chao,
        chao,
        sizeof(chao)
    );

    GLuint shaderProgram = createShaderProgram();


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

        glBindVertexArray(VAO_Casa);

        glDrawArrays(
            GL_TRIANGLES,
            0,
            6
        );

        glBindVertexArray(VAO_Telhado);

        glDrawArrays(
            GL_TRIANGLES,
            0,
            3
        );

        glBindVertexArray(VAO_Janela);

        glDrawArrays(
            GL_LINE_LOOP,
            0,
            4
        );

        glBindVertexArray(VAO_Porta);

        glDrawArrays(
            GL_LINE_LOOP,
            0,
            4
        );

        glBindVertexArray(VAO_Chao);

        glDrawArrays(
            GL_LINES,
            0,
            2
        );

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO_Casa);
    glDeleteBuffers(1, &VBO_Casa);

    glDeleteVertexArrays(1, &VAO_Telhado);
    glDeleteBuffers(1, &VBO_Telhado);

    glDeleteVertexArrays(1, &VAO_Janela);
    glDeleteBuffers(1, &VBO_Janela);

    glDeleteVertexArrays(1, &VAO_Porta);
    glDeleteBuffers(1, &VBO_Porta);

    glDeleteVertexArrays(1, &VAO_Chao);
    glDeleteBuffers(1, &VBO_Chao);

    glDeleteProgram(shaderProgram);


    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}