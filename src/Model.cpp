#include "Model.h"
#include "OBJ.h"
#include "OFF.h"

extern const char* vertexShaderCode;
extern const char* fragmentShaderCode;
extern int selectedModel;
extern float color_ambiental[4];
extern float color_difuso[4];
extern float color_especular[4];
extern float color_luz_ambiental[4];
extern float color_luz_difuso[4];
extern float color_luz_especular[4];
extern float colorbb[4];
extern float brillo;
extern float colorNormals[4];
extern style currentStyle;
extern projection currentProjection;
extern shader currentShader;
extern bool boolpersa;
extern float ejeXL;
extern float ejeYL;
extern float ejeZL;
extern float tipo_shader;
extern int autoRotar;
extern int tiempoRotacion;
extern float rotacionInicial[4];
extern int selectedModel;
extern bool selecting;
extern bool zbuffer;
extern bool bculling;
extern bool bounding;
extern bool showNormals;

//Variables para shaders
extern GLuint program; //Programa de VBO
extern glm::mat4 project_mat; //Matriz de Proyeccion
extern glm::mat4 view_mat; //Matriz de View
extern vector<CModel> models; //Todos los modelos iran en este vector

CModel::CModel() {}

CModel::~CModel() {}

//Para crear la matriz de escalamiento
glm::mat4 scale_en_matriz(float scale_tx) {
	glm::mat4 scaleMatrix = glm::mat4(glm::vec4(scale_tx, 0.0, 0.0, 0.0), glm::vec4(0.0, scale_tx, 0.0, 0.0), glm::vec4(0.0, 0.0, scale_tx, 0.0), glm::vec4(0.0, 0.0, 0.0, 1)); //Creo matriz de escalamiento
	return scaleMatrix;
}

//Para crear la matriz de translate
glm::mat4 translate_en_matriz(float translate_tx, float translate_ty, float translate_tz) {
	glm::mat4 translateMatrix = glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 1.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 1.0, 0.0), glm::vec4(translate_tx, translate_ty, translate_tz, 1)); //Creo matriz de translate
	return translateMatrix;
}

//Para crear la matriz de rotacion
glm::mat4 rotacion_en_matriz(float rotacion_tx, float rotacion_ty, float rotacion_tz, float rotacion_ta) {
	glm::mat4 rotateMatrix;
	//Creo matriz de rotacion usando los quat
	rotateMatrix[0][0] = 1.0 - 2.0 * (rotacion_ty * rotacion_ty + rotacion_tz * rotacion_tz);
	rotateMatrix[0][1] = 2.0 * (rotacion_tx * rotacion_ty + rotacion_tz * rotacion_ta);
	rotateMatrix[0][2] = 2.0 * (rotacion_tx * rotacion_tz - rotacion_ty * rotacion_ta);
	rotateMatrix[0][3] = 0.0;
	rotateMatrix[1][0] = 2.0 * (rotacion_tx * rotacion_ty - rotacion_tz * rotacion_ta);
	rotateMatrix[1][1] = 1.0 - 2.0 * (rotacion_tx * rotacion_tx + rotacion_tz * rotacion_tz);
	rotateMatrix[1][2] = 2.0 * (rotacion_ty * rotacion_tz + rotacion_tx * rotacion_ta);
	rotateMatrix[1][3] = 0.0;
	rotateMatrix[2][0] = 2.0 * (rotacion_tx * rotacion_tz + rotacion_ty * rotacion_ta);
	rotateMatrix[2][1] = 2.0 * (rotacion_ty * rotacion_tz - rotacion_tx * rotacion_ta);
	rotateMatrix[2][2] = 1.0 - 2.0 * (rotacion_tx * rotacion_tx + rotacion_ty * rotacion_ty);
	rotateMatrix[2][3] = 0.0;
	rotateMatrix[3][0] = 0.0;
	rotateMatrix[3][1] = 0.0;
	rotateMatrix[3][2] = 0.0;
	rotateMatrix[3][3] = 1.0;
	return rotateMatrix;
}

