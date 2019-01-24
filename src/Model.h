// Brian Torres
#pragma once
#include "Main.h"

class CModel {
public:
	int nVertices;
	int nFaces;
	int nEdges;
	GLuint vbo;
	GLuint vindex;
	vector<GLuint> face_index;
	vector<float> vertices;
	vector<float> normales;
	vector<float> normales_cara_fin;
	vector<float> normales_vertice_fin;
	float rotacion[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	double scaleT = 1.00;
	float mat[4 * 4];
	float color_a[4] = { 0.5f, 0.4f, 0.4f, 0.0 };
	float color_d[4] = { 1.0f, 0.5f, 0.0f, 0.0 };
	float color_e[4] = { 1.0f, 1.0f, 1.0f, 0.0 };
	style stylo = Triangulos_rellenos;
	float ejeX = 0.0;
	float ejeY = 0.0;
	float ejeZ = 0.0;
	double minX = INT_MAX, minY = INT_MAX, minZ = INT_MAX, maxX = INT_MIN, maxY = INT_MIN, maxZ = INT_MIN;
	CModel();
	~CModel();
	void initVBO();
	GLuint installShaders();
	void sendDataToOpenGL(char *nombre, bool is_obj);
	void draw(float scaleT, float rotacion[4], float ejeX, float ejeY, float ejeZ);
};

struct vertice {
	double x;
	double y;
	double z;
};

struct face {
	int n_vertex;
	vector<int> f;
};

