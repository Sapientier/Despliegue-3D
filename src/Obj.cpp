#include "Obj.h"

CObj::CObj()
{
}

CObj::~CObj()
{
}

extern vector<CModel> models; //Todos los modelos iran en este vector

//Para separar un string
vector<string> CObj::split(const string &s, char delim) {
	stringstream ss(s);
	string item;
	vector<string> tokens;
	while (getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	return tokens;
}

//Lector de OBJ
void CObj::read_obj(string path) {
	int nFigures, nVertices = 0; int nFaces = 0; int nEdges = 0;
	vector<vertice> auxVertices_1, auxVertices_2;
	char line[4096] = ""; //Leo 4096 char en una linea
	ifstream file;
	bool centrado = true;
	file.open(path);
	string comprobacion;
	map<vertice, vertice> normal_vertice_real;
	map<vertice, int> normal_vertice_total;
	double x, y, z, minX = INT_MAX, minY = INT_MAX, minZ = INT_MAX, maxX = INT_MIN, maxY = INT_MIN, maxZ = INT_MIN;
	vertice v_secundario, normal;
	CModel m;
	int contador = 0;
	vector<face> faces;
	while (!file.eof()) { //Hasta terminar el archivo
		file >> comprobacion; // Leo el primer string (para ver si es vn, vt, v o f)
		if (comprobacion == "v") {
			//Como es v leo los x y z pertinentes, agrego al vector y sumo un vertice
			file >> x >> y >> z;
			if (x > maxX) maxX = x; if (y > maxY) maxY = y; if (z > maxZ) maxZ = z;
			if (x < minX) minX = x; if (y < minY) minY = y; if (z < minZ) minZ = z;
			v_secundario.x = x;
			v_secundario.y = y;
			v_secundario.z = z;
			auxVertices_1.push_back(v_secundario);
			nVertices++;
		}
		else if (comprobacion == "f") { //Si es f leo la cara                       
										//Obtengo la cara
			face cara;
			int nFacesAux = 0;
			file.getline(line, 4096); //Obtengo toda la linea (sin el "f")
			vector<string> indices_vertices;
			string f_auxiliar = "";
			string auxiliar = line;
			auxiliar.erase(auxiliar.find_last_not_of(" ") + 1); //Borro espacios al final de la linea
			stringstream ss(auxiliar);
			while (!ss.eof()) { //Recorro la linea que agarre hasta el final
				ss >> f_auxiliar; //Leo strings
				if (f_auxiliar != "\0") {
					indices_vertices = split(f_auxiliar, '/'); //separo el string por "/"
					nFacesAux++;
					const char * c = indices_vertices[0].c_str(); //Siempre el [0] sera el vertice, lo transformo a int
					int real_index = atoi(c) - 1;
					cara.f.push_back(real_index);
					auxiliar = f_auxiliar;
				}

			}
			//Termine esa cara, pusheo al vector de caras
			if (nFacesAux != 0) {
				cara.n_vertex = nFacesAux;
				faces.push_back(cara);
				nFaces++;
			}

		}
		else if ((comprobacion[0] != 'v' && comprobacion[0] != 'f')) {
			//Si es vt o vn salto la linea
			file.getline(line, 4096);
		}
	}


	//YA LEI EL MODELO, TOCA TRIANGULAR Y ESO

	//Saco los vertices centrales del modelo
	double cx = (maxX + minX) / 2.0; double cy = (maxY + minY) / 2.0; double cz = (maxZ + minZ) / 2.0;
	vertice v, nor;
	double maxXYZ = max(maxX, max(maxY, maxZ));
	minX = INT_MAX, minY = INT_MAX, minZ = INT_MAX, maxX = INT_MIN, maxY = INT_MIN, maxZ = INT_MIN;

	vector<int> totales;
	vector<vertice> normales_pavo;
	vector<vertice> normales_caraV;

	for (int i = 0; i < auxVertices_1.size(); i++) {

		v.x = (auxVertices_1[i].x - cx) / maxXYZ;
		v.y = (auxVertices_1[i].y - cy) / maxXYZ;
		v.z = (auxVertices_1[i].z - cz) / maxXYZ;
		if (v.x > maxX) maxX = v.x; if (v.y > maxY) maxY = v.y; if (v.z > maxZ) maxZ = v.z;
		if (v.x < minX) minX = v.x; if (v.y < minY) minY = v.y; if (v.z < minZ) minZ = v.z;

		m.vertices.push_back(v.x);
		m.vertices.push_back(v.y);
		m.vertices.push_back(v.z);
		auxVertices_2.push_back(v);

		nor.x = 0;
		nor.y = 0;
		nor.z = 0;
		normales_pavo.push_back(nor);
		normales_caraV.push_back(nor);

		totales.push_back(0);
	}

	for (int i = 0; i < faces.size(); i++) {
		int v1, v2, v3, aux = 1, k = 0;
		double centrohipico_x, centrohipico_y, centrohipico_z;
		vertice p1, p2, p3, u, v;

		while (faces[i].n_vertex - 1 > aux) {
			v1 = faces[i].f[k];
			v2 = faces[i].f[aux];
			v3 = faces[i].f[++aux];

			m.face_index.push_back(v1);
			m.face_index.push_back(v2);
			m.face_index.push_back(v3);

			//Guardo los centroides de los triangulos (para saber su inicio)
			centrohipico_x = (auxVertices_2[v1].x + auxVertices_2[v2].x + auxVertices_2[v3].x) / 3.00;
			centrohipico_y = (auxVertices_2[v1].y + auxVertices_2[v2].y + auxVertices_2[v3].y) / 3.00;
			centrohipico_z = (auxVertices_2[v1].z + auxVertices_2[v2].z + auxVertices_2[v3].z) / 3.00;
			normal.x = centrohipico_x;
			normal.y = centrohipico_y;
			normal.z = centrohipico_z;
			m.normales.push_back(normal.x); m.normales.push_back(normal.y); m.normales.push_back(normal.z);

			//Saco normales
			u.x = auxVertices_2[v2].x - auxVertices_2[v1].x;
			u.y = auxVertices_2[v2].y - auxVertices_2[v1].y;
			u.z = auxVertices_2[v2].z - auxVertices_2[v1].z;
			v.x = auxVertices_2[v3].x - auxVertices_2[v1].x;
			v.y = auxVertices_2[v3].y - auxVertices_2[v1].y;
			v.z = auxVertices_2[v3].z - auxVertices_2[v1].z;
			normal.x = u.y*v.z - u.z*v.y;
			normal.y = u.z*v.x - u.x*v.z;
			normal.z = u.x*v.y - u.y*v.x;

			//Guardo normales de cara asociada a vertice
			normales_caraV[v1].x = normal.x;
			normales_caraV[v1].y = normal.y;
			normales_caraV[v1].z = normal.z;

			normales_caraV[v2].x = normal.x;
			normales_caraV[v2].y = normal.y;
			normales_caraV[v2].z = normal.z;

			normales_caraV[v3].x = normal.x;
			normales_caraV[v3].y = normal.y;
			normales_caraV[v3].z = normal.z;

			//Guardo suma de normales por vertice
			normales_pavo[v1].x += normal.x;
			normales_pavo[v1].y += normal.y;
			normales_pavo[v1].z += normal.z;

			normales_pavo[v2].x += normal.x;
			normales_pavo[v2].y += normal.y;
			normales_pavo[v2].z += normal.z;

			normales_pavo[v3].x += normal.x;
			normales_pavo[v3].y += normal.y;
			normales_pavo[v3].z += normal.z;

			//Guardo total de la suma de normales para dividir
			totales[v1] += 1;
			totales[v2] += 1;
			totales[v3] += 1;


			normal.x += centrohipico_x + (centrohipico_x / 20.0);
			normal.y += centrohipico_y + (centrohipico_y / 20.0);
			normal.z += centrohipico_z + (centrohipico_z / 20.0);

			m.normales.push_back(normal.x); m.normales.push_back(normal.y); m.normales.push_back(normal.z);

		}

	}

	for (int i = 0; i < auxVertices_2.size(); i++) {
		vertice inicio;
		inicio.x = auxVertices_2[i].x;
		inicio.y = auxVertices_2[i].y;
		inicio.z = auxVertices_2[i].z;
		vertice final;
		m.normales.push_back(inicio.x); m.normales.push_back(inicio.y); m.normales.push_back(inicio.z);

		final.x = (normales_pavo[i].x / (float)totales[i]);
		final.y = (normales_pavo[i].y / (float)totales[i]);
		final.z = (normales_pavo[i].z / (float)totales[i]);

		m.normales_vertice_fin.push_back(final.x); m.normales_vertice_fin.push_back(final.y); m.normales_vertice_fin.push_back(final.z);

		final.x += (auxVertices_2[i].x);
		final.y += (auxVertices_2[i].y);
		final.z += (auxVertices_2[i].z);

		m.normales.push_back(final.x); m.normales.push_back(final.y); m.normales.push_back(final.z);
		m.normales_cara_fin.push_back(normales_caraV[i].x); m.normales_cara_fin.push_back(normales_caraV[i].y); m.normales_cara_fin.push_back(normales_caraV[i].z);

	}

	//Coloco max y min en la estructura
	m.minX = minX;
	m.maxX = maxX;
	m.minY = minY;
	m.maxY = maxY;
	m.minZ = minZ;
	m.maxZ = maxZ;

	//Agrego datos finales al modelo
	m.nFaces = nFaces;
	m.nVertices = nVertices;
	m.nEdges = 0;
	models.push_back(m); //Pusheo el modelo a vector de modelos
	file.close();
	CModel mod;
	mod.initVBO();

}