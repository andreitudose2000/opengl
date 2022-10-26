#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "soil2-debug.lib")

/* INDEXARE
Elemente de noutate:
   - folosirea indecsilor: elemente asociate (matrice, buffer)
   - cele 4 functii de desenare (glDrawArrays, glDrawElements, glDrawElementsBaseVertex, glDrawArraysInstanced) */
#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL\glew.h> // glew apare inainte de freeglut
#include <GL\freeglut.h> // nu trebuie uitat freeglut.h
#include <algorithm>

#include "loadShaders.h"

#include "glm/glm/glm.hpp"  
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "SOIL.h"

using namespace std;

//////////////////////////////////////

float PI = 3.141592;

GLuint
VaoId,
VboId,
EboId,
ColorBufferId,
ProgramId,
myMatrixLocation,
codColLocation;

int codCol;

GLuint texture;


float width = 80.f, height = 60.f;
glm::mat4 myMatrix, resizeMatrix = glm::ortho(-width, width, -height, height), matrTransl, matrRot1, matrRot2, matrRot3, matrRot4, matrRot5;

long frame = -115;
int no_pieces = 9;
float speed = 1.5;

void CreateVBO(void)
{

    GLfloat Vertices[] = {

        // coordonate                 // culori				 // coordonate de texturare
         0.0f,  20.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // dreapta sus
         0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // dreapta jos 
       -10.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f,	 0.0f, 0.0f, // stanga jos
       -10.0f,  20.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f,	 0.0f, 1.0f  // stanga sus
    };

    GLuint Indices[] = {
      2, 0, 3,
      0, 1, 2
    };

    glGenVertexArrays(1, &VaoId);
    // se creeaza un buffer nou (atribute)
    glGenBuffers(1, &VboId);
    // se creeaza un buffer nou (indici)
    glGenBuffers(1, &EboId);

    // legarea VAO 
    glBindVertexArray(VaoId);

    // legarea buffer-ului "Array"
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    // punctele sunt "copiate" in bufferul curent
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    // legarea buffer-ului "Element" (indicii)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
    // indicii sunt "copiati" in bufferul curent
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    // se activeaza lucrul cu atribute; atributul 0 = pozitie
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);

    // se activeaza lucrul cu atribute; atributul 1 = culoare
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));

    // se activeaza lucrul cu atribute; atributul 2 = coordonate de texturare
    glEnableVertexAttribArray(2);
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
    glDeleteBuffers(1, &ColorBufferId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);

}

void LoadTexture(string path)
{

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height;
    unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

}
void CreateShaders(void)
{
    ProgramId = LoadShaders("05_02_Shader.vert", "05_02_Shader.frag");
    glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
    glDeleteProgram(ProgramId);
}

void Initialize(void)
{
    myMatrix = glm::mat4(1.0f);

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului
}
void RenderFunction(void)
{
    // Creare VBO
    glClear(GL_COLOR_BUFFER_BIT);

    if (frame >= no_pieces * 1 / speed * 90) return;
    CreateVBO();
    //LoadTexture("wood.jpg");
    LoadTexture("domino2.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    CreateShaders();

     
    
    // Transmitere variabile uniforme
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
    // Desenare

    glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);


    codCol = 1;
    codColLocation = glGetUniformLocation(ProgramId, "codCol");
    glUniform1i(codColLocation, codCol);

    for (int piece_index = 0; piece_index < no_pieces; piece_index++)
    {
        matrTransl = glm::translate(glm::vec3(15.0f * (piece_index - no_pieces / 2), 0.0f, 0.0f));

        GLfloat maxAngle = piece_index == no_pieces - 1 ? -90.f : piece_index == no_pieces - 2 ? -60.f : -50.f;

        matrRot1 = glm::rotate(glm::mat4(1.0f), glm::radians(min(max((-frame * speed + 18 * piece_index), maxAngle), 0.f)), glm::vec3(0.0, 0.0, 1.0));
        myMatrix = resizeMatrix * matrTransl * matrRot1;

        myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
        glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glutPostRedisplay();
    glutSwapBuffers();

    //glFlush();
    frame += 1;
    
}
void Cleanup(void)
{
    DestroyShaders();
    DestroyVBO();
}

int main(int argc, char* argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Domino");
    glewInit();
    Initialize();
    glutDisplayFunc(RenderFunction);
    glutCloseFunc(Cleanup);

    glutMainLoop();


}

