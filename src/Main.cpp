#include "Main.h"
#include "Model.h"

using std::vector;

//<-----------------VARIABLES Y ESTRUCTURAS--------------------->
int gWidth, gHeight;
vector <CModel> models;

//Variables globales a usar en AntTweakBar
float color_ambiental[] = { 0.3f, 0.3f, 0.3f, 0.0 };
float color_difuso[] = { 1.0f, 1.0f, 0.0f, 0.0 };
float color_especular[] = { 1.0f, 1.0f, 1.0f, 0.0 };
float color_luz_ambiental[] = { 1.0f, 1.0f, 1.0f, 0.0 };
float color_luz_difuso[] = { 1.0f, 1.0f, 1.0f, 0.0 };
float color_luz_especular[] = { 1.0f, 1.0f, 1.0f, 0.0 };
float colorbb[] = { 1,1,1,0 };
float colorNormals[] = { 1,1,1,0 };
float rotacion[] = { 0.0f, 0.0f, 0.0f, 1.0f };
style currentStyle = Triangulos_rellenos;
projection currentProjection = Perspectiva;
shader currentShader = Gouraud;
int autoRotar = 0, tiempoRotacion = 0, selectedModel = 0, width = 1200, height = 680;
float rotacionInicial[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float scaleT = 1.00, ejeX = 0.0, ejeY = 0.0, ejeZ = 0.0, ejeXL = 1.0, ejeYL = 0.75, ejeZL = 0.0, brillo = 32;
bool selecting = false, zbuffer = true, bculling = true, bounding = true, showNormals = false, boolpersa = true;

GLuint program; //Programa de VBO
glm::mat4 project_mat; //Matriz de Proyeccion
glm::mat4 view_mat; //Matriz de View

//<-----------------FUNCIONES GENERALES--------------------->

//Impresion de pantalla
void initGlew() {
	printf("OpenGL Version: %s \n", glGetString(GL_VERSION));
	printf("Vendor: %s \n", glGetString(GL_VENDOR));
	printf("Renderer: %s \n", glGetString(GL_RENDERER));
	printf("GLSL Version: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

//Para la autorotacion
int getTimeMs() {
	//Para obtener el tiempo de rotacion
#if !defined(_WIN32)
	return glutGet(GLUT_ELAPSED_TIME);
#else
	return (int)GetTickCount();
#endif

}

//Para la rotacion (autorotacion)
void setQuat(const float *eje, float angulo, float *quat) {
	float sina2, norm;
	sina2 = (float)sin(0.5f * angulo);
	norm = (float)sqrt(eje[0] * eje[0] + eje[1] * eje[1] + eje[2] * eje[2]);
	quat[0] = sina2 * eje[0] / norm;
	quat[1] = sina2 * eje[1] / norm;
	quat[2] = sina2 * eje[2] / norm;
	quat[3] = (float)cos(0.5f * angulo);
}

//Para la rotacion (autorotacion)
void multiplicarQuat(const float *q1, const float *q2, float *qout) {
	float qr[4];
	qr[0] = q1[3] * q2[0] + q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1];
	qr[1] = q1[3] * q2[1] + q1[1] * q2[3] + q1[2] * q2[0] - q1[0] * q2[2];
	qr[2] = q1[3] * q2[2] + q1[2] * q2[3] + q1[0] * q2[1] - q1[1] * q2[0];
	qr[3] = q1[3] * q2[3] - (q1[0] * q2[0] + q1[1] * q2[1] + q1[2] * q2[2]);
	qout[0] = qr[0]; qout[1] = qr[1]; qout[2] = qr[2]; qout[3] = qr[3];
}

void display()
{
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	TwDraw();

	float mat[4 * 4];
	float v[4];
	float g_LightMultiplier = 1.0f;
	CModel mod;

	mod.draw(scaleT, rotacion, ejeX, ejeY, ejeZ);

	glColor3f(color_luz_ambiental[0] * color_luz_especular[0] * color_luz_difuso[0],
		color_luz_ambiental[1] * color_luz_especular[1] * color_luz_difuso[1],
		color_luz_ambiental[2] * color_luz_especular[2] * color_luz_difuso[2]
	);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTranslatef(ejeXL, ejeYL, ejeZL);
	glutWireSphere(0.25, 50, 50);

	glutPostRedisplay();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	gWidth = w;
	gHeight = h;

	glViewport(0, 0, gWidth, gHeight);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (boolpersa) {
		project_mat = glm::perspective(45.0f, (float)gWidth / (float)gHeight, 0.1f, 1000.0f);
		gluPerspective(45.0f, (float)gWidth / (float)gHeight, 0.1f, 1000.0f);
	}
	else {
		project_mat = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -0.01f, 100.0f);
		glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -0.01f, 100.0f);
	}

	glm::vec3 eye(0.0f, 0.0f, 2.0f);
	glm::vec3 norm(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	view_mat = glm::lookAt(eye, norm, up);
	gluLookAt(eye[0], eye[1], eye[2], norm[0], norm[1], norm[2], up[0], up[1], up[2]);

	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.5, 0.5, 0.5, 0);
	glColor3f(1.0, 1.0, 1.0);
	TwWindowSize(gWidth, gHeight);
}


