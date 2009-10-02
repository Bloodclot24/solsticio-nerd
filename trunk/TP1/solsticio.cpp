///
/// Sistemas Gráficos - 6671
/// 2do cuatrimestre 2009
///
#include "GL/glut.h"
#include <stdlib.h>

#include <iostream> //TODO sacar

#include "Primitivas Graficas/Linea/linea.h"
#include "Primitivas Graficas/Poligono.h"
#include "Primitivas Graficas/Numero.h"


#define ANCHO	800
#define ALTO	600

char caption[]="Solsticio Nerd";

void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, (GLdouble)w, (GLdouble)h, 0.0);
}

void init(void)
{
}


void display(void)
{
//	Poligono p;  //TODO esta para testear :P
//	p.agregarVertice(10,10);
//	p.agregarVertice(200,100);
//	p.agregarVertice(150,300);
//	p.agregarVertice(600,100);
	Numero n(CUATRO);

	//Linea linea;
	///
	glClear(GL_COLOR_BUFFER_BIT);
   	glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

    glBegin(GL_POINTS);
//		glColor3f(0,1.0,1.0);
//		linea.lineaBresenham(10,10,300,200);
//
		glColor3f(255.0,0,6.0);
//		linea.lineaDDA(500, 5, 6, 750);

    //p.dibujar();
		n.dibujar();

	glEnd();
	
	///
  	glutSwapBuffers();
	///
}




/* x, y coordenadas mouse cuando se presiona key
*/
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{		//toma solo las minusculas
	case 0x2B:
		std::cout<<".....+......"<<std::endl;
		break;
	case 0x2D:
		std::cout<<".....-......"<<std::endl;
		break;
	case 0x62:
		std::cout<<".....b......"<<std::endl;
		break;
	case 0x64:
		std::cout<<"......d......"<<std::endl;
		break;
	case 0x1b:
		exit (1);
		break;
	}
}

void specialKeys(int key,int x, int y) {

	switch(key) {
	case GLUT_KEY_LEFT:
		std::cout<<"........izquierda........"<<std::endl;
		break;
	case GLUT_KEY_RIGHT:
		std::cout<<"..........derecha.........."<<std::endl;
		break;
	case GLUT_KEY_UP:
		std::cout<<".........arriba........"<<std::endl;
		break;
	case GLUT_KEY_DOWN:
		std::cout<<"......abajo........"<<std::endl;
		break;
	}
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (ANCHO, ALTO);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (caption);
   init ();
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(specialKeys);
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMainLoop();

   return 0;
}