//Crea los VBOS con vertice,normal y normal por cara
void CModel::initVBO() {
	glGenBuffers(1, &(models[models.size() - 1].vbo));

	glBindBuffer(GL_ARRAY_BUFFER, models[models.size() - 1].vbo);
	glBufferData(GL_ARRAY_BUFFER, models[models.size() - 1].vertices.size() * sizeof(float) + models[models.size() - 1].normales_vertice_fin.size() * sizeof(float) + models[models.size() - 1].normales_cara_fin.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	//Guardo Vertices en el VBO
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		models[models.size() - 1].vertices.size() * sizeof(float),
		models[models.size() - 1].vertices.data());
	//Guardo normales por vertice en el VBO
	glBufferSubData(GL_ARRAY_BUFFER,
		models[models.size() - 1].vertices.size() * sizeof(float),
		models[models.size() - 1].normales_vertice_fin.size() * sizeof(float),
		models[models.size() - 1].normales_vertice_fin.data());
	//Guardo normales por cara en el VBO
	glBufferSubData(GL_ARRAY_BUFFER,
		models[models.size() - 1].vertices.size() * sizeof(float) + models[models.size() - 1].normales_vertice_fin.size() * sizeof(float),
		models[models.size() - 1].normales_cara_fin.size() * sizeof(float),
		models[models.size() - 1].normales_cara_fin.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &(models[models.size() - 1].vindex));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, models[models.size() - 1].vindex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, models[models.size() - 1].face_index.size() * sizeof(GLuint), models[models.size() - 1].face_index.data(), GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//Compilacion, linkeamiento e instalacion de shaders
GLuint CModel::installShaders() {
	GLuint programID;
	GLuint vertexID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

	const char * adapter[1];
	adapter[0] = vertexShaderCode;
	glShaderSource(vertexID, 1, adapter, 0);
	adapter[0] = fragmentShaderCode;
	glShaderSource(fragmentID, 1, adapter, 0);

	glCompileShader(fragmentID);
	glCompileShader(vertexID);

	GLint vStatus;
	GLint fStatus;
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &vStatus);
	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &fStatus);
	printf("Status del vertex: %d\n", vStatus);
	printf("Status del fragment: %d\n", fStatus);

	GLsizei vInfoLen;
	GLsizei fInfoLen;
	glGetShaderiv(vertexID, GL_INFO_LOG_LENGTH, &vInfoLen);
	glGetShaderiv(fragmentID, GL_INFO_LOG_LENGTH, &fInfoLen);
	char *vInfoLog = (char*)malloc(vInfoLen);
	char *fInfoLog = (char*)malloc(fInfoLen);
	glGetShaderInfoLog(vertexID, vInfoLen, &vInfoLen, vInfoLog);
	glGetShaderInfoLog(fragmentID, fInfoLen, &fInfoLen, fInfoLog);
	printf("Compilar vertex shader: %s\n", vInfoLog);
	printf("Compilar fragment shader: %s\n", fInfoLog);

	if (vStatus == GL_FALSE) {
		printf("Error de compilacion en el vertex shader!\n");
	}
	if (fStatus == GL_FALSE) {
		printf("Error de compilacion en el fragment shader!\n");
	}
	printf("Compilacion exitosa\n");

	programID = glCreateProgram();
	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);

	glLinkProgram(programID);

	printf("Enlace de programa hecho\n");
	GLint linked;
	glGetProgramiv(programID, GL_LINK_STATUS, &linked);
	GLsizei infoLen;
	char infoLog[100];
	glGetProgramInfoLog(programID, 100, &infoLen, infoLog);
	printf("Enlace de programa:%s\n", infoLog);
	if (linked == GL_TRUE)
	{
		printf("Enlazado de shaders exitoso\n\n");
		return programID;
	}
	else
	{
		printf("Enlazado de shaders fallido\n\n");
		return 0;
	}
}

//Crea el programa y llama a los read
void CModel::sendDataToOpenGL(char *nombre, bool is_obj) {
	program = installShaders();
	if (is_obj) read_obj(nombre);
	else read_off(nombre);
}

