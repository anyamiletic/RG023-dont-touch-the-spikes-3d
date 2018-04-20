#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "functions.h"
#include "callbacks.h"

extern float window_width;
extern float window_heigth;

void draw_new_spike(const char *side){
	glEnable(GL_LIGHTING);
	

	glColor3f(0, 0, 0.7);

	glPushMatrix();
	if(strcmp("left", side) == 0){
		glTranslatef(-window_width/2, 0, 0);
		glRotatef(90, 0, 1, 0);
	}
	else{
		glTranslatef(window_width/2, 0, 0);
		glRotatef(-90, 0, 1, 0);
	}
	glutSolidCone(20, 25, 20, 8);
	glPopMatrix();
}