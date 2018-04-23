#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "functions.h"
#include "callbacks.h"

extern float window_width;
extern float window_height;
extern int visine_levo[50];
extern int visine_desno[50];

//lista koja cuva visine spikeova, tj rand brojeve
//iz kojih se saznaje visina
void init_heights(){
	visine_levo[0] = 0;
	visine_desno[0] = 0;
}



void draw_new_spike(const char *side, float height){
	glEnable(GL_LIGHTING);
	

	glColor3f(0, 0, 0.7);

	glPushMatrix();
	if(strcmp("left", side) == 0){
		glTranslatef(-window_width/2, height, 0);
		glRotatef(90, 0, 1, 0);
	}
	else{
		glTranslatef(window_width/2, height, 0);
		glRotatef(-90, 0, 1, 0);
	}
	glutSolidCone(20, 25, 20, 8);
	glPopMatrix();
}

void draw_spike_wall(const char *side, int difficulty_level, bool change){
	if(change){
		srand(time(NULL));

		int i = 0;
		int rand_broj;

		printf("difficulty_level: %d______\n", difficulty_level);
		for(i = 0; i < difficulty_level; i++){
			//number of spikes we need - 
			//window height / spike base
			rand_broj = (window_height/20) * rand()/RAND_MAX; // 0-max spikes to fit screen
			printf("%d\n", rand_broj);
			if(strcmp("left", side) == 0)
				visine_levo[i] = rand_broj*20-10-window_height/2;
			else
				visine_desno[i] = rand_broj*20-10-window_height/2;

			draw_new_spike(side, rand_broj*20-10-window_height/2);
		}
	}
	else{
		int i;
		for(i = 0; i < difficulty_level; i++){
			if(strcmp("left", side) == 0)
				draw_new_spike(side, visine_levo[i]);
			else
				draw_new_spike(side, visine_desno[i]);
		}
	}
}