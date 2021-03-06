#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "functions.h"
#include "callbacks.h"
#include "image.h"

#define SPACEBAR 32
#define ESC 27
#define START 1
#define ACTIVE 2
#define END 3
#define ACTIVE_CAVE 4

extern int GAME_MODE;

extern int timer_active;
extern float window_width;
extern float window_height;

	//promenljive vezane za lokaciju sfere
extern float brojac;
extern float translate_y;
extern float translate_x;
extern int collision;
extern bool jump;
extern float ball_radius;

	//promenljive vezane za lokaciju spikesa
int difficulty_level = 1;
bool wall = false;
int visine_levo[50];
int visine_desno[50];
extern float spike_width_left;
extern float spike_width_right;
	//pomenljive vezane za velicinu spikesa
extern int spike_base;
extern int spike_height;

	//promenljive vezane za token
extern float token_width;
extern float token_height;
extern float token_radius;

	//promenljive vezane za tok igre
extern int score;
extern int lives;

GLuint names[3];
extern bool textures_initialised;

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

    if(GAME_MODE == START){
    	printStartScreen();
    }

    if(GAME_MODE == ACTIVE || GAME_MODE == ACTIVE_CAVE){
		
		if(!textures_initialised){
			initTexture();
			textures_initialised = true;
		}
    	displayTextures();

    	//pozicioniranje objekata
			//glavna lopta
		glColor3f(0.7, 0, 0);
		glPushMatrix();
		glTranslatef(translate_x, translate_y, 0);
		glutSolidSphere(ball_radius, ball_radius/2, ball_radius/2);
		glPopMatrix();
		
		//provera za collision
		int i;
		for(i = 0; i < difficulty_level; i++){
			if(ball_spike_collision(translate_x, translate_y, visine_levo[i], "left")){
				printf("kolizija sa spikeom %d, posx: %.4f posy: %.4f visina: %d\n", 
					i, translate_x, translate_y, visine_levo[i]);
				lives--;
			}
		}
		for(i = 0; i < difficulty_level; i++){
			if(ball_spike_collision(translate_x, translate_y, visine_desno[i], "right")){
				printf("kolizija sa spikeom %d, posx: %.4f posy: %.4f visina: %d\n", 
					i, translate_x, translate_y, visine_levo[i]);
				lives--;
			}
		}

		if(lives <= 0){
			GAME_MODE = END;
		}
		
		if(collision){
			draw_spike_wall("right", difficulty_level, (GAME_MODE == ACTIVE_CAVE ? true : false));
			draw_spike_wall("left", difficulty_level, wall);
		}
		else{
			draw_spike_wall("left", difficulty_level-1, (GAME_MODE == ACTIVE_CAVE ? true : false));
			draw_spike_wall("right", difficulty_level, wall);
		}

		//if there was a collision, draw the token on the other wall
		bool token_collision = ball_token_collision(translate_x, translate_y, token_height, token_width);
		draw_rand_token(token_radius, token_height, token_width, token_collision);
		if(token_collision){
			score++;
		}

		wall = false;

		//draw the score
		//score += difficulty_level += tokens collected
		char score_text[10];
		char lives_text[10];
		sprintf(score_text, "score: %d", score);
		sprintf(lives_text, "lives: %d", lives);
		drawBitmapText(score_text, window_width/2-100, -window_height/2 + 30, 0);
		drawBitmapText(lives_text, window_width/2-100, -window_height/2 + 10, 0);
	}

	if(GAME_MODE == END){
		glColor3f(0.7, 0, 0);
		highscores(score);
		timer_active = 0;
	}

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
				GAME_MODE = ACTIVE;
				spike_width_left = window_width/2 + spike_height;
				spike_width_right = window_width/2 + spike_height;

				difficulty_level = 1;
				brojac = 0;
				translate_y = 0;
				translate_x = 0;
				collision = 0; //nije doslo do kolizije
				jump = false;
				token_width = -window_width/2 + 50;
				token_height = 50;
				lives = 2;
				score = 0;

				timer_active = 1;
				glutTimerFunc(20, on_timer, 0);
			}
			break;
		case 'c':
		case 'C':
			if(!timer_active){
				init_heights();
				GAME_MODE = ACTIVE_CAVE;
				spike_width_left = window_width/2;
				spike_width_right = window_width/2;

				difficulty_level = 1;
				brojac = 0;
				translate_y = 0;
				translate_x = 0;
				collision = 0; //nije doslo do kolizije
				jump = false;
				token_width = -window_width/2 + 50;
				token_height = 50;
				lives = 2;
				score = 0;

				timer_active = 1;
				glutTimerFunc(20, on_timer, 0);
			}
			break;
		case SPACEBAR:
			if(!timer_active){
				timer_active = 1;
				token_width = -1* window_width/2 + 3*token_radius;
				glutTimerFunc(20, on_timer, 0);
			}
			else{
				jump = true;
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

	if(GAME_MODE == START){

	}
	else if(GAME_MODE == ACTIVE || GAME_MODE == ACTIVE_CAVE){

		//keeps you on the screen
		if(translate_y > window_height/2 - ball_radius){
			jump = false;
		}
		else if(translate_y < -window_height/2 + ball_radius){
			jump = true;
		}

		//if the ball touched the sides of the screen
		if(translate_x > window_width/2 - 20){
			if(GAME_MODE == ACTIVE) 
				spike_width_left = window_width/2 + spike_height;
			collision = 1;
			brojac = -brojac;
			wall = true;
			score++;
		}
		else if(translate_x < -window_width/2 + 20){
			if(GAME_MODE == ACTIVE)				
				spike_width_right = window_width/2 + spike_height;
			collision = 0;
			brojac = -brojac;
			wall = true;
			if(difficulty_level < 7) difficulty_level += 1;
			score++;
		}

		//HACK
		if(GAME_MODE == ACTIVE_CAVE && difficulty_level == 1)
			wall = true;

		//spike movement
		spike_width_left -= (spike_width_left > window_width/2) ? 0.4 : 0.0;
		spike_width_right -= (spike_width_right > window_width/2) ? 0.4 : 0.0;

		//zasto? da bi krenuo u skok ispocetka, ali sa tog
		//istog mesta
		if(jump) {brojac = 0;}

		brojac = (collision==0) ? brojac+0.1 : brojac-0.1;

		//ball movement function
		//try to plot  -((x^2-4)
		translate_x += brojac*10/6;	//this multiplication helps the movement
		translate_y += -1*((brojac)*(brojac)-4);
		

		jump = false;
	}

	if(GAME_MODE == END){
		timer_active = 0;
	}

	glutPostRedisplay();

	if(timer_active){
		timer_active = 1;
		glutTimerFunc(20, on_timer, value);
	}
}

