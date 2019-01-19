#include "Main.h"
#include "Model.h"
#include "Off.h"
#include "UserInterface.h"

using std::vector;

GLFWwindow *gWindow;
int gWidth, gHeight;
CUserInterface * userInterface;
vector <CModel> models;
int picked;
//<-----------------VARIABLES Y ESTRUCTURAS--------------------->
//Variables globales a usar en AntTweakBar
float color_ambiental[] = { 0.5f, 0.0f, 0.0f, 0.0 };
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
int autoRotar = 0, tiempoRotacion = 0, selectedModel = 0, width = 512, height = 512;
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

void updateUserInterface()
{
	if (picked > -1) {
		/*models[picked]->setTranslation(userInterface->getModelTranslation());*/
	}		
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (unsigned int i = 0; i < models.size(); i++)
	{
		/*glm::vec3 translation = models[i]->getTranslation();*/

		glPushMatrix();
		/*glTranslatef(translation.x, translation.y, translation.z);
		models[i]->display();*/
		glPopMatrix();
	}

}

void reshape(GLFWwindow *window, int width, int height)
{
	gWidth = width;
	gHeight = height;

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

void keyInput(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (TwEventKeyGLFW(key, action))
		return;

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		}
	}
}

void mouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (TwEventMouseButtonGLFW(button, action))
		return;
}

void cursorPos(GLFWwindow* window, double x, double y)
{
	if (TwEventMousePosGLFW(int(x), int(y)))
		return;
}

void charInput(GLFWwindow* window, unsigned int scanChar)
{
	if (TwEventCharGLFW(scanChar, GLFW_PRESS))
		return;
}

void destroy()
{
	delete userInterface;

	TwTerminate();
	glfwDestroyWindow(gWindow);
	glfwTerminate();
}

bool initGlfw()
{
	if (!glfwInit())
		return false;

	gWindow = glfwCreateWindow(gWidth, gHeight, "Despliegue 3D", NULL, NULL);

	if (!gWindow)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(gWindow);

	const GLFWvidmode * vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(gWindow, (vidMode->width - gWidth) >> 1, (vidMode->height - gHeight) >> 1);

	glfwSetWindowSizeCallback(gWindow, reshape);
	glfwSetKeyCallback(gWindow, keyInput);
	glfwSetMouseButtonCallback(gWindow, mouseButton);
	glfwSetCursorPosCallback(gWindow, cursorPos);
	glfwSetCharCallback(gWindow, charInput);

	return true;
}

bool initUserInterface()
{
	if (!TwInit(TW_OPENGL, NULL))
		return false;

	userInterface = CUserInterface::Instance();

	return true;
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
	reshape(gWindow, gWidth, gHeight);
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

int main(void)
{
	gWidth = 1200;
	gHeight = 680;
	picked = 0;

	if (!initGlfw() || !initUserInterface())
		return EXIT_FAILURE;

	glEnable(GL_DEPTH_TEST);

	reshape(gWindow, gWidth, gHeight);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(3.0f, 3.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	while (!glfwWindowShouldClose(gWindow))
	{
		display();

		TwDraw();

		updateUserInterface();

		glfwSwapBuffers(gWindow);

		glfwPollEvents();
	}

	destroy();

	return EXIT_SUCCESS;
}