#pragma once

#define GLFW_INCLUDE_GLU
#include "../headers/GL/glew.h"
#include "../headers/glfw3.h"
#include "../headers/glm/common.hpp"
#include "../headers/glm/gtc/matrix_transform.hpp"
#include "../headers/glm/gtc/type_ptr.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <windows.h>

using namespace std;

typedef enum { Perspectiva = 1, Ortogonal } projection;
typedef enum { Puntos = 1, Mallado, Triangulos_rellenos } style;

//Para las normales y su offset
#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

void updateUserInterface();

void display();

int getTimeMs();

void setQuat(const float *eje, float angulo, float *quat);

void multiplicarQuat(const float *q1, const float *q2, float *qout);

void reshape(GLFWwindow *window, int width, int height);

void keyInput(GLFWwindow *window, int key, int scancode, int action, int mods);

void mouseButton(GLFWwindow* window, int button, int action, int mods);

void cursorPos(GLFWwindow* window, double x, double y);

void charInput(GLFWwindow* window, unsigned int scanChar);

void destroy();

bool initGlfw();

bool initUserInterface();

int main(void);