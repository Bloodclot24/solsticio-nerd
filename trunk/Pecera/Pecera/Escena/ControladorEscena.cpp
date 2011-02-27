#include "ControladorEscena.h"

#include <GL/glut.h>
#include <stdlib.h>

bool ControladorEscena::animando = ANIMACION_INICIAL;
int ControladorEscena::cam_pez = -1;
ControladorEscena* ControladorEscena::instancia = 0;
const Vertice __v(3,-3,3);

ControladorEscena::ControladorEscena():objetos(*(ContenedorObjetos::getInstancia())){

	//inicializo la escena
	render_mode = RM_INICIAL;
	ver_tray = VER_TRAYECTORIAS;
	instancia = this;

	//inicializo la luz de la escena

	glEnable(GL_LIGHTING);

	glEnable(GL_FOG);
	{
	   GLfloat fogColor[4] = {0, 0.05, 0.10, 1.0};

	   glFogi (GL_FOG_MODE, GL_EXP2);
	   glFogfv (GL_FOG_COLOR, fogColor);
	   glFogf (GL_FOG_DENSITY, 0.01);
	   glHint (GL_FOG_HINT, GL_DONT_CARE);
//	   glHint (GL_FOG_HINT, GL_NICEST);
//	   glHint (GL_FOG_HINT, GL_FASTEST);
	   glFogf (GL_FOG_START, 20.0);
	   glFogf (GL_FOG_END, 50.0);
	}

/*	BURBUJAS */
	float radio=1;
	size_t ubicacion = objetos.getTrayectoriaBurbujas()->cantPosiciones() / CANT_BURBUJAS;
	for (size_t i=0; i<CANT_BURBUJAS; i++) {
		radio = (rand()%25)/20.0 + 0.2;
		burbujas[i] = new ObjetoViajero(BURBUJA, objetos.getTrayectoriaBurbujas(),radio,radio,radio);
		for (size_t j=0; j<(i*ubicacion); j++) { //las desplazo
			burbujas[i]->viajar();
		}
	}

	if (animando)
		ControladorEscena::animar(0);
}

ControladorEscena::~ControladorEscena() {

/* BURBUJAS */
	for (int i=0; i<CANT_BURBUJAS; i++) {
			delete burbujas[i];
	}

	delete ContenedorObjetos::getInstancia();
	delete ContenedorTexturas::getInstancia();
}

void ControladorEscena::generarEscena() {

/* FLORERO Y ALGAS */

	glPushMatrix();
		glRotatef(180,0,0,1);
		glTranslatef(5,4,0);
		objetos.dibujarFlorero(render_mode);
		glTranslatef(1.5,0,1.7);
		objetos.getAnimacion(ALGA1)->dibujar(render_mode);
		objetos.getAnimacion(ALGA2)->dibujar(render_mode);
	glPopMatrix();

//	glPushMatrix();
//		glTranslatef(0,0,5);
//		objetos.dibujarObjeto(ROCA2, render_mode);
//		objetos.getAnimacion(ALGA0)->dibujar(render_mode);
//		glPushMatrix();
//			glRotatef(90,0,0,1);
//			glTranslatef(0,2,0);
//			objetos.getAnimacion(ALGA0)->dibujar(render_mode);
//		glPopMatrix();
//		glPushMatrix();
//			glRotatef(45,0,0,1);
//			glTranslatef(0,2,0);
//			objetos.getAnimacion(ALGA0)->dibujar(render_mode);
//		glPopMatrix();
//		glPushMatrix();
//			glRotatef(180,0,0,1);
//			glTranslatef(0,2,0);
//			objetos.getAnimacion(ALGA0)->dibujar(render_mode);
//		glPopMatrix();
//		glPushMatrix();
//			glRotatef(270,0,0,1);
//			glTranslatef(0,2,0);
//			objetos.getAnimacion(ALGA0)->dibujar(render_mode);
//		glPopMatrix();
//		glPushMatrix();
//			glRotatef(225,0,0,1);
//			glTranslatef(0,2,0);
//			objetos.getAnimacion(ALGA0)->dibujar(render_mode);
//		glPopMatrix();
//		glPushMatrix();
//			glRotatef(315,0,0,1);
//			glTranslatef(0,2,0);
//			objetos.getAnimacion(ALGA0)->dibujar(render_mode);
//		glPopMatrix();
//		glPushMatrix();
//			glRotatef(135,0,0,1);
//			glTranslatef(0,2,0);
//			objetos.getAnimacion(ALGA0)->dibujar(render_mode);
//		glPopMatrix();
//	glPopMatrix();

	objetos.cambiarTexturaRoca("roca.jpg");
	grupoAlgasRocas(5,4,2.9);
	grupoAlgasRocas(-42,60,5.1);
	grupoAlgasRocas(-35,45,4.8);
	grupoAlgasRocas(-35,-35,2.42);
	grupoAlgasRocas(60,-10,7.1);


	objetos.cambiarTexturaRoca("roca2.jpg");
	grupoRocas(20,20,6.2);
	objetos.cambiarTexturaRoca("roca3.jpg");
	grupoRocas(30,-20,6.1);
	objetos.cambiarTexturaRoca("roca6.jpg");
	grupoRocas(-50,0,2.52);
	/* CARDUMEN DORI */
	objetos.dibujarCardumen(objetos.getCardumen(CAR1), render_mode);

	/* CARDUMEN KOI */
	objetos.dibujarCardumen(objetos.getCardumen(CAR2), render_mode);

	/* CARDUMEN PLATEADO */
	objetos.dibujarCardumen(objetos.getCardumen(CAR0), render_mode);


	/* Trayectorias */
	if (ver_tray) {
		objetos.getCardumen(CAR2)->recorrido->dibujarTrayecto();
		objetos.getCardumen(CAR1)->recorrido->dibujarTrayecto();
		objetos.getCardumen(CAR0)->recorrido->dibujarTrayecto();
		objetos.getTrayectoriaBurbujas()->dibujarTrayecto();
	}

	/* Terreno */
//	objetos.dibujarEscenario(render_mode);
	terreno.dibujar(render_mode);

	/* Burbujas */
	Vertice t;
	for (int i=0; i<CANT_BURBUJAS; i++) {
		glPushMatrix();
			glEnable(GL_RESCALE_NORMAL);
			t= burbujas[i]->getPos();
			glTranslatef(t.x-6.3,t.y-4.2,t.z);
			t= burbujas[i]->deformacion;
			glScalef(t.x,t.y,t.z);
			objetos.dibujarObjeto(BURBUJA, render_mode);
			glTranslatef(0.5,0.75,-3);
			objetos.dibujarObjeto(BURBUJA, render_mode);
			glDisable(GL_RESCALE_NORMAL);
		glPopMatrix();
	}

	/* Test de Peces aislados */
//	glEnable(GL_RESCALE_NORMAL);

	/* DORI */
//	glTranslated(0,0,10);
//	objetos.dibujarPez(PEZ1,render_mode,1);

	/* KOI */
//	glTranslated(0,0,10);
//	objetos.dibujarPez(PEZ2,render_mode,1);

	/* Plateado */
//	glTranslated(0,0,10);
//	objetos.dibujarPez(PEZ0,render_mode,1);

//	glDisable(GL_RESCALE_NORMAL);
}