//Funcion de mouseFunc
void mouseFunc(int button, int state, int x, int y) {
	if (!TwEventMouseButtonGLUT(button, state, x, y)) {
		if (button == 3) scaleT += 0.01;
		if (button == 4 && scaleT > 0.02) scaleT -= 0.01;
	}
}

//Funcion de motionfunc
void motionFunc(int x, int y) {
	if (!TwEventMouseMotionGLUT(x, y)) {
		glutPostRedisplay();
	}
}

//Funcion de MotionPassive
void passiveMotionFunc(int x, int y) {
	if (!TwEventMouseMotionGLUT(x, y)) {
		glutPostRedisplay();
	}
	glutPostRedisplay();
}

//<-----------------FUNCIONES DE ANTTWEAKBAR--------------------->

//Funcion de salir
void TW_CALL exit(void *clientData) {
	exit(1);
}

//Funcion de cambiar proyeccion
void TW_CALL cambiar_proyeccion(void *clientData) {
	if (currentProjection == 1) boolpersa = true;
	else boolpersa = false;
	reshape(gWidth, gHeight);
}

//Funcion de activar Z buffer
void TW_CALL activateZ(void *clientData) {
	zbuffer = !zbuffer;
}

//Funcion de activar BB
void TW_CALL activateBB(void *clientData) {
	bounding = !bounding;
}

//Funcion de activar Back Face Culling
void TW_CALL activateBF(void *clientData) {
	bculling = !bculling;
}

//Funcion de activar Normales
void TW_CALL activateNormals(void *clientData) {
	showNormals = !showNormals;
}

//Funcion de activar el AutoRotate
void TW_CALL SetAutoRotateCB(const void *value, void *clientData) {
	autoRotar = *(const int *)value;
	if (autoRotar != 0) {
		tiempoRotacion = getTimeMs();
		rotacionInicial[0] = rotacion[0];
		rotacionInicial[1] = rotacion[1];
		rotacionInicial[2] = rotacion[2];
		rotacionInicial[3] = rotacion[3];
	}
}

//Funcion de AutoRotate
void TW_CALL GetAutoRotateCB(void *value, void *clientData) {
	(void)clientData; // unused
	*(int *)value = autoRotar;
}

//Funcion para seleccionar un modelo
void TW_CALL selectModel(void *clientData) {
	if (models.size() > 0) {
		if (!selecting) {
			TwDefine("Menu visible=false");
			TwDefine("Figura visible=true");
			selecting = true;
			rotacion[0] = models[selectedModel].rotacion[0];
			rotacion[1] = models[selectedModel].rotacion[1];
			rotacion[2] = models[selectedModel].rotacion[2];
			rotacion[3] = models[selectedModel].rotacion[3];
			scaleT = models[selectedModel].scaleT;

			currentStyle = models[selectedModel].stylo;
			ejeX = models[selectedModel].ejeX;
			ejeY = models[selectedModel].ejeY;
			ejeZ = models[selectedModel].ejeZ;
		}
		else {
			TwDefine("Menu visible=true");
			TwDefine("Figura visible=false");
			models[selectedModel].rotacion[0] = rotacion[0];
			models[selectedModel].rotacion[1] = rotacion[1];
			models[selectedModel].rotacion[2] = rotacion[2];
			models[selectedModel].rotacion[3] = rotacion[3];
			models[selectedModel].scaleT = scaleT;

			models[selectedModel].color_a[0] = color_ambiental[0];
			models[selectedModel].color_a[1] = color_ambiental[1];
			models[selectedModel].color_a[2] = color_ambiental[2];

			models[selectedModel].color_d[0] = color_difuso[0];
			models[selectedModel].color_d[1] = color_difuso[1];
			models[selectedModel].color_d[2] = color_difuso[2];

			models[selectedModel].color_e[0] = color_especular[0];
			models[selectedModel].color_e[1] = color_especular[1];
			models[selectedModel].color_e[2] = color_especular[2];

			models[selectedModel].stylo = currentStyle;
			models[selectedModel].ejeX = ejeX;
			models[selectedModel].ejeY = ejeY;
			models[selectedModel].ejeZ = ejeZ;
			autoRotar = 0;
			selecting = false;
		}
	}
}

