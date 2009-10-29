#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include "Primitivas Graficas/Curva/Curva.h"
#include "Controlador/Controlador.h"

// Variables que controlan la ubicación de la cámara en la Escena 3D
float eye[3] = {15.0, 15.0, 5.0};
float at[3]  = { 0.0,  0.0, 0.0};
float up[3]  = { 0.0,  0.0, 1.0};

// Variables asociadas a única fuente de luz de la escena
float light_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float light_position[3] = {10.0f, 10.0f, 8.0f};
float light_ambient[4] = {0.05f, 0.05f, 0.05f, 1.0f};

// Color de la esfera en movimiento dentro de la escena
float color_esfera[4] = {0.5f, 0.5f, 0.2f, 1.0f};

// Variable asociada al movimiento de rotación de la esfera alrededor del eje Z
float rotate_sphere = 0;

// Variables de control
bool view_grid = true;
bool view_axis = true;
bool edit_panel = true;
bool view_curves = true;	//indica si se ven las curvas de control


// Handle para el control de las Display Lists
GLuint dl_handle;
#define DL_AXIS (dl_handle+0)
#define DL_GRID (dl_handle+1)
#define DL_AXIS2D_TOP (dl_handle+2)

// Tamaño de la ventana
GLfloat window_size[2];
#define W_WIDTH window_size[0]
#define W_HEIGHT window_size[1]

#define TOP_VIEW_POSX	((int)((float)W_WIDTH*0.60f))
#define TOP_VIEW_W		((int)((float)W_WIDTH*0.40f))
#define TOP_VIEW_POSY	((int)((float)W_HEIGHT*0.60f))
#define TOP_VIEW_H		((int)((float)W_HEIGHT*0.40f))

// Variables globales
Curva curva;
VertexList pControl;	//puntos de control dibujados con el mouse
VertexList curva_cam;	//curva que describe la camara
Controlador controlador(&curva);

bool mouseDown = false; //indica si se apreta el boton izquierdo del mouse

void OnIdle (void)
{
	rotate_sphere += 0.1;
	if(rotate_sphere > 360.0) rotate_sphere = 0.0;
    glutPostRedisplay();
}

void DrawAxis()
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		// X
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glColor3f(0.0, 0.0, 0.0);
		glVertex3f(15.0, 0.0, 0.0);
		// Y
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glColor3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 15.0, 0.0);
		// Z
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glColor3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 15.0);
	glEnd();
	glEnable(GL_LIGHTING);
}

void DrawAxis2DTopView()
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINE_LOOP);
		// X
		glColor3f(0.0, 0.5, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
		glVertex3f(1.0, 1.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);
	glEnd();
	glBegin(GL_QUADS);
		glColor3f(0.1, 0.1, 0.1);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
		glVertex3f(1.0, 1.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);
	glEnd();

	glEnable(GL_LIGHTING);
}


void DrawXYGrid()
{
	int i;
	glDisable(GL_LIGHTING);
	glColor3f(0.15, 0.1, 0.1);
	glBegin(GL_LINES);
	for(i=-20; i<21; i++)
	{
		glVertex3f(i, -20.0, 0.0);
		glVertex3f(i,  20.0, 0.0);
		glVertex3f(-20.0, i, 0.0);
		glVertex3f( 20.0, i, 0.0);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}
void Set3DEnv()
{
	glViewport (0, 0, (GLsizei) W_WIDTH, (GLsizei) W_HEIGHT); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) W_WIDTH/(GLfloat) W_HEIGHT, 0.10, 100.0);
}

void SetPanelTopEnv()
{
	glViewport (TOP_VIEW_POSX, TOP_VIEW_POSY, (GLsizei) TOP_VIEW_W, (GLsizei) TOP_VIEW_H); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
	gluOrtho2D(-0.10, 1.05, -0.10, 1.05);
}


