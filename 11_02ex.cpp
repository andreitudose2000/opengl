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
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
using namespace std;

GLuint
	VaoId, VboId, EboId,
	ProgramIdf, ProgramIdv,
	matrUmbraLocation, viewLocation, projLocation,
	codColLocation,
	objectColorLoc, lightColorLoc, lightPosLoc, viewPosLoc,
	rendermode;

float const PI = 3.141592f;

float const U_MIN = -PI / 2, U_MAX = PI / 2, V_MIN = 0, V_MAX = 2 * PI;

int const NR_PARR = 30, NR_MERID = 30;

float step_u = (U_MAX - U_MIN) / NR_PARR, step_v = (V_MAX - V_MIN) / NR_MERID;