//Funcion que carga un modelo OFF y OBJ con un filechooser
void TW_CALL loadModel(void *clientData) {
	char filename[MAX_PATH];
	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "OBJ and OFF Files\0*.off;*.obj;*.OBJ;*.OFF\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Selecciona un modelo";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
	CModel mod;

	if (GetOpenFileNameA(&ofn)) {
		string real = filename;
		if ((tolower(real[real.size() - 3]) == 'o') && (tolower(real[real.size() - 2]) == 'b') && (tolower(real[real.size() - 1]) == 'j')) {
			mod.sendDataToOpenGL(filename, true);
		}
		else {
			mod.sendDataToOpenGL(filename, false);
		}
	}

}

int main(int argc, char* argv[])
{
	gWidth = 1200;
	gHeight = 680;

	float eje[] = { 0.0f, 0.0f, 0.0f };
	float angulo = 0.0f;
	TwBar *menuTW;
	TwBar *modelTW;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(gWidth, gHeight);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Proyecto 2 - Despliegue 3D");

	glutMouseFunc((mouseFunc));
	glutDisplayFunc(display);
	glutPassiveMotionFunc(passiveMotionFunc);
	glutMotionFunc(motionFunc);

	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
	TwGLUTModifiersFunc(glutGetModifiers);
	glutReshapeFunc(reshape);
	TwGLUTModifiersFunc(glutGetModifiers);

	if (!TwInit(TW_OPENGL, NULL))
		return EXIT_FAILURE;

	initGlew();

	menuTW = TwNewBar("Menu");
	TwWindowSize(gWidth, gHeight);

	//Botones del menu inicial en AntTweakBar
	TwDefine("Menu refresh = '0.0001f'");
	TwDefine("Menu resizable = false");
	TwDefine("Menu fontresizable = false");
	TwDefine("Menu movable = false");
	TwDefine("Menu position = '20 20'");
	TwDefine("Menu size = '220 320'");

	TwAddButton(menuTW, "load", loadModel, NULL, " label='Abrir modelo' key=CTRL+o");
	TwAddVarRW(menuTW, "model", TW_TYPE_INT32, &selectedModel, "min=0 step=1 label='Modelo' group='Seleccionar modelo'");
	TwAddButton(menuTW, "select", selectModel, NULL, " label='Seleccionar' key=s group='Seleccionar modelo'");
	TwAddButton(menuTW, "exit", exit, NULL, " label='Salir' key=Esc");

	modelTW = TwNewBar("Figura");
	//Botones de menu Figura
	TwWindowSize(gWidth, gHeight);
	TwDefine("Figura visible=false size='270 680' position='20 20' color='128 0 0' label='Modelos'");

	//Grupo de proyeccion
	TwAddButton(modelTW, "select", selectModel, NULL, " label='Volver al Menu' key=m");
	TwEnumVal projEN[2] = { { Perspectiva, "Perspectiva" },{ Ortogonal, "Ortogonal" } };
	TwType projType = TwDefineEnum("ProjType", projEN, 2);
	TwAddVarRW(modelTW, "Proyeccion", projType, &currentProjection, "group='Proyeccion2'");
	TwAddButton(modelTW, "Activate_P", cambiar_proyeccion, NULL, " label='Activar' group='Proyeccion2'");

	//Activar Z Buffer
	TwAddButton(modelTW, "activateZ", activateZ, NULL, " label='Activar' group='Z-Buffer'");
	TwAddVarRW(modelTW, "zBuffer", TW_TYPE_BOOL8, &zbuffer, "label='Z-Buffer' readonly=true group='Z-Buffer'");

	//Activar BackFace Culling
	TwAddButton(modelTW, "activateBF", activateBF, NULL, " label='Activar' group='Back-Face Culling'");

	//Tipo de despliegue (mallado, triangulos, puntos)
	TwAddVarRW(modelTW, "bCulling", TW_TYPE_BOOL8, &bculling, "label='Back-Face Culling' readonly=true group='Back-Face Culling'");
	TwEnumVal styleEN[3] = { { Puntos, "Puntos" },{ Mallado, "Mallado" },{ Triangulos_rellenos, "Triangulos rellenos" } };
	TwType stylo = TwDefineEnum("ShapeType", styleEN, 3);
	TwAddVarRW(modelTW, "Estilo", stylo, &currentStyle, "");

	//Tipo de shader (Gouraud, Flat, Phong)
	TwEnumVal styleShader[2] = { { Gouraud, "Gouraud" },{ Flat, "Flat" } };
	TwType shader = TwDefineEnum("ShaderType", styleShader, 2);
	TwAddVarRW(modelTW, "Shader", shader, &currentShader, "");

	//Botones Bounding Box
	TwAddVarRW(modelTW, "Color BB", TW_TYPE_COLOR3F, &colorbb, "label='Color' group='BoundingBox'");
	TwAddButton(modelTW, "activateBB", activateBB, NULL, " label='Activar' group='BoundingBox'");
	TwAddVarRW(modelTW, "bounding", TW_TYPE_BOOL8, &bounding, "label='Bounding Box' readonly=true group='BoundingBox'");

	//Botones Normales
	TwAddVarRW(modelTW, "colorNormals", TW_TYPE_COLOR3F, &colorNormals, "label='Color' group='Normales'");
	TwAddVarRW(modelTW, "normals", TW_TYPE_BOOL8, &showNormals, "label='Normales' readonly=true group='Normales'");
	TwAddButton(modelTW, "activateNormals", activateNormals, NULL, " label='Activar' group='Normales'");

	//Botones rotacion, traslacion y escalamiento
	TwAddVarRW(modelTW, "scale", TW_TYPE_FLOAT, &scaleT, "min=0.01 step=0.01 label='Escalar'");
	TwAddVarRW(modelTW, "ejeX", TW_TYPE_FLOAT, &ejeX, "step=0.01 label='x' group='Trasladar'");
	TwAddVarRW(modelTW, "ejeY", TW_TYPE_FLOAT, &ejeY, "step=0.01 label='y' group='Trasladar'");
	TwAddVarRW(modelTW, "ejeZ", TW_TYPE_FLOAT, &ejeZ, "step=0.01 label='z' group='Trasladar'");
	TwAddVarCB(modelTW, "autoRotate", TW_TYPE_BOOL32, SetAutoRotateCB, GetAutoRotateCB, NULL, " label='Auto-rotacion' group='Rotar'");
	TwAddVarRW(modelTW, "rotation", TW_TYPE_QUAT4F, &rotacion, " label='Rotacion' opened=true group='Rotar'");

	//Color del material, ambiental, difuso y especular
	TwAddVarRW(modelTW, "Color Ambiental", TW_TYPE_COLOR3F, &color_ambiental, "label='Ambiental' group='Material'");
	TwAddVarRW(modelTW, "Color Difuso", TW_TYPE_COLOR3F, &color_difuso, "label='Difuso' group='Material'");
	TwAddVarRW(modelTW, "Color Especular", TW_TYPE_COLOR3F, &color_especular, "label='Especular' group='Material'");

	//Botones de Luz. Brillo. Color de Luz, Posicion de Luz.
	TwAddVarRW(modelTW, "Brillo", TW_TYPE_FLOAT, &brillo, "min=1.0 max=400.0 step=1.0 label='Brillo' group='Luz'");
	TwAddVarRW(modelTW, "ejeXL", TW_TYPE_FLOAT, &ejeXL, "step=0.01 label='x' group='Trasladar luz' group='Luz'");
	TwAddVarRW(modelTW, "ejeYL", TW_TYPE_FLOAT, &ejeYL, "step=0.01 label='y' group='Trasladar luz' group='Luz'");
	TwAddVarRW(modelTW, "ejeZL", TW_TYPE_FLOAT, &ejeZL, "step=0.01 label='z' group='Trasladar luz' group='Luz'");
	TwAddVarRW(modelTW, "Color AmbientalL", TW_TYPE_COLOR3F, &color_luz_ambiental, "label='Color Ambiental' group='Luz'");
	TwAddVarRW(modelTW, "Color DifusoL", TW_TYPE_COLOR3F, &color_luz_difuso, "label='Color Difuso' group='Luz'");
	TwAddVarRW(modelTW, "Color EspecularL", TW_TYPE_COLOR3F, &color_luz_especular, "label='Color Especular' group='Luz'");

	//Boton de salir
	TwAddButton(modelTW, "exitF", exit, NULL, " label='Salir' key=Esc");

	//Para que sirva VBO
	glewExperimental = GL_TRUE;
	glewInit();

	//Auto-rotacion
	tiempoRotacion = getTimeMs();
	setQuat(eje, angulo, rotacion);
	setQuat(eje, angulo, rotacionInicial);

	glutMainLoop();
	return EXIT_SUCCESS;
}