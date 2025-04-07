#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#define PI glm::pi<float>()

GLuint shader_programme, vao;
glm::mat4 projectionMatrix, viewMatrix, modelMatrix;

int nrFaces = 6;
int nrVerticesPerFace = 6;
int nrVertices = nrFaces * nrVerticesPerFace;

glm::vec3 lightPos(0, 1, 5);
glm::vec3 viewPos(2, 3, 6);

float L = 0.5f;

//trebuie adaugate coordonatele vectorilor normali pentru fiecare vertex, pe langa pozitie
float cube[] = {
    // face x positive
    L, -L, -L,  1, 0, 0,
    L, -L, L,    1, 0, 0,
    L, L, L,     1, 0, 0,

    L, -L, -L,   1, 0, 0,
    L, L, -L,    1, 0, 0,
    L, L, L,     1, 0, 0,

    // face x negative
    -L, -L, -L,  -1, 0, 0,
    -L, L, -L,   -1, 0, 0,
    -L, L, L,    -1, 0, 0,

    -L, -L, -L,  -1, 0, 0,
    -L, -L, L,   -1, 0, 0,
    -L, L, L,    -1, 0, 0,

    // face y positive
    L, L, -L,    0, 1, 0,
    L, L, L,     0, 1, 0,
    -L, L, L,    0, 1, 0,

    L, L, -L,    0, 1, 0,
    -L, L, -L,   0, 1, 0,
    -L, L, L,    0, 1, 0,

    // face y negative
    L, -L, -L,   0, -1, 0,
    L, -L, L,    0, -1, 0,
    -L, -L, L,   0, -1, 0,

    L, -L, -L,   0, -1, 0,
    -L, -L, -L,  0, -1, 0,
    -L, -L, L,   0, -1, 0,

    // face z positive
    L, -L, L,    0, 0, 1,
    L, L, L,     0, 0, 1,
    -L, L, L,    0, 0, 1,

    L, -L, L,    0, 0, 1,
    -L, -L, L,   0, 0, 1,
    -L, L, L,    0, 0, 1,

    // face z negative
    L, -L, -L,   0, 0, -1,
    L, L, -L,    0, 0, -1,
    -L, L, -L,   0, 0, -1,

    L, -L, -L,   0, 0, -1,
    -L, -L, -L,  0, 0, -1,
    -L, L, -L,   0, 0, -1
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

void printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

void printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

float rotAngle = 0;
float rotAngleInc = PI / 64;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader_programme);

	modelMatrix = glm::mat4(1.0f);

	glBindVertexArray(vao);
	
	GLuint lightPosLoc = glGetUniformLocation(shader_programme, "lightPos");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

	GLuint viewPosLoc = glGetUniformLocation(shader_programme, "viewPos");
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));

	modelMatrix *= glm::rotate(rotAngle, glm::vec3(0, 1, 0));
	GLuint modelMatrixLoc = glGetUniformLocation(shader_programme, "mvpMatrix");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * modelMatrix));
	
	// se determina matricea ce realizeaza corectia normalelor. Ea se trimite catre vertex shader la fel cum s-a procedat si cu mvpMatrix 
    GLuint normalMatrixLoc = glGetUniformLocation(shader_programme, "normalMatrix");
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));
    glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	
	glDrawArrays(GL_TRIANGLES, 0, nrVertices);

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
	glEnable(GL_DEPTH_TEST);

	glewInit();

    GLuint vbo = 1;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	//normalele se trimit la vertex shader ca atributul cu index 1, asa cum s-a procedat in cazul culorilor la Lab 7
	//atentie la parametrii stride si offset ai functiei glVertexAttribPointer
	
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

	printShaderInfoLog(shader_programme);
	printProgramInfoLog(shader_programme);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	projectionMatrix = glm::perspective(PI / 6, (float)w / h, 0.1f, 100.0f);
	/*
	viewMatrix este matricea transformarii de observare. Parametrii functiei
	lookAt sunt trei vectori ce reprezinta, in ordine:
	- pozitia observatorului
	- punctul catre care priveste observatorul
	- directia dupa care este orientat observatorul
	*/
	viewMatrix = glm::lookAt(viewPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		rotAngle += rotAngleInc;
		break;
	case 's':
		rotAngle -= rotAngleInc;
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_SINGLE);
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
