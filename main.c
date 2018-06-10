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

int GAME_MODE; //koji ekran se prikazuje


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

	//promenljive vezane za tok igre
int score;
int lives;

GLuint names[3];
void initTexture();

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

	initTexture();
	
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

	//spikes are equilateral triangles
	spike_base = 20;
	spike_height = spike_base * sqrt(3) / 2;
	
	ball_radius = 20;
	token_radius = 15;

	score = 0;
	lives = 2;

	glutMainLoop();
	return 0;
}

//taken from class
void initTexture(){
	Image *image;

    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    image = image_init(0, 0);

    glGenTextures(3, names);

    image_read(image, "brick.bmp");

    glBindTexture(GL_TEXTURE_2D, names[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    image_done(image);
}