//Funcion que dibuja el modelo VBO, BB y normales
void CModel::draw(float scaleT, float rotacion[4], float ejeX, float ejeY, float ejeZ) {
	if (selectedModel >= models.size()) selectedModel = models.size() - 1;
	if (zbuffer) glDepthMask(GL_TRUE);
	else glDepthMask(GL_FALSE);

	for (int i = 0; i < models.size(); i++) { //Para los modelos

		glUseProgram(program);

		GLuint view_matr_loc = glGetUniformLocation(program, "view_matrix");
		GLuint model_matr_loc = glGetUniformLocation(program, "model_matrix");
		GLuint proj_matr_loc = glGetUniformLocation(program, "projection_matrix");
		GLuint color_am = glGetUniformLocation(program, "color_ambiental");
		GLuint color_di = glGetUniformLocation(program, "color_difuso");
		GLuint color_es = glGetUniformLocation(program, "color_especular");
		GLuint light_loc = glGetUniformLocation(program, "lightPos");
		GLuint view_loc = glGetUniformLocation(program, "view");
		GLuint color_aux = glGetUniformLocation(program, "TheColor");
		GLuint shiny = glGetUniformLocation(program, "brillo_shiny");
		GLuint light_colora_loc = glGetUniformLocation(program, "color_luza");
		GLuint light_colord_loc = glGetUniformLocation(program, "color_luzd");
		GLuint light_colore_loc = glGetUniformLocation(program, "color_luze");

		GLuint bool_loc = glGetUniformLocation(program, "normal_o_bounding");
		GLuint tipo = glGetUniformLocation(program, "tipo_de_shader");

		glUniform1i(bool_loc, 0);
		glUniform1i(tipo, currentShader);
		glUniform3f(view_loc, 0.0, 0.0, 3.0);
		glUniform3f(light_colora_loc, color_luz_ambiental[0], color_luz_ambiental[1], color_luz_ambiental[2]);
		glUniform3f(light_colord_loc, color_luz_difuso[0], color_luz_difuso[1], color_luz_difuso[2]);
		glUniform3f(light_colore_loc, color_luz_especular[0], color_luz_especular[1], color_luz_especular[2]);
		glUniform3f(light_loc, ejeXL, ejeYL, ejeZL);
		//Color de los materiales
		glUniform3f(color_am, models[i].color_a[0], models[i].color_a[1], models[i].color_a[2]);
		glUniform3f(color_di, models[i].color_d[0], models[i].color_d[1], models[i].color_d[2]);
		glUniform3f(color_es, models[i].color_e[0], models[i].color_e[1], models[i].color_e[2]);
		glUniform1f(shiny, brillo);
		//Matrices de view y proyeccion
		glm::mat4 model_mat = translate_en_matriz(models[i].ejeX, models[i].ejeY, models[i].ejeZ);

		if (autoRotar) {
			float eje[3] = { 0, 1, 0 };
			float angulo = (float)(getTimeMs() - tiempoRotacion) / 1000.0f;
			float quat[4];
			setQuat(eje, angulo, quat);
			multiplicarQuat(rotacionInicial, quat, rotacion);
		}

		model_mat = model_mat * rotacion_en_matriz(models[i].rotacion[0], models[i].rotacion[1], models[i].rotacion[2], models[i].rotacion[3]);
		model_mat = model_mat * scale_en_matriz(models[i].scaleT);
		glUniformMatrix4fv(model_matr_loc, 1, GL_FALSE, value_ptr(model_mat));
		glUniformMatrix4fv(view_matr_loc, 1, GL_FALSE, value_ptr(view_mat));
		glUniformMatrix4fv(proj_matr_loc, 1, GL_FALSE, value_ptr(project_mat));


		glBindBuffer(GL_ARRAY_BUFFER, models[i].vbo);
		//VERTICE Y NORMALES
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[i].vertices.size() * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[i].vertices.size() * sizeof(float) + (models[i].normales_vertice_fin.size() * sizeof(float))));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, models[i].vindex);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (models[i].stylo == 1)
			glDrawElements(GL_POINTS, models[i].face_index.size(), GL_UNSIGNED_INT, 0);
		else if (models[i].stylo == 3)
			glDrawElements(GL_TRIANGLES, models[i].face_index.size(), GL_UNSIGNED_INT, 0);
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, models[i].face_index.size(), GL_UNSIGNED_INT, 0);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(0);

		glPushMatrix();

		if (selecting && i == selectedModel) {
			//Para escalar (al principio es 1.0 en los tres)
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
			//Si la figura seleccionada tiene BFC
			if (bculling) {
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}
			else glDisable(GL_CULL_FACE);

			if (bounding) {
				//Pinto el bounding con el fragmentshader
				glUseProgram(program);
				bool_loc = glGetUniformLocation(program, "normal_o_bounding");
				glUniform1i(bool_loc, 1);
				color_aux = glGetUniformLocation(program, "TheColor");
				glUniform3f(color_aux, colorbb[0], colorbb[1], colorbb[2]);

				glBegin(GL_LINE_STRIP);
				glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
				glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].maxZ);
				glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].maxZ);
				glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].minZ);
				glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
				glEnd();

				glBegin(GL_LINE_STRIP);
				glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
				glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].maxZ);
				glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].maxZ);
				glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].maxZ);
				glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
				glEnd();

				glBegin(GL_LINE_STRIP);
				glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
				glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].minZ);
				glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].minZ);
				glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].maxZ);
				glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
				glEnd();

				glBegin(GL_LINE_STRIP);
				glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
				glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].maxZ);
				glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].minZ);
				glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].minZ);
				glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
				glEnd();

				glBegin(GL_LINE_STRIP);
				glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
				glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].minZ);
				glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].maxZ);
				glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].maxZ);
				glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
				glEnd();

				glBegin(GL_LINE_STRIP);
				glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
				glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].minZ);
				glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].minZ);
				glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].minZ);
				glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
				glEnd();
				glUseProgram(0);
			}

			//Pinto las normales usando el fragmentshader
			if (showNormals) {
				glUseProgram(program);
				bool_loc = glGetUniformLocation(program, "normal_o_bounding");
				glUniform1i(bool_loc, 1);
				color_aux = glGetUniformLocation(program, "TheColor");
				glUniform3f(color_aux, colorNormals[0], colorNormals[1], colorNormals[2]);
				glBegin(GL_LINES);
				//Normales vertice
				for (int j = 0; j < models[i].normales.size(); j++) {
					glVertex3f(models[i].normales[j], models[i].normales[j + 1], models[i].normales[j + 2]);
					glVertex3f(models[i].normales[j + 3], models[i].normales[j + 4], models[i].normales[j + 5]);
					j += 5;
				}

				glEnd();
				glUseProgram(0);
			}
		}
		//Quito el BFC
		if (!selecting) {
			glDisable(GL_CULL_FACE);
			bculling = false;
		}
		glPopMatrix();
	}
}