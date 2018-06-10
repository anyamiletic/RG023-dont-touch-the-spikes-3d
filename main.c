#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "callbacks.h"
#include "functions.h"
#include "image.h"

#define START 1
#define ACTIVE 2
#define END 3
#define ACTIVE_CAVE 4

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
float ball_radius;

	//promenljive vezane za lokaciju spikesa
float spike_width_left;
float spike_width_right;
	//promenljive vezane za velicinu spikesa
int spike_base;
int spike_height;

	//promenljive vezane za token
float token_width;
float token_height;
float token_radius;
int token_rotation;

	//promenljive vezane za tok igre
int score;
int lives;
int GAME_MODE; //koji ekran se prikazuje

bool textures_initialised;


int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(400, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("ImeAplikacije");

	glutDisplayFunc(on_display);
	glutMouseFunc(on_mouse);
	glutKeyboardFunc(on_keyboard);
	glutReshapeFunc(on_reshape);


	glEnable(GL_DEPTH_TEST);
	glClearColor(0.75, 0.75, 0.75, 0);

	//inicijalizacija globalnih promenljivih
	GAME_MODE = START;

	timer_active = 0;
	brojac = 0;
	translate_y = 0;
	translate_x = 0;
	collision = 0; //nije doslo do kolizije
	jump = false;
	spike_width_left = 0;
	spike_width_right = 200;
	token_width = -150;
	token_height = 50;
	token_rotation = 10;

	//spikeovi su jednakostranicni trouglovi
	spike_base = 20;
	spike_height = spike_base * sqrt(3) / 2;
	
	ball_radius = 20;
	token_radius = 15;

	score = 0;
	lives = 2;

	textures_initialised = false;

	glutMainLoop();
	return 0;
}