void displayTextures(){
	makeWall(0);
}
//taken from class
void makeWall(int i){
	glPushMatrix();

		glTranslatef(75/2*1.5, 0, 0);
		glRotatef(90, 0, 1, 0);
		glTranslatef(-75*2.25/2, -75/4 + 7, 0);

	    glBindTexture(GL_TEXTURE_2D, names[i]);
	    glBegin(GL_QUADS);
	        glNormal3f(0, 0, 1);

	        glTexCoord2f(0, 0);
	        glVertex3f(0, 0, 0);

	        glTexCoord2f(75*2.25/10, 0);
	        glVertex3f(75*2.25, 0, 0);

	        glTexCoord2f(75*2.25/10, 75/20);
	        glVertex3f(75*2.25, 75/2, 0);

	        glTexCoord2f(0, 75/20);
	        glVertex3f(0, 75/2, 0);
	    glEnd();

	    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    glPushMatrix();
		glTranslatef(-75/2*1.5, 0, 0);
		glRotatef(90, 0, 1, 0);
		glTranslatef(-75*2.25/2, -75/4 + 7, 0);

	    glBindTexture(GL_TEXTURE_2D, names[i]);
	    glBegin(GL_QUADS);
	        glNormal3f(0, 0, 1);

	        glTexCoord2f(0, 0);
	        glVertex3f(0, 0, 0);

	        glTexCoord2f(75*2.25/12, 0);
	        glVertex3f(75*2.25, 0, 0);

	        glTexCoord2f(75*2.25/12, 75/24);
	        glVertex3f(75*2.25, 75/2, 0);

	        glTexCoord2f(0, 75/24);
	        glVertex3f(0, 75/2, 0);
	    glEnd();

	    glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-75*1.5/2, -75/4 + 7, -10);

	    glBindTexture(GL_TEXTURE_2D, names[i]);
	    glBegin(GL_QUADS);
	        glNormal3f(0, 0, 1);

	        glTexCoord2f(0, 0);
	        glVertex3f(-window_width/2, -window_height/2, 0);

	        glTexCoord2f(75*1.5/12, 0);
	        glVertex3f(window_width, -window_height/2, 0);

	        glTexCoord2f(75*1.5/12, 75/24);
	        glVertex3f(window_width, window_height, 0);

	        glTexCoord2f(0, 75/24);
	        glVertex3f(-window_width/2, window_height, 0);
	    glEnd();

	    glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
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