void init(void) 
{
	dl_handle = glGenLists(3);

	glClearColor (0.02, 0.02, 0.04, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

	// Generación de las Display Lists
	glNewList(DL_AXIS, GL_COMPILE);
		DrawAxis();
	glEndList();
	glNewList(DL_GRID, GL_COMPILE);
		DrawXYGrid();
	glEndList();
	glNewList(DL_AXIS2D_TOP, GL_COMPILE);
		DrawAxis2DTopView();
	glEndList();
}



void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	///////////////////////////////////////////////////
	// Escena 3D
	Set3DEnv();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1], up[2]);
   
	if (view_axis)
		 glCallList(DL_AXIS);
	
	if (view_grid)
		 glCallList(DL_GRID);

	if (view_curves) {
		//dibujar las curvas
	}
	//
	///////////////////////////////////////////////////


	///////////////////////////////////////////////////
	// Panel 2D para la vista superior
	if (edit_panel)
	{
		SetPanelTopEnv();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt (0, 0, 0.5, 0, 0, 0, 0, 1, 0);

		glDisable(GL_LIGHTING);

		VertexList::iterator it;

		//dibujo los puntos de control
		glColor3f(1.0,0,1.0);
		glBegin(GL_POINTS);
		glVertex3f(1.0,1.0,0);
		glVertex3f(0,0,0);
			for (it = pControl.begin(); it != pControl.end(); it++)
				glVertex3f(it->x, it->y, 0.0);
		glEnd();

		//dibujo el poligono de control
		glColor3f(0.0,1.0,1.0);
		glBegin(GL_LINE_STRIP);
			for (it = pControl.begin(); it != pControl.end(); it++)
				glVertex3f(it->x, it->y, 0.0);
		glEnd();

		//dibujo la curva
		//curva.BezierCubica(pControl,,,);

		glEnable(GL_LIGHTING);

		glCallList(DL_AXIS2D_TOP);
	}
	//
	///////////////////////////////////////////////////
	
	glutSwapBuffers();
}

void reshape (int w, int h)
{
   	W_WIDTH  = w;
	W_HEIGHT = h;

	std::cout<<"W: "<<W_WIDTH<< " H: "<<W_HEIGHT<<std::endl<<std::endl;
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
		case 0x1b:
         exit(0);
    	break;
	  case 'g':
		  view_grid = !view_grid;
		  glutPostRedisplay();
			break;
	  case 'a':
		  view_axis = !view_axis;
		  glutPostRedisplay();
			break;
	  case 'e':
		  edit_panel = !edit_panel;
		  glutPostRedisplay();
			break;
	  case 't':
		  view_curves = !view_curves;
		  glutPostRedisplay();
	  case 'c':
		  pControl.clear(); //borra los puntos de control de la curva VER!!!!!!
		  glutPostRedisplay();
    default:
    	break;
   }
}

void mouse(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		std::cout<<"boton izquierdo presionado x:"<<x<<" y: "<<y<<std::endl;

		int x0 = (float)W_WIDTH*0.60;
		int x1 = x0 + (float)W_WIDTH*0.40;
		int y0 = W_HEIGHT - (float)W_HEIGHT*0.60;
		int alto = (float)W_HEIGHT*0.40*0.95;
		int y1 = y0-alto; 	//limite superior


		std::cout<<"x0: "<<x0<<" x1: "<<x1<<" y0: "<<y0<< " y1:"<< y1<<" alto: "<<alto<<std::endl;

		if (edit_panel) { 	//si el edit panel esta habilitado capturo el x e y para dibujar el punto de control

			Vertice2D v;
			v.x = (float)(x - x0) / (float) (x1-x0) ;
			v.y = (float)(y0 - y) / (float) (alto);


			std::cout<<std::endl<<"dibujar punto en x: "<<v.x<<" y: "<<v.y<<std::endl<<std::endl;

			if ( x > x0 && x < x1) {
				if ( y < y0 && y > y1) {

					mouseDown = false; //lo deshabilito para que no rote
					//todavia no funciona bien por un problema de que los TOP_VIEW son constantes y
					//falla  cuando se redimensiona la pantalla

//					Vertice2D v;
//					v.x = (float)(x-TOP_VIEW_POSX) / (float) W_WIDTH;
//					v.y = 1- ((float)(y- TOP_VIEW_POSY) / (float) W_HEIGHT);
//
//					std::cout<<std::endl<<"dibujar punto en x: "<<v.x<<" y: "<<v.y<<std::endl;

					pControl.push_back(v);	//agrego el vertice normalizado
					glutPostRedisplay();
				}
			}
			else
				mouseDown = true;
		}
	}
	else
		mouseDown = false;
}

void mouseMotion(int x, int y) {

	if (mouseDown) {
//		yrot = x - xdiff;TODO
//		xrot = y + ydiff;
		std::cout<<"rotar :P"<<std::endl;
		std::cout<<"."<<std::endl;
//		glutPostRedisplay();
	}
}



int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(1024, 768); 
   glutInitWindowPosition(0, 0);
   glutCreateWindow("TP2 - Sistemas Graficos");
   glutFullScreen();
   init();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMotionFunc(mouseMotion);
   glutIdleFunc(OnIdle);
   glutMainLoop();
   return 0;
}
