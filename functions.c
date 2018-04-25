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

//returns true if the ball is touching the spike given as argument
bool ball_spike_collision(float pos_x, float pos_y, float spike_height, char *side){

	//for now, lets assume that spikes are in box-shape
	//TODO: calc the collision with the actual shape
	//for now, spike base 20 height 25 ball radious 20 -- all this will be through global variables
	if((pos_x > -window_width/2 + 25 + 20)&&(pos_x < window_width/2 - 25 - 20))
		return false;
	if((pos_y+25 < 0 && spike_height > 0) || (pos_y > 0 && spike_height+25 < 0)){
		//not on the same vertical side
		return false;
	}
	if(((strcmp("left", side) == 0) && (pos_x < 0)) || ((strcmp("left", side) != 0) && (pos_x > 0))) {
		//chance of collision as the ball and the spike are on the same side
		if(abs(pos_y) - 20 > abs(spike_height) || abs(pos_y) + 20 < abs(spike_height) ){
			//ball completely above or below spike
			return false;
		}
		else{
			printf("U FJI: posx: %.4f posy: %.4f visina: %.4f\n", 
				pos_x, pos_y, spike_height);
			return true;
		}
	}

	return false;
}