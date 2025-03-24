#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#include <stdio.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#define PI glm::pi<float>()

GLuint shader_programme, vao;
glm::mat4 projectionMatrix, viewMatrix;

std::stack <glm::mat4> modelStack;

float segment1 = 2; //lungimea bratului
float segment1Angle = 0; //unghiul de rotatie al bratului
float segment2 = 3; //lungimea antebratului
float segment2Angle = 0; //unghiul de rotatie al antebratului
float segment3 = 1; //lungimea degetului
float segment3Angle = 0; //unghiul de rotatie al degetului

float points[] = {
	-0.5f,  -0.5f,  -0.5f,
	-0.5f, 0.5f,  -0.5f,
	-0.5f, 0.5f,  -0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f, -0.5f,  -0.5f,
	0.5f, -0.5f,  -0.5f,
	- 0.5f,  -0.5f,  -0.5f,

	-0.5f,  -0.5f,  0.5f,
	-0.5f, 0.5f,  0.5f,
	-0.5f, 0.5f,  0.5f,
	0.5f,  0.5f, 0.5f,
	0.5f,  0.5f, 0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	-0.5f,  -0.5f,  0.5f,

	-0.5f,  -0.5f,  0.5f,
	-0.5f,  -0.5f,  -0.5f,
	-0.5f, 0.5f,  0.5f,
	-0.5f, 0.5f,  -0.5f,
	0.5f,  0.5f, 0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  -0.5f,

	0.0f, 0.0f, 0.0f,
	16.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 8.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 24.0f,
};

std::string textFileRead(char *fn) 
{
	std::ifstream ifile(fn);
	std::string filetext;
	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		filetext.append(line + "\n");
	}
	return filetext;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shader_programme);

	float rotationAngle = PI / 2;
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	glBindVertexArray(vao);
	GLuint matrixID = glGetUniformLocation(shader_programme, "modelViewProjectionMatrix");

	////desenare axe coordonate
	
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * modelMatrix));
	glDrawArrays(GL_LINES, 24, 6);

	//brat
	modelMatrix *= glm::rotate(segment1Angle, glm::vec3(0, 0, 1));
	modelMatrix *= glm::translate(glm::vec3(segment1 / 2.0f, 0, 0));
	
	modelStack.push(modelMatrix);
		modelMatrix *= glm::scale(glm::vec3(segment1, 0.5, 0.5));

		glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * modelMatrix));
		glDrawArrays(GL_LINES, 0, 24);
	modelMatrix = modelStack.top();
	modelStack.pop();

	// antebrat
	modelMatrix *= glm::translate(glm::vec3(segment1 / 2.0f, 0, 0));
	modelMatrix *= glm::rotate(segment2Angle, glm::vec3(0, 0, 1));
	modelMatrix *= glm::translate(glm::vec3(segment2 / 2.0f, 0, 0));

	modelStack.push(modelMatrix);
		modelMatrix *= glm::scale(glm::vec3(segment2, 0.5, 0.5));

		glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * modelMatrix));
		glDrawArrays(GL_LINES, 0, 24);
	modelMatrix = modelStack.top();
	modelStack.pop();

    // deget
	modelMatrix *= glm::translate(glm::vec3(segment2 / 2.0f, 0, 0));
	modelMatrix *= glm::rotate(segment3Angle, glm::vec3(0, 0, 1));
	modelMatrix *= glm::translate(glm::vec3(segment3 / 2.0f, 0, 0));

	modelStack.push(modelMatrix);
		modelMatrix *= glm::scale(glm::vec3(segment3, 0.5, 0.5));

		glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * modelMatrix));
		glDrawArrays(GL_LINES, 0, 24);
	modelMatrix = modelStack.top();
	modelStack.pop();

	glFlush();
}

void init()
{
	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	glClearColor(1, 1, 1, 0);

	glewInit();

	GLuint vbo = 1;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 90 * sizeof(float), points, GL_STATIC_DRAW);

	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	std::string vstext = textFileRead("vertex.vert");
	std::string fstext = textFileRead("fragment.frag");
	const char* vertex_shader = vstext.c_str();
	const char* fragment_shader = fstext.c_str();

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

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		segment1Angle += 0.1;
			break;
	case 'e':
		segment1Angle -= 0.1;
			break;
    case 's':
        segment2Angle += 0.1;
        break;
    case 'd':
        segment2Angle -= 0.1;
        break;
    case 'x':
        segment3Angle += 0.1;
        break;
    case 'c':
        segment3Angle -= 0.1;
        break;
	}
	glutPostRedisplay();
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	projectionMatrix = glm::perspective(PI / 3, (float)w / h, 0.1f, 100.0f);
	/*
	viewMatrix este matricea transformarii de observare. Parametrii functiei
	lookAt sunt trei vectori ce reprezinta, in ordine:
	- pozitia observatorului
	- punctul catre care priveste observatorul
	- directia dupa care este orientat observatorul
	*/
	viewMatrix = glm::lookAt(glm::vec3(0, 0, 10),	glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(700, 700);
	glutCreateWindow("SPG");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}
