#include "SuperficieRevolucion.h"
#include <math.h>


SuperficieRevolucion::SuperficieRevolucion(std::vector<Vertice> &forma, float angulo, Vertice eje1, Vertice eje2, uint pasos) {

	tam = forma.size(); //guardo la long de la curva patron
	cerrada = !((int)angulo%360);

	//guardo la malla de vertices de la superficie formada

	Vertice v, v1, q;

	//traslado el eje
	eje2 -= eje1;
	eje2.normalizar();

	//proyecto sobre yz y calculo el angulo
	v = eje2.proyeccionYZ();
	double norm = v.modulo();
	double sen_x = v.y/norm;
	double cos_x = v.z/norm;

	if (eje2.esIgual(1,0,0)) { //caso especial si el eje de rotacion es x
		sen_x = 0;
		cos_x = 1;
	}

	v1 = eje2;
	Util::rotar(v1.y, v1.z, sen_x, cos_x);

	double sen_y = v1.x / v1.modulo();
	double cos_y = v1.z / v1.modulo();

	//aplico rotacion

	for (uint i=0; i<=pasos; i++) {
		for(unsigned int pos=0 ; pos <  tam ; pos++) {

			q = forma[pos];
			//traslado al origen
			q -= eje1;
			//roto sobre eje x
			Util::rotar(q.y, q.z, sen_x, cos_x);
			//roto sobre eje y
			Util::rotar(q.z, q.x, sen_y, cos_y);
			//roto el angulo alfa en el eje z
			Util::rotar(q.x, q.y, -angulo*i/pasos);

			//hago el camino inverso

			//roto sobre eje y
			Util::rotar(q.z, q.x, -sen_y, cos_y);
			//roto sobre eje x
			Util::rotar(q.y, q.z, -sen_x, cos_x);
			//traslado
			q += eje1;

			superficie.push_back(q);
		}
	}
	init();

}

SuperficieRevolucion::~SuperficieRevolucion() {}