void ControladorEscena::grupoAlgasRocas(float x, float y, float z) {
	glPushMatrix();
		glTranslatef(x,y,z);
		objetos.getAnimacion(ALGA1)->dibujar(render_mode);
		glPushMatrix();
			glRotatef(-35,0,0,1);
			objetos.getAnimacion(ALGA2)->dibujar(render_mode);
		glPopMatrix();
		glPushMatrix();
			glRotatef(45,0,0,1);
			objetos.getAnimacion(ALGA0)->dibujar(render_mode);
			glTranslatef(0.5,-0.5,0);
			glRotatef(160,0,0,1);
			objetos.getAnimacion(ALGA0)->dibujar(render_mode);
		glPopMatrix();
		glTranslatef(0,-2,0);
		objetos.dibujarObjeto(ROCA2,render_mode);
		glTranslatef(2.2,1.5,0);
		glEnable(GL_RESCALE_NORMAL);
		glScalef(2,2,2);
		objetos.dibujarObjeto(ROCA1,render_mode);
		glTranslatef(-1.8,0.3,0);
		objetos.dibujarObjeto(ROCA0,render_mode);
		glDisable(GL_RESCALE_NORMAL);
	glPopMatrix();
}

void ControladorEscena::grupoRocas(float x, float y, float z) {
	glPushMatrix();

		glTranslatef(x,y,z);
		objetos.dibujarObjeto(ROCA0,render_mode);
		glTranslatef(-2,0,0);
		objetos.dibujarObjeto(ROCA1,render_mode);
		glTranslatef(4,4,0);
		objetos.dibujarObjeto(ROCA2,render_mode);
		glTranslatef(0,-5,0);
		objetos.dibujarObjeto(ROCA0,render_mode);
		glEnable(GL_RESCALE_NORMAL);
		glTranslatef(3,-3,0);
		glScalef(5,5,3);
			objetos.dibujarObjeto(ROCA1,render_mode);
		glDisable(GL_RESCALE_NORMAL);
	glPopMatrix();
}


void ControladorEscena::animar(int n=0){

	//animo las superficies
	instancia->objetos.animarAlgas();

	//muevo la camara si estoy en modo pez
	if (cam_pez > -1){
		instancia->cam.setEye(instancia->objetos.getCardumen(cam_pez)->recorrido->getPosicion()+__v);
		instancia->cam.setAt(instancia->objetos.getCardumen(cam_pez)->recorrido->getDireccion().normalizar());
	}

	//muevo los cardumenes
	instancia->objetos.animarPeces();

/* BURBUJAS */
	for (int i=0; i<CANT_BURBUJAS; i++) {
		instancia->burbujas[i]->viajar();
	}

	//redibujo la escena
	glutPostRedisplay();

	//continuo con la animacion
	if (animando && (n >= 0))
		glutTimerFunc(50,animar,++n);
}

int ControladorEscena::camaraCardumen(int nro) {
	if (nro < MAX_CARDUMEN){
		cam_pez = nro;
		cam.setEye(objetos.getCardumen(nro)->recorrido->getPosicion()+__v);
		cam.setAt(objetos.getCardumen(nro)->recorrido->getDireccion().normalizar());
		return nro;
	}else{
		cam_pez = -1;
		cam.reset();
		return -1;
	}
}

void ControladorEscena::nextRenderMode() {
	if (render_mode == GL_FILL) {
		render_mode = GL_TEXTURE;
	}else if (render_mode == GL_TEXTURE) {
		render_mode = GL_LINE;
	}else if (render_mode == GL_LINE) {
		render_mode = GL_FILL;
	}
}

unsigned int ControladorEscena::getRenderMode() {
	return render_mode;
}

Camara* ControladorEscena::getCamara() {
	return &cam;
}

void ControladorEscena::nextAnimationMode() {
	animando = !animando;
	if (animando){
		ControladorEscena::animar();
	}
}

void ControladorEscena::nextTrackDisplayMode(){
	ver_tray = !ver_tray;
}
