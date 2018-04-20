#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "functions.h"
#include "callbacks.h"

#define SPACEBAR 32

extern int timer_active;
extern float window_width;
extern float window_height;

	//promenljive vezane za lokaciju sfere
extern float brojac;
extern float translate_y;
extern float translate_x;
extern int collision;
extern bool jump;
extern bool fall;

void on_display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//light
	/* Parametri svetla i podrazumevanog materijala */
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };
    GLfloat light_position[] = {10, 10, 10, 0};
 
    GLfloat ambient_coeffs[] = { 0.3, 0.7, 0.3, 1 };
    GLfloat diffuse_coeffs[] = { 0.2, 1, 0.2, 1 };
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
    GLfloat shininess = 30;

	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//pozicioniranje kamere
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
	//

	glEnable(GL_LIGHTING);
 
    /* Postavljaju se svojstva prvog svetla */
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
 
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
 
    /* Postavljaju se svojstva materijala */
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	//pozicioniranje objekata

		//glavna lopta
	glColor3f(0.7, 0, 0);
	glPushMatrix();
	glTranslatef(translate_x, translate_y, 0);
	glutSolidSphere(20, 10, 10);
	glPopMatrix();

	//mislim da je ovo za pomeranje visine cunjeva
	//u oridjidji igrici se spikeovi ne 
	//pomeraju, samo se svaki put ponovo stvaraju
	//glTranslatef(0, - abs(translate_y), 0);
	//piramide - spikes
	draw_new_spike("left");
	draw_new_spike("right");


	glutSwapBuffers();
}

void on_keyboard(unsigned char key, int x, int y){
	switch(key){
		case 27:
			exit(EXIT_SUCCESS);
		case 'g':
		case 'G':
			if(!timer_active){
				timer_active = 1;
				glutTimerFunc(20, on_timer, 0);
			}
			break;
		case SPACEBAR:
			if(!timer_active){
				timer_active = 1;

				glutTimerFunc(20, on_timer, 0);
				
			}
			else{
				jump = true;
				printf("%f\n", window_width);
				/*znaci ovako postizemo da 
				se brzina ne povecava - 
				ne zovemo timer funkciju. genijalno.
				samo menjamo parametre*/
			}
			break;
		case 's':
		case 'S':
			timer_active = 0;
			break;
	
	}
}

void on_reshape(int width, int height){
	glViewport(0, 0, width, height);

	window_width = width;
	window_height = height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(60, (float)3/5, 1, 50);
	glOrtho(-width/2, width/2, -height/2, height/2, -100, 100);
}

void on_mouse(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON){
		if(!timer_active){
				timer_active = 1;
				glutTimerFunc(20, on_timer, 0);
		}
	}

}

void on_timer(int value){
	if(value != 0)
		return;

	//menjanje vrednosti promenljivih koje ucestvuju u animaciji
	

	if(translate_x > window_width/2 - 20){
		collision = 1;
	}
	else if(translate_x < -window_width/2 + 20){
		collision = 0;
	}

	//zasto? da bi krenuo u skok ispocetka, ali sa tog
	//istog mesta. nadam se.
	if(jump) {brojac = 0;}

	brojac = (collision==0) ? brojac+0.1 : brojac-0.1;


	//try to plot  -((x^2-1)
	translate_x += brojac;
	translate_y += -1*(brojac*brojac-4);
	

	jump = false;
	glutPostRedisplay();

	if(timer_active){
		timer_active = 1;
		glutTimerFunc(20, on_timer, value);
	}
}