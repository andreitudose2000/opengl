#include <windows.h>  
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h> 
#include <GL/freeglut.h> 
#include "loadShaders.h"
#include "glm/glm/glm.hpp"  
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/string_cast.hpp"
#include "glm/glm/gtc/quaternion.hpp"
#include "glm/glm/gtx/quaternion.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "SOIL.h"
#include "proiect-tetris.h"
using namespace std;

// identificatori 
GLuint
VaoId,
VboId,
EboId,
ProgramId,
viewLocation,
projLocation,
codColLocation,
codUmbraLocation,
matrTranslLocation,
matrUmbraLocation;

// variabile
int codCol, codUmbra;
const float PI = 3.14159;

// elemente pentru matricea de vizualizare
float cameraAngle = 0.f;
float cameraDistance = 300.f;
float cameraHeight = 50.0;

// lumina
GLfloat xL = 50.f, yL = 200.f, zL = 100.f;

// elemente pentru matricea de proiectie
float width = 800, height = 600, xwmin = -200.f, xwmax = 200, ywmin = -200, ywmax = 200, znear = 100, zfar = -100, fov = 30;

// vectori
glm::vec3 eye, center, up;

// matrice utilizate
glm::mat4 view, projection;

glm::mat4 resizeMatrix, matrTransl, myMatrix;

glm::quat quaternion;

long frame = 0;
bool showBorder = false;

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '+':
		cameraDistance -= 10;
		break;
	case '-':
		cameraDistance += 10;
		break;
	case 'w':
		zL += 5;
		break;
	case 's':
		zL -= 5;
		break;
	case 'd':
		xL += 5;
		break;
	case 'a':
		xL -= 5;
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
		cameraAngle -= 0.05;
		if (cameraAngle < 0.f) cameraAngle = 2 * PI;
		cout << glm::to_string(view) << endl;
		break;
	case GLUT_KEY_RIGHT:
		cameraAngle += 0.05;
		if (cameraAngle > 2 * PI) cameraAngle = 0.f;
		cout << glm::to_string(view) << endl;
		break;
	case GLUT_KEY_UP:
		cameraHeight += 20;
		break;
	case GLUT_KEY_DOWN:
		cameraHeight -= 20;
		break;
	}
}

enum {
	Restart, ResetCamera, ShowBorder
};

void menu(int selection)
{
	switch (selection) 
	{
		case Restart:
			frame = 0.f;
			break;

		case ResetCamera:
			cameraAngle = 0.f;
			cameraDistance = 300.f;
			cameraHeight = 50.f;
			break;

		case ShowBorder:
			showBorder = !showBorder;
			break;
	}
}


GLfloat gameHeight = 160.f;

GLfloat matrUmbra[4][4];
float A = 0.f, B = 1.f, C = 0.f, D = - (float)gameHeight + 0.5;

