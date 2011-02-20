#include "Burbuja.h"
#include <cstdlib>

const GLfloat Burbuja::mat_specular[] = { 1 ,1, 1.0 ,0.40 };
const GLfloat Burbuja::mat_diffuse[] = { 0.15 ,0.15, 0.60 ,0.70 };
const GLfloat Burbuja::mat_shininess[] = { 70.0 };
Curva Burbuja::curva;

Burbuja::Burbuja(float x, float y, float z) {
	// TODO
	radio = (float)((rand()%19)+1)/20.0;
	tramo = 0;

	/* trayectoria que seguira la burbuja */
	std::vector<Vertice> puntos;
	Vertice v;
	v.x = x; v.y = y; v.z = z;
	puntos.push_back(v);
	puntos.push_back(v);
	puntos.push_back(v);
	v.x += (float)(rand()%20 - 10)/5;
	v.y += (float)(rand()%20 - 10)/5;
	v.z += 4;
	puntos.push_back(v);
	v.x += (float)(rand()%20 - 10)/5;
	v.y += (float)(rand()%20 - 10)/5;
	v.z += 6;
	puntos.push_back(v);
	v.x += (float)(rand()%20 - 10)/5;
	v.y += (float)(rand()%20 - 10)/5;
	v.z += 10;
	puntos.push_back(v);
	puntos.push_back(v);
	puntos.push_back(v);
	curva.Bspline(puntos,trayectoria);
}

Burbuja::~Burbuja() {
	trayectoria.clear();
}

void Burbuja::dibujar() {
	glEnable(GL_BLEND);
	    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	    glBlendFunc (GL_SRC_ALPHA, GL_ONE);	//transparencia
	    glutSolidSphere (radio, 20, 10);
	glDisable(GL_BLEND);
}

Vertice* Burbuja::getPos() {
	Vertice* v = NULL;
	if (tramo < trayectoria.size()) { //todavia le queda trayectoria
		v = &(trayectoria[tramo]);
	}
	return v;
}

void Burbuja::animar() {
	tramo++;
}

