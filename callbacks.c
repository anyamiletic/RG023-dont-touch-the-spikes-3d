#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "functions.h"
#include "callbacks.h"

#define SPACEBAR 32
#define ESC 27


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

	//promenljive vezane za lokaciju spikesa
int difficulty_level = 1;
bool wall = false;
int visine_levo[50];
int visine_desno[50];
extern float spike_width_left;
extern float spike_width_right;

	//promenljive vezane za token
extern float token_width;
extern float token_height;

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

	
	//u oridjidji igrici se spikeovi ne 
	//pomeraju, samo se svaki put ponovo stvaraju
	
	//provera za collision, treba obrisati posle!
	int i;
	for(i = 0; i < difficulty_level; i++){
		if(ball_spike_collision(translate_x, translate_y, visine_levo[i], "left"))
			printf("kolizija sa spikeom %d, posx: %.4f posy: %.4f visina: %d\n", 
				i, translate_x, translate_y, visine_levo[i]);
	}
	for(i = 0; i < difficulty_level; i++){
		if(ball_spike_collision(translate_x, translate_y, visine_desno[i], "right"))
			printf("kolizija sa spikeom %d, posx: %.4f posy: %.4f visina: %d\n", 
				i, translate_x, translate_y, visine_levo[i]);
	}
	
	if(collision){
		draw_spike_wall("right", difficulty_level-1, false);
		draw_spike_wall("left", difficulty_level, wall);
	}
	else{
		draw_spike_wall("left", difficulty_level-1, false);
		draw_spike_wall("right", difficulty_level, wall);
	}

	
	bool token_collision = ball_token_collision("left", translate_x, translate_y, token_height, token_width);
	draw_rand_token(20, token_height, token_width, token_collision);

	wall = false;

	glutSwapBuffers();
}

void on_keyboard(unsigned char key, int x, int y){
	switch(key){
		case ESC:
			exit(EXIT_SUCCESS);
		case 'g':
		case 'G':
			if(!timer_active){
				init_heights();
				spike_width_left = window_width/2 + 25;
				spike_width_right = window_width/2 + 25;
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
				/*znaci ovako postizemo da 
				se brzina ne povecava - 
				ne zovemo timer funkciju. genijalno.
				samo menjamo parametre
				RETARDE GLUPI */
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
	
	//TODO ne treba svaki put da se povecava difficulty_level
	if(translate_x > window_width/2 - 20){
		spike_width_left = window_width/2 + 25;
		collision = 1;
		brojac = -brojac;
		printf("%f\n", brojac);
		wall = true;
		if(difficulty_level < 10) difficulty_level += 1;
	}
	else if(translate_x < -window_width/2 + 20){				
		spike_width_right = window_width/2 + 25;
		collision = 0;
		brojac = -brojac;
		printf("%f\n", brojac);
		wall = true;
		if(difficulty_level < 10) difficulty_level += 1;
	}

	//spike movement
	spike_width_left -= (spike_width_left > window_width/2) ? 0.3 : 0.0;
	spike_width_right -= (spike_width_right > window_width/2) ? 0.3 : 0.0;

	//zasto? da bi krenuo u skok ispocetka, ali sa tog
	//istog mesta. nadam se.
	if(jump) {brojac = 0;}

	brojac = (collision==0) ? brojac+0.1 : brojac-0.1;	//aaaaaaaaaaaaaaaaaaa


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