void CreateVBO(void)
{

	GLfloat Vertices[] =
	{
		// borders - 0
		-60.5f,      0.f,		 0.f, 1.f,	0.f, 0.f, 0.f,		0.f, 0.f,
		 60.5f,      0.f,		 0.f, 1.f,	0.f, 0.f, 0.f,		0.f, 1.f,
		 60.5f,    gameHeight,   0.f, 1.f,	0.f, 0.f, 0.f,		1.f, 1.f,
		-60.5f,    gameHeight,   0.f, 1.f,	0.f, 0.f, 0.f,		1.f, 0.f,
		-60.5f,      0.f,		20.f, 1.f,	0.f, 0.f, 0.f,		0.f, 0.f,
		 60.5f,      0.f,		20.f, 1.f,	0.f, 0.f, 0.f,		0.f, 1.f,
		 60.5f,    gameHeight,	20.f, 1.f,	0.f, 0.f, 0.f,		1.f, 1.f,
		-60.5f,    gameHeight,	20.f, 1.f,	0.f, 0.f, 0.f,		1.f, 0.f,

		// block verde
		-40.f,	0.f,  0.f, 1.f,   0.f, 1.f, 0.f,  0.f, 0.f,
		-20.f,  0.f,  0.f, 1.f,   0.f, 1.f, 0.f,  0.f, 1.f,
		-20.f, 20.f,  0.f, 1.f,   0.f, 1.f, 0.f,  1.f, 1.f,
		-40.f, 20.f,  0.f, 1.f,   0.f, 1.f, 0.f,  1.f, 0.f,
		-40.f,  0.f, 20.f, 1.f,   0.f, 1.f, 0.f,  0.f, 0.f,
		-20.f,  0.f, 20.f, 1.f,   0.f, 1.f, 0.f,  0.f, 1.f,
		-20.f, 20.f, 20.f, 1.f,   0.f, 1.f, 0.f,  1.f, 1.f,
		-40.f, 20.f, 20.f, 1.f,   0.f, 1.f, 0.f,  1.f, 0.f,


		// podea
		-140.f, gameHeight + 0.5f,   -100.f, 1.f,  1.f, 0.f, 0.f,  0.f, 0.f,
		 140.f, gameHeight + 0.5f,  -100.f, 1.f,  1.f, 0.f, 0.f,  1.f, 0.f,
		 140.f, gameHeight + 0.5f,   120.f, 1.f,  1.f, 0.f, 0.f,  1.f, 1.f,
		-140.f, gameHeight + 0.5f,   120.f, 1.f,  1.f, 0.f, 0.f,  0.f, 1.f,


		// lumina
		- xL - 5.f, -yL - 5.f, - zL - 5.f, 1.f,		0.0f, 0.0f, 0.0f,	0.f, 0.f,
		- xL + 5.f, -yL - 5.f, - zL - 5.f, 1.f,		0.0f, 0.0f, 0.0f,	0.f, 0.f,
		- xL + 5.f, -yL + 5.f, - zL - 5.f, 1.f,		0.0f, 0.0f, 0.0f,	0.f, 0.f,
		- xL - 5.f, -yL + 5.f, - zL - 5.f, 1.f,		0.0f, 0.0f, 0.0f,	0.f, 0.f,
		- xL - 5.f, -yL - 5.f, - zL + 5.f, 1.f,		0.0f, 0.0f, 0.0f,	0.f, 0.f,
		- xL + 5.f, -yL - 5.f, - zL + 5.f, 1.f,		0.0f, 0.0f, 0.0f,	0.f, 0.f,
		- xL + 5.f, -yL + 5.f, - zL + 5.f, 1.f,		0.0f, 0.0f, 0.0f,	0.f, 0.f,
		- xL - 5.f, -yL + 5.f, - zL + 5.f, 1.f,		0.0f, 0.0f, 0.0f,	0.f, 0.f,

		// skybox
		-200.f, -200.f,  200.f,		 0.0f, 0.0f, 0.0f,		0.f, 0.f,
		 200.f, -200.f,  200.f,		 0.0f, 0.0f, 0.0f,		0.f, 0.f,
		 200.f, -200.f, -200.f,		 0.0f, 0.0f, 0.0f,		0.f, 0.f,
		 200.f, -200.f, -200.f,		 0.0f, 0.0f, 0.0f,		0.f, 0.f,
		-200.f, -200.f, -200.f,		 0.0f, 0.0f, 0.0f,		0.f, 0.f,
		-200.f,  200.f,  200.f,		 0.0f, 0.0f, 0.0f,		0.f, 0.f,
		 200.f,  200.f,  200.f,		 0.0f, 0.0f, 0.0f,		0.f, 0.f,
		 200.f,  200.f, -200.f,		 0.0f, 0.0f, 0.0f,		0.f, 0.f,
		-200.f,  200.f, -200.f,		 0.0f, 0.0f, 0.0f,		0.f, 0.f,

	};

	GLubyte Indices[] =
	{
		// borders - muchii
		0, 1, 2, 3, // fata din fata
		4, 5, 6, 7, // fata din spate
		0, 4, // linie jos stanga 
		1, 5, // linie jos dreapta
		2, 6, // linie sus dreapta
		3, 7, // linie sus stanga


		// block - fete
		8, 9, 10, 11, // fata din fata
		12, 13, 14, 15, // fata din spate
		8, 11, 15, 12, // fata din stanga
		9, 10, 14, 13, // fata din dreapta
		8, 9, 13, 12, // fata de jos
		11, 10, 14, 15, // fata de sus

		// block - muchii
		8, 9, 10, 11, // fata din fata
		12, 13, 14, 15, // fata din spate
		8, 12, // linie jos stanga 
		9, 13, // linie jos dreapta
		10, 14, // linie sus dreapta
		11, 15, // linie sus stanga


		// podea
		16, 17, 18, 19,

		0, 3, 7, 4, // latura stanga
		1, 2, 6, 5, // latura dreapta
		0, 1, 5, 4, // latura sus

		// sursa lumina
		20, 21, 22, 23, // latura fata
		24, 25, 26, 27, // latura spate
		20, 21, 25, 24, // latura jos
		
		// lumina
		20, 21, 22, 23, // fata din fata
		24, 25, 26, 27, // fata din spate
		20, 23, 27, 24, // fata din stanga
		21, 22, 26, 25, // fata din dreapta
		20, 21, 25, 24, // fata de jos
		23, 22, 26, 27, // fata de sus

		// skybox
		1, 2, 6, // dreapta
		6, 5, 1,
		0, 4, 7, // stanga
		7, 3, 0,
		4, 5, 6, // sus
		6, 7, 4,
		0, 3, 2, // jos
		2, 1, 0,
		0, 1, 5, // spate
		5, 4, 0,
		3, 7, 6, // fata
		6, 2, 3
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
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2); // atributul 2 = textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));

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

GLuint textures[3];
void LoadTextures()
{
	glGenTextures(3, &textures[0]);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int pieceWidth, pieceHeight;
	unsigned char* pieceImage = SOIL_load_image("tetris-black-white.png", &pieceWidth, &pieceHeight, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pieceWidth, pieceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pieceImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(pieceImage);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int grassWidth, grassHeight;
	unsigned char* grassImage = SOIL_load_image("grass.png", &grassWidth, &grassHeight, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, grassWidth, grassHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, grassImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(grassImage);


	string facesCubemap[6] =
	{
		"skybox-dreapta.jpg",
		"skybox-stanga.jpg",
		"skybox-sus.jpg",
		"skybox-jos.jpg",
		"skybox-fata.jpg",
		"skybox-spate.jpg",

	};

	for (int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* image = SOIL_load_image(facesCubemap[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		if (image)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);
		}
		else
		{
			cout << "eroare " << facesCubemap[i] << endl;
			SOIL_free_image_data(image);
		}
	}

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures[2]);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindVertexArray(0);
}

void CreateShaders(void)
{
	ProgramId = LoadShaders("tetris.vert", "tetris.frag");
	glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

void Initialize(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_CULL_FACE);

	// Creare VBO+shader
	CreateVBO();
	CreateShaders();

	// Locatii ptr shader
	viewLocation = glGetUniformLocation(ProgramId, "viewShader");
	projLocation = glGetUniformLocation(ProgramId, "projectionShader");
	codColLocation = glGetUniformLocation(ProgramId, "codCol");
	codUmbraLocation = glGetUniformLocation(ProgramId, "codUmbra");

	LoadTextures();

	glUniform1i(glGetUniformLocation(ProgramId, "pieceTexture"), 0);
	glUniform1i(glGetUniformLocation(ProgramId, "grassTexture"), 1);
	glUniform1i(glGetUniformLocation(ProgramId, "skybox"), 2);
}

void DesenareBorder()
{
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glLineWidth(1.0f);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)4);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_BYTE, (void*)8);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_BYTE, (void*)10);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_BYTE, (void*)12);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_BYTE, (void*)14);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)60);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)64);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)68);
}

