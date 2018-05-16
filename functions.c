#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "functions.h"
#include "callbacks.h"

//add the other modes after
#define ACTIVE 2
#define ACTIVE_CAVE 4
extern int GAME_MODE;

extern float ball_radius;

extern float window_width;
extern float window_height;
extern int visine_levo[50];
extern int visine_desno[50];

extern float spike_width_left;
extern float spike_width_right;
	//pomenljive vezane za velicinu spikesa
extern int spike_base;
extern int spike_height;

extern float token_width;
extern float token_height;
extern float token_radius;


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
		glTranslatef(-spike_width_left, height, 0);
		glRotatef(90, 0, 1, 0);
	}
	else{
		glTranslatef(spike_width_right, height, 0);
		glRotatef(-90, 0, 1, 0);
	}
	glutSolidCone(spike_base, spike_height, 20, 8);
	glPopMatrix();
}

void draw_spike_wall(const char *side, int difficulty_level, bool change){
	int i;
	int j;
	bool is_touching;
	int rand_num;
	float current_height;

	if(GAME_MODE == ACTIVE_CAVE){
		//first, we drop all the heights down
		for(i = 0; i < (change ? difficulty_level : difficulty_level-1); i++){
			if(strcmp("left", side) == 0){
				visine_levo[i] -= 2;
			}
			else {
				visine_desno[i] -= 2;
			}
		}

		//here we're using the change variable as an indicator 
		//that we should add another spike
		if(change){
			is_touching = false;
			do{
				is_touching = false;
				rand_num = (window_height/spike_base) * rand()/RAND_MAX; // 0-max spikes to fit screen
				
				current_height = rand_num*spike_base-spike_base/2+window_height/2;
				if(strcmp("left", side) == 0){
					for(j = 0; j < difficulty_level; j++){
						if(spike_collision(current_height, visine_levo[j]))
							is_touching = true;
						}
					}
				else {
					for(j = 0; j < difficulty_level; j++){
						if(spike_collision(current_height, visine_desno[j]))
							is_touching = true;
					}
				}
			}while(is_touching);
			
			if(strcmp("left", side) == 0)
				visine_levo[difficulty_level] = current_height;
			else
				visine_desno[difficulty_level] = current_height;

			//draw_new_spike(side, current_height);
		}

		//finally, if one of the spikes is below the screen,
		//we draw it back above the screen
		//also, draw the spikes
		for(i = 0; i < difficulty_level; i++){
			if(!strcmp("left", side)){
				if(visine_levo[i] < -window_height/2+spike_base/2){
					is_touching = false;
					do{
						is_touching = false;
						rand_num = (window_height/spike_base) * rand()/RAND_MAX; // 0-max spikes to fit screen
						
						current_height = rand_num*spike_base-spike_base/2+window_height/2;
						for(j = 0; j < difficulty_level; j++){
							if(j==i) continue; //skips itself
							if(spike_collision(current_height, visine_levo[j]))
								is_touching = true;
						}
					}while(is_touching);
			
					visine_levo[i] = current_height;
				}
				draw_new_spike(side, visine_levo[i]);
			}
			else{
				if(visine_desno[i] < -window_height/2+spike_base/2){
					is_touching = false;
					do{
						is_touching = false;
						rand_num = (window_height/spike_base) * rand()/RAND_MAX; // 0-max spikes to fit screen
						
						current_height = rand_num*spike_base-spike_base/2+window_height/2;
						for(j = 0; j < difficulty_level; j++){
							if(j==i) continue; //skips itself
							if(spike_collision(current_height, visine_desno[j]))
								is_touching = true;
						}
					}while(is_touching);
			
					visine_desno[i] = current_height;
				}
				draw_new_spike(side, visine_desno[i]);
			}
		}

	}
	else if(GAME_MODE == ACTIVE){
		if(change){
			srand(time(NULL));

			for(i = 0; i < difficulty_level; i++){
				is_touching = false;
				//number of spikes we need - 
				//window height / spike base

				//keep picking a random number
				//until the spike is not touching
				//another spike
				do{
					is_touching = false;
					rand_num = (window_height/spike_base) * rand()/RAND_MAX; // 0-max spikes to fit screen
					
					current_height = rand_num*spike_base-spike_base/2-window_height/2;

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
}

//returns true if the ball is touching the spike given as argument
bool ball_spike_collision(float pos_x, float pos_y, float visina, char *side){

	//for now, lets assume that spikes are in box-shape
	//TODO: calc the collision with the actual shape
	//for now, spike base 20 height 25 ball radious 20 -- all this will be through global variables
	if((pos_x > -window_width/2 + 25 + 20)&&(pos_x < window_width/2 - 25 - 20))
		return false;
	if((pos_y+25 < 0 && visina > 0) || (pos_y > 0 && visina+25 < 0)){
		//not on the same vertical side
		return false;
	}
	if((!strcmp("left", side) && (pos_x < 0)) || (strcmp("left", side) && (pos_x > 0))) {
		//chance of collision as the ball and the spike are on the same side
		if(abs(pos_y) - 20 > abs(visina) || abs(pos_y) + 20 < abs(visina) ){
			//ball completely above or below spike
			return false;
		}
		else{
			if(abs(pos_y) < abs(visina)+3 && abs(pos_y) > abs(visina)-3)
				return true;
			//gives a slightly better collision
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


int parametar = 10; //for rotation
void draw_token(float token_radius, float height, float width){
	
	glEnable(GL_LIGHTING);

	//cylinder
	glPushMatrix();	
	glTranslatef(width, height, 0); 
	glRotatef(parametar, 0, 1, 0);
	

	glColor3f(0.0, 0.7, 0.7);
	
	gluCylinder(gluNewQuadric(), token_radius, token_radius, 20, 20, 20);	
	glPopMatrix();

	//top disc
	glPushMatrix();
	glColor3f(0.0, 0.5, 0.5);
	glTranslatef(width, height, 20);	//0 and 20 should be global variables (z coordinate)
	glRotatef(parametar, 0, 1, 0);
	
	gluDisk(gluNewQuadric(), 5, token_radius, 20, 20);
	glPopMatrix();

	//botom disc
	glPushMatrix();
	glColor3f(0.0, 0.5, 0.5);
	glTranslatef(width, height, 0);	//0 and 20 should be global variables (z coordinate)
	glRotatef(parametar, 0, 1, 0);
		
	gluDisk(gluNewQuadric(), 5, token_radius, 20, 20);
	glPopMatrix();

	parametar++;
}

bool ball_token_collision(float pos_x, float pos_y, float token_height, float token_width){
	if((token_width < 0 && pos_x < 0) || (token_width > 0 && pos_x > 0)) {
		//same side
		if(pos_x - token_width > -(ball_radius + token_radius) && pos_x - token_width < (ball_radius + token_radius)){ 
			if(pos_y - token_height > -(ball_radius + token_radius) && pos_y - token_height < (ball_radius + token_radius)){
				printf("token ball collision: %f %f - %f %f\n", pos_x,pos_y,token_width,token_height);
				return true;
			}
		}
	}

	return false;
}

void draw_rand_token(float token_radius, float height, float width, bool collision){
	if(collision){
		float rand_num = (window_height/20) * rand()/RAND_MAX; // taken from spike function
				
		float current_height = rand_num*20-10-window_height/2;

		draw_token(token_radius, current_height, -width);

		token_width = -width;
		token_height = current_height;
	}
	else{
		draw_token(token_radius, height, width);
	}
}

void drawBitmapText(char *string,float x,float y,float z){
	//taken from http://www.codersource.net/2011/01/27/displaying-text-opengl-tutorial-5/  
	char *c = string;
	int i;
	glRasterPos3f(x, y,z);

	for (i = 0; c[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c[i]);
	}
}


//lists and updates highscores, makes the file if one doesn't exist
void highscores(int current_score){

	int score1, score2, score3;

	//opening the file as both a stream and a descriptor
	//to take advantage of the functions
	int fd = open("highscores.txt", O_CREAT | O_RDWR, 0600);
	if(fd == -1){
		printf("error opening file\n");
		exit(EXIT_FAILURE);
	}

	FILE *f = fdopen(fd, "r+");
	if(f == NULL){
		printf("fdopen failed\n");
		exit(EXIT_FAILURE);
	}

	//taken from OS 
	struct stat fInfo;
	if(fstat(fd, &fInfo) == -1){
		printf("fstat failed\n");
		exit(EXIT_FAILURE);
	}
	int file_size = fInfo.st_size;

	if(file_size == 0){
		score1 = 100;
		score2 = 50;
		score3 = 25;
	}
	else{
		fscanf(f, "%d %d %d", &score1, &score2, &score3);
	}

	if(current_score > score1){
		score3 = score2;
		score2 = score1;
		score1 = current_score;
	}
	else if(current_score > score2){
		score3 = score2;
		score2 = current_score;
	}
	else if(current_score > score3){
		score3 = current_score;
	}

	//erase previous scores
	if(ftruncate(fd, 0) == -1){
		printf("ftruncate failed\n");
		exit(EXIT_FAILURE);
	}
	//reset file offset to the beginning
	if(lseek(fd, 0, SEEK_SET) == -1){
		printf("lseek failed\n");
		exit(EXIT_FAILURE);
	}

	fprintf(f, "%d %d %d", score1, score2, score3);

	char scores[30];
	sprintf(scores, "1st: %d, 2nd: %d, 3rd: %d", score1, score2, score3);
	drawBitmapText(scores, -window_width/2+30, 0, 0);

	fclose(f);
	close(fd);
}