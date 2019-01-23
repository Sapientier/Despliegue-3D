#include "Off.h"

extern vector<CModel> models; //Todos los modelos iran en este vector

void read_off(string path)
{
	int nVertices, nFaces, nEdges;
	char line[4096] = ""; //Leo 4096 char en una linea
	bool validFile = false; //Para ver si el archivo es valido
	int lineStart = 0; //Linea inicial (pero donde empieza el contenido REAL del OFF)
	bool centrado = true;
	double minX = INT_MAX, minY = INT_MAX, minZ = INT_MAX, maxX = INT_MIN, maxY = INT_MIN, maxZ = INT_MIN;
	ifstream file;
	file.open(path);
	while (line[0] == '\0' || line[0] == 'O' || line[0] == '#') { //FALTA VALIDAR VAINAS MAS RARAS
		lineStart++;
		file.getline(line, 4096);
	}
	validFile = true;
	stringstream ss(line); //Tomo la linea y me quito los espacios 
	ss >> nVertices >> nFaces >> nEdges; // "8 6 12" del ejemplo de wikipedia (espera UN STRING Y YA)
	if (validFile) {
		CModel m;
		m.nVertices = nVertices;
		m.nFaces = nFaces;
		m.nEdges = nEdges;
		vertice v;
		vector<vertice> auxVertices_1, auxVertices_2;
		for (int i = 0; i < nVertices; i++) {
			double x, y, z;
			file >> x >> y >> z;
			if (x > maxX) maxX = x; if (y > maxY) maxY = y; if (z > maxZ) maxZ = z;
			if (x < minX) minX = x; if (y < minY) minY = y; if (z < minZ) minZ = z;
			v.x = x;
			v.y = y;
			v.z = z;
			auxVertices_1.push_back(v);
		}
		//Centrar los vertices
		double cx = (maxX + minX) / 2;
		double cy = (maxY + minY) / 2;
		double cz = (maxZ + minZ) / 2;
		double maxXYZ = max(maxX, max(maxY, maxZ));
		double minX = INT_MAX, minY = INT_MAX, minZ = INT_MAX, maxX = INT_MIN, maxY = INT_MIN, maxZ = INT_MIN;
		vector<int> totales;
		vector<vertice> normales_pavo;
		vector<vertice> normales_caraV;

		for (int i = 0; i < auxVertices_1.size(); i++) {
			vertice nor;
			v.x = (auxVertices_1[i].x - cx) / maxXYZ;
			v.y = (auxVertices_1[i].y - cy) / maxXYZ;
			v.z = (auxVertices_1[i].z - cz) / maxXYZ;
			m.vertices.push_back(v.x);
			m.vertices.push_back(v.y);
			m.vertices.push_back(v.z);
			if (v.x > maxX) maxX = v.x; if (v.y > maxY) maxY = v.y; if (v.z > maxZ) maxZ = v.z;
			if (v.x < minX) minX = v.x; if (v.y < minY) minY = v.y; if (v.z < minZ) minZ = v.z;
			auxVertices_2.push_back(v);

			nor.x = 0;
			nor.y = 0;
			nor.z = 0;
			normales_pavo.push_back(nor);
			normales_caraV.push_back(nor);

			totales.push_back(0);
		}

		for (int i = 0; i < nFaces; i++) {
			int nFacesAux;
			file >> nFacesAux;
			face fAux;
			fAux.n_vertex = nFacesAux;
			//Guardo la cara
			for (int j = 0; j < nFacesAux; j++) {
				int face_culling;
				file >> face_culling;
				fAux.f.push_back(face_culling);
			}
			//Creo los triangulos
			if (nFacesAux != 0) {
				int v1, v2, v3, aux = 1, k = 0;
				double centrohipico_x, centrohipico_y, centrohipico_z;
				vertice p1, p2, p3, u, v, normal;;
				while (fAux.n_vertex - 1 > aux) {
					v1 = fAux.f[k];
					v2 = fAux.f[aux];
					v3 = fAux.f[++aux];

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

					//Guardo normal cara asociada a vertice
					normales_caraV[v1].x = normal.x;
					normales_caraV[v1].y = normal.y;
					normales_caraV[v1].z = normal.z;

					normales_caraV[v2].x = normal.x;
					normales_caraV[v2].y = normal.y;
					normales_caraV[v2].z = normal.z;

					normales_caraV[v3].x = normal.x;
					normales_caraV[v3].y = normal.y;
					normales_caraV[v3].z = normal.z;

					//Guardo acumulado de normales por vertice
					normales_pavo[v1].x += normal.x;
					normales_pavo[v1].y += normal.y;
					normales_pavo[v1].z += normal.z;

					normales_pavo[v2].x += normal.x;
					normales_pavo[v2].y += normal.y;
					normales_pavo[v2].z += normal.z;

					normales_pavo[v3].x += normal.x;
					normales_pavo[v3].y += normal.y;
					normales_pavo[v3].z += normal.z;

					//Guardo el total de normales por vertice
					totales[v1] += 1;
					totales[v2] += 1;
					totales[v3] += 1;

					normal.x += (centrohipico_x)+(centrohipico_x / 20.00);
					normal.y += (centrohipico_y)+(centrohipico_y / 20.00);
					normal.z += (centrohipico_z)+(centrohipico_z / 20.00);

					m.normales.push_back(normal.x); m.normales.push_back(normal.y); m.normales.push_back(normal.z);
				}
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
		m.minX = minX;
		m.maxX = maxX;
		m.minY = minY;
		m.maxY = maxY;
		m.minZ = minZ;
		m.maxZ = maxZ;
		models.push_back(m);
	}
	file.close();
	CModel mod;
	mod.initVBO();
}