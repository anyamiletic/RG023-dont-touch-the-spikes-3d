#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "callbacks.h"
#include "functions.h"

#define SPACEBAR 32



//deklaracije globalnih promenljivih
int timer_active;
float window_width;
float window_height;



	//promenljive vezane za lokaciju sfere
float brojac;
float translate_y;
float translate_x;
int collision;
bool jump;
bool fall;

	//promenljive vezane za lokaciju spikesa
float spike_width_left;
float spike_width_right;



int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("ImeAplikacije");

	glutDisplayFunc(on_display);
	glutMouseFunc(on_mouse);
	glutKeyboardFunc(on_keyboard);
	glutReshapeFunc(on_reshape);


	glEnable(GL_DEPTH_TEST);
	glClearColor(0.75, 0.75, 0.75, 0);

	//inicijalizacija globalnih promenljivih
	timer_active = 0;
	brojac = 0;
	translate_y = 0;
	translate_x = 0;
	collision = 0; //nije doslo do kolizije
	jump = false;
	spike_width_left = 0;
	spike_width_right = 0;
	

	glutMainLoop();
	return 0;
}



