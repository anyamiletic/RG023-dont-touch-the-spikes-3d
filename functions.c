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

extern float spike_width_left;
extern float spike_width_right;


//lista koja cuva visine spikeova, tj rand brojeve
//iz kojih se saznaje visina
void init_heights(){
	visine_levo[0] = 0;
	visine_desno[0] = 0;
}

#define Cos(th) cos(M_PI/180*(th))
#define Sin(th) sin(M_PI/180*(th))



void draw_new_spike(const char *side, float height){
	glEnable(GL_LIGHTING);
	

	glColor3f(0, 0, 0.7);

	glPushMatrix();
	if(strcmp("left", side) == 0){
		glTranslatef(-spike_width_left, height, 0);
		glRotatef(90, 0, 1, 0);
	}
	else{
		glTranslatef(spike_width_right, height, 0);
		glRotatef(-90, 0, 1, 0);
	}
	glutSolidCone(20, 25, 20, 8);
	glPopMatrix();
}

void draw_spike_wall(const char *side, int difficulty_level, bool change){
	int i;
	int j;

	if(change){
		srand(time(NULL));

		int rand_broj;
		
		float current_height;

		printf("difficulty_level: %d______\n", difficulty_level);
		for(i = 0; i < difficulty_level; i++){
			bool is_touching = false;
			//number of spikes we need - 
			//window height / spike base
			do{
				is_touching = false;
				rand_broj = (window_height/20) * rand()/RAND_MAX; // 0-max spikes to fit screen
				
				current_height = rand_broj*20-10-window_height/2;
				printf("%4.4f -- %d\n", current_height, i);

				if(strcmp("left", side) == 0){
					for(j = 0; j < i; j++){
						if(spike_collision(current_height, visine_levo[j]))
							is_touching = true;
					}
				}
				else {
					for(j = 0; j < i; j++){
						if(spike_collision(current_height, visine_desno[j]))
							is_touching = true;
					}
				}
				printf("%s\n", is_touching ? "true":"false");
			}while(is_touching);
			

			if(strcmp("left", side) == 0)
				visine_levo[i] = current_height;
			else
				visine_desno[i] = current_height;

			draw_new_spike(side, current_height);
		}
	}
	else{
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
			return true;
		}
	}

	return false;
}

bool spike_collision(float height1, float height2){
	//again, these will later be in global variable, but for now, spike width(base) is 20
	if(height1 - height2 < 30 && height1 - height2 > -30)
		return true;
	else
		return false;
}


int parametar = 10;
void draw_token(float token_radius, float height){
	//TODO: draw a sphere with radius
	//token_radius, and cut itwith a 
	//cutting plane to achieve a 3D disc

	glEnable(GL_LIGHTING);

	glPushMatrix();	
	glRotatef(parametar, 0, 1, 0);
	glTranslatef(0, 40, 0);


	glColor3f(0.0, 0.7, 0.7);
	
	gluCylinder(gluNewQuadric(), 20, 20, 20, 20, 20);
	parametar++;
	
	glPopMatrix();
}