void DesenareFete(int color)
{
	codUmbra = 0;
	glUniform1i(codUmbraLocation, codUmbra);
	codCol = color;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)16);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)20);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)24);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)28);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)32);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)36);

	codUmbra = 1;
	glUniform1i(codUmbraLocation, codUmbra);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)16);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)20);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)24);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)28);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)32);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)36);
}

void DesenareMuchii(void)
{
	codUmbra = 0;
	glUniform1i(codUmbraLocation, codUmbra);
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glLineWidth(2.0f);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)40);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)44);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_BYTE, (void*)48);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_BYTE, (void*)50);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_BYTE, (void*)52);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_BYTE, (void*)54);
}

void DesenarePatrat(void)
{
	codCol = 11;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)56);
}

void DesenareLumina(void)
{
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)72);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)76);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)80);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)84);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)88);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (void*)92);
}

void DesenareSkybox(void)
{

	glDepthFunc(GL_LEQUAL);
	codUmbra = 0;
	glUniform1i(codUmbraLocation, codUmbra);
	codCol = 50;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)96);
	glDepthFunc(GL_LESS);
}

int CalculareY(long frame, int ordine)
{
	return 20 * ((int)frame / 20) - (ordine - 1) * 160;
}


void DesenareZshape(int xPiesa, int yPiesa, int ordine)
{
	GLfloat x = -(float)xPiesa * 20 + 80.f;

	GLfloat y = CalculareY(frame, ordine);

	GLfloat minY = yPiesa * 20.f;

	if (y > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x, min(y, gameHeight - 20.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(3);
		DesenareMuchii();
	}

	if (y > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x + 20.f, min(y, gameHeight - 20.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(3);
		DesenareMuchii();
	}

	if (y - 20.f > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x + 20.f, min(y - 20.f, gameHeight - 40.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(3);
		DesenareMuchii();
	}

	if (y - 20.f > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x + 40.f, min(y - 20.f, gameHeight - 40.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(3);
		DesenareMuchii();
	}
}

void DesenareTshape(int xPiesa, int yPiesa, int ordine)
{
	GLfloat x = -(float)xPiesa * 20 + 80.f;

	GLfloat y = CalculareY(frame, ordine);

	GLfloat minY = yPiesa * 20.f;

	if (y > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x, min(y, gameHeight - 20.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(5);
		DesenareMuchii();
	}

	if (y - 20.f > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x, min(y - 20.f, gameHeight - 40.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(5);
		DesenareMuchii();
	}

	if (y - 20.f > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x - 20.f, min(y - 20.f, gameHeight - 40.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(5);
		DesenareMuchii();
	}

	if (y - 40.f> -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x, min(y - 40.f, gameHeight - 60.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(5);
		DesenareMuchii();
	}
}

void DesenareOshape(int xPiesa, int yPiesa, int ordine)
{
	GLfloat x = -(float)xPiesa * 20 + 80.f;

	GLfloat y = CalculareY(frame, ordine);

	GLfloat minY = yPiesa * 20.f;

	if (y > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x, min(y, gameHeight - 20.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(6);
		DesenareMuchii();
	}

	if (y - 20.f > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x, min(y - 20.f, gameHeight - 40.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(6);
		DesenareMuchii();
	}
	
	if (y > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x - 20.f, min(y, gameHeight - 20.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(6);
		DesenareMuchii();
	}

	if (y - 20.f > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x - 20.f, min(y - 20.f, gameHeight - 40.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(6);
		DesenareMuchii();
	}
}

void DesenareLshape(int xPiesa, int yPiesa, int ordine)
{
	GLfloat x = -(float)xPiesa * 20 + 80.f;

	GLfloat y = CalculareY(frame, ordine);

	GLfloat minY = yPiesa * 20.f;
	if (y > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x, min(y, gameHeight - 20.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(7);
		DesenareMuchii();
	}

	if (y - 20.f > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x, min(y - 20.f, gameHeight - 40.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(7);
		DesenareMuchii();
	}

	if (y - 40.f > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x, min(y - 40.f, gameHeight - 60.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(7);
		DesenareMuchii();
	}

	if (y - 40.f > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x - 20.f, min(y - 40.f, gameHeight - 60.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(7);
		DesenareMuchii();
	}
}

void DesenareIshape(int xPiesa, int yPiesa, int ordine)
{
	GLfloat x = -(float)xPiesa * 20 + 80.f;

	GLfloat y = CalculareY(frame, ordine);

	GLfloat minY = yPiesa * 20.f;

	if (y > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x, min(y, gameHeight - 20.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(8);
		DesenareMuchii();
	}

	if (y - 20.f > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x, min(y - 20.f, gameHeight - 40.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(8);
		DesenareMuchii();
	}

	if (y - 40.f > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x, min(y - 40.f, gameHeight - 60.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(8);
		DesenareMuchii();
	}

	if (y - 60.f > -20.f)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x, min(y - 60.f, gameHeight - 80.f - minY), 0.f));
		matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		DesenareFete(8);
		DesenareMuchii();
	}
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);

	CreateVBO();
	glBindVertexArray(VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);

	glm::vec3 eulerAngles(0.f, cameraAngle, 0.f);
	quaternion = glm::quat(eulerAngles);
	view = glm::toMat4(quaternion);
	view[3][1] = -cameraHeight;
	view[3][2] = -cameraDistance;
	
	/*eye = glm::vec3(cameraDistance * sin(cameraAngle), cameraHeight, cameraDistance * cos(cameraAngle));
	center = glm::vec3(0.f, cameraHeight, 0.f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(eye, center, up);*/

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projection = glm::perspective(fov, GLfloat(width) / GLfloat(height), znear, zfar);

	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	
	matrUmbra[0][0] = B * yL + C * zL + D;	matrUmbra[1][0] = -B * xL;				matrUmbra[2][0] = -C * xL;				matrUmbra[3][0] = -D * xL;
	matrUmbra[0][1] = -A * yL;				matrUmbra[1][1] = A * xL + C * zL + D;	matrUmbra[2][1] = -C * yL;				matrUmbra[3][1] = -D * yL;
	matrUmbra[0][2] = -A * zL;				matrUmbra[1][2] = -B * zL;				matrUmbra[2][2] = A * xL + B * yL + D;	matrUmbra[3][2] = -D * zL;
	matrUmbra[0][3] = -A;					matrUmbra[1][3] = -B;					matrUmbra[2][3] = -C;					matrUmbra[3][3] = A * xL + B * yL + C * zL;
	matrUmbraLocation = glGetUniformLocation(ProgramId, "matrUmbra");
	glUniformMatrix4fv(matrUmbraLocation, 1, GL_FALSE, &matrUmbra[0][0]);

	if (showBorder) { DesenareBorder(); }

	codUmbra = 0;
	glUniform1i(codUmbraLocation, codUmbra);

	DesenarePatrat();

	DesenareLumina();

	DesenareIshape(5, 0, 1);
	DesenareZshape(4, 0, 2);
	DesenareTshape(0, 0, 3);
	DesenareOshape(2, 2, 4);
	DesenareLshape(1, 2, 5);

	DesenareSkybox();

	glutSwapBuffers();
	glFlush();

	if (frame < 10000) { frame += 1; }
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
	glutCreateWindow("Animatie Tetris");
	glewInit();
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCreateMenu(menu);
	glutAddMenuEntry("Restart", Restart);
	glutAddMenuEntry("Reset camera", ResetCamera);
	glutAddMenuEntry("Arata Border", ShowBorder);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}
