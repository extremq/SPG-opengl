#include <iostream>
#include <fstream>
#include <string>

#include <stdio.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// varfurile triunghiului
float points[] = {
    0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f,
};

GLuint shader_programme, vao;
glm::mat4 model;

std::string textFileRead(char *fn) {
    std::ifstream ifile(fn);
    std::string filetext;
    while (ifile.good()) {
        std::string line;
        std::getline(ifile, line);
        filetext.append(line + "\n");
    }
    return filetext;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_programme);

    // First triangle: Rotate by 45 degrees
    float rotationAngle = glm::radians(-45.0f);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationAngle,
                                            glm::vec3(0.0f, 0.0f, 1.0f));

    // First triangle: Rotate by 45 degrees
    float rotationAngle1 = glm::radians(45.0f);
    glm::mat4 rotationMatrix1 = glm::rotate(glm::mat4(1.0f), rotationAngle1,
                                            glm::vec3(0.0f, 0.0f, 1.0f));

    GLuint matrixID = glGetUniformLocation(shader_programme, "modelMatrix");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(rotationMatrix1));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Second triangle: Rotate by -45 degrees
    float rotationAngle2 = glm::radians(-45.0f);
    glm::mat4 rotationMatrix2 = glm::rotate(glm::mat4(1.0f), rotationAngle2,
                                            glm::vec3(0.0f, 0.0f, 1.0f));

    glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(rotationMatrix2));
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glFlush();
}

void init() {
    // get version info
    const GLubyte *renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte *version = glGetString(GL_VERSION);   // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    glClearColor(1, 1, 1, 0);

    glewInit();

    GLuint vbo = 1;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    std::string vertextfile = "vertex.vert";
    std::string fragmentfile = "fragment.frag";
    std::string vstext = textFileRead(vertextfile.data());
    std::string fstext = textFileRead(fragmentfile.data());
    const char *vertex_shader = vstext.c_str();
    const char *fragment_shader = fstext.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);
}

int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(512, 512);
    glutCreateWindow("SPG");

    init();

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
