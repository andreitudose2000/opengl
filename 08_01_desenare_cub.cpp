/*
- Folosirea indexarii pentru a trasa separata fetele si muchiile unui obiect 3D (cub)
- Rolul testului de adancime
*/
#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h
#include "loadShaders.h"
#include "glm/glm/glm.hpp"  
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
using namespace std;

// identificatori 
GLuint
VaoId,
VboId,
EboId,
ProgramId,
ProgramIdv,
ProgramIdf,
viewLocation,
projLocation,
codColLocation,
depthLocation,
rendermode,
l1, l2;
GLint objectColorLoc, lightColorLoc, lightPosLoc, viewPosLoc;


// variabile
int codCol;

// elemente pentru matricea de vizualizare
float Obsx = 0.0, Obsy = 0.0, Obsz = -300.f;
float Refx = 0.0f, Refy = 0.0f;
float Vx = 0.0;

// elemente pentru matricea de proiectie
float width = 800, height = 600, xwmin = -200.f, xwmax = 200, ywmin = -200, ywmax = 200, znear = 100, zfar = -100, fov = 30;

// vectori
glm::vec3 Obs, PctRef, Vert;

// matrice utilizate
glm::mat4 view, projection;

enum {
	Il_Frag, Il_Frag_Av, Il_Vert, Il_Vert_Av
};
void menu(int selection)
{
	rendermode = selection;
	glutPostRedisplay();
}
void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'l':
		Vx += 0.1;
		break;
	case 'r':
		Vx -= 0.1;
		break;
	case '+':
		Obsz += 10;
		break;
	case '-':
		Obsz -= 10;
		break;
	}
	if (key == 27)
		exit(0);
}
void processSpecialKeys(int key, int xx, int yy)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Obsx -= 20;
		break;
	case GLUT_KEY_RIGHT:
		Obsx += 20;
		break;
	case GLUT_KEY_UP:
		Obsy += 20;
		break;
	case GLUT_KEY_DOWN:
		Obsy -= 20;
		break;
	}
}
void CreateVBO(void)
{
	// varfurile 
	GLfloat Vertices[] =
	{
		// varfurile din planul z=-50  
		// coordonate                   // culori			
		-50.0f,  -50.0f, -50.0f, 1.0f,   0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, // 0
		-50.0f,  -50.0f,  50.0f, 1.0f,   0.0f, 0.9f, 0.0f,	0.0f, 0.0f, 1.0f, // 1
		-50.0f,   50.0f,  50.0f, 1.0f,   0.0f, 0.6f, 0.0f,	0.0f, 0.0f, 1.0f, // 2
		-50.0f,   50.0f, -50.0f, 1.0f,   0.0f, 0.2f, 0.0f,	1.0f, 1.0f, 0.0f, // 3
		// varfurile din planul z=+50  
		// coordonate                   // culori			
		  50.0f,  0.0f,  0.0f,  1.0f,    1.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	// 4
		-150.0f,  0.0f,  0.0f,  1.0f,    1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f	// 5

	};

	// indicii pentru varfuri
	GLubyte Indices[] =
	{
		1, 0, 2,   2, 0, 3,  //  Fata "de jos"
		0, 1, 4,    // Lateral 
		1, 2, 4,    // Lateral 
		2, 3, 4,    // Lateral 
		3, 0, 4,    // Lateral 
		0, 1, 5,    // Lateral 
		1, 2, 5,    // Lateral 
		2, 3, 5,    // Lateral 
		3, 0, 5,    // Lateral
		0, 4, // Muchie laterala
		1, 4, // Muchie laterala
		2, 4, // Muchie laterala
		3, 4, // Muchie laterala
		0, 5, // Muchie laterala
		1, 5, // Muchie laterala
		2, 5, // Muchie laterala
		3, 5, // Muchie laterala
		0, 1, 2, 3 // muchie patrat
	};

	// generare VAO/buffere
	glGenBuffers(1, &VboId); // atribute
	glGenBuffers(1, &EboId); // indici

	// legare+"incarcare" buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); // "copiere" in bufferul curent
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW); // "copiere" indici in bufferul curent

	// se activeaza lucrul cu atribute; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2); // atributul 1 = normale
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
}
void DestroyVBO(void)
{
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboId);
	glDeleteBuffers(1, &EboId);
}
void CreateShaders(void)
{
	ProgramId = LoadShaders("10_02f_Shader.vert", "10_02f_Shader.frag");
	glUseProgram(ProgramId);
}
void CreateShadersVertex(void)
{
	ProgramIdv = LoadShaders("10_02v_Shader.vert", "10_02v_Shader.frag");
	glUseProgram(ProgramIdv);
}
void CreateShadersFragment(void)
{
	ProgramIdf = LoadShaders("10_02f_Shader.vert", "10_02f_Shader.frag");
	glUseProgram(ProgramIdf);
}
void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
	glDeleteProgram(ProgramIdf);
	glDeleteProgram(ProgramIdv);
}
void Initialize(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului

	// Creare VBO+shader
	CreateVBO();

	CreateShadersFragment();
	objectColorLoc = glGetUniformLocation(ProgramIdf, "objectColor");
	lightColorLoc = glGetUniformLocation(ProgramIdf, "lightColor");
	lightPosLoc = glGetUniformLocation(ProgramIdf, "lightPos");
	viewPosLoc = glGetUniformLocation(ProgramIdf, "viewPos");
	viewLocation = glGetUniformLocation(ProgramIdf, "view");
	projLocation = glGetUniformLocation(ProgramIdf, "projection");
	CreateShadersVertex();
	objectColorLoc = glGetUniformLocation(ProgramIdv, "objectColor");
	lightColorLoc = glGetUniformLocation(ProgramIdv, "lightColor");
	lightPosLoc = glGetUniformLocation(ProgramIdv, "lightPos");
	viewPosLoc = glGetUniformLocation(ProgramIdv, "viewPos");
	viewLocation = glGetUniformLocation(ProgramIdv, "view");
	projLocation = glGetUniformLocation(ProgramIdv, "projection");
}
void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// CreateVBO(); // decomentati acest rand daca este cazul 
	glBindVertexArray(VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);

	// matricea de vizualizare
	Obs = glm::vec3(Obsx, Obsy, Obsz); // pozitia observatorului	
	Refx = Obsx; Refy = Obsy;
	PctRef = glm::vec3(Refx, Refy, 800.0f); // pozitia punctului de referinta
	Vert = glm::vec3(Vx, 1.0f, 0.0f); // verticala din planul de vizualizare 
	view = glm::lookAt(Obs, PctRef, Vert);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// matricea de proiectie, pot fi testate si alte variante
	projection = glm::perspective(fov, GLfloat(width) / GLfloat(height), znear, zfar);
	// projection = glm::ortho(xwmin, xwmax, ywmin, ywmax, znear, zfar);
	// projection = glm::frustum(xwmin, xwmax, ywmin, ywmax, 80.f, -80.f);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

	// Variabile uniforme pentru iluminare
	glUniform3f(objectColorLoc, 0.0f, 1.0f, 1.0f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0.f, -500.f, 0.f);
	glUniform3f(viewPosLoc, Obsx, Obsy, Obsz);

	// Fetele
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_BYTE, 0);
	// Muchiile
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glLineWidth(3.5);
	glDrawElements(GL_LINES, 16, GL_UNSIGNED_BYTE, (void*)(30));
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(46));

	glutSwapBuffers();
	glFlush();
}
void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1200, 900);
	glutCreateWindow("Desenarea unui cub folosind testul de adancime");
	glewInit();
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}

