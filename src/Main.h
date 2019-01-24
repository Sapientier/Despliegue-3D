// Brian Torres
#pragma once

#include "../headers/GL/glew.h"
#include "../headers/GL/glut.h"
#include "../headers/GL/freeglut.h"
#include "../headers/glm/common.hpp"
#include "../headers/glm/gtc/matrix_transform.hpp"
#include "../headers/glm/gtc/type_ptr.hpp"
#include "../headers/AntTweakBar.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <windows.h>
#include <stdlib.h> 
#include <Commdlg.h>
#include <stdlib.h>

using namespace std;

typedef enum { Gouraud = 1, Flat } shader;
typedef enum { Perspectiva = 1, Ortogonal } projection;
typedef enum { Puntos = 1, Mallado, Triangulos_rellenos } style;

//Para las normales y su offset
#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

void setQuat(const float *eje, float angulo, float *quat);

void multiplicarQuat(const float *q1, const float *q2, float *qout);

void display();

void reshape(int width, int height);

void mouseFunc(int button, int state, int x, int y);

void motionFunc(int x, int y);

void passiveMotionFunc(int x, int y);

int getTimeMs();

void TW_CALL cambiar_proyeccion(void *clientData);

void TW_CALL activateZ(void *clientData);

void TW_CALL activateBB(void *clientData);

void TW_CALL activateBF(void *clientData);

void TW_CALL activateNormals(void *clientData);

void TW_CALL activarLuces1(void *clientData);

void TW_CALL SetAutoRotateCB(const void *value, void *clientData);

void TW_CALL GetAutoRotateCB(void *value, void *clientData);

void TW_CALL selectModel(void *clientData);

void TW_CALL loadModel(void *clientData);

void TW_CALL exit(void *clientData);

int main(int argc, char* argv[]);