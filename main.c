#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_mouse(int button, int state, int x, int y);
static void on_timer(int value);

//deklaracije globalnih promenljivih
int timer_active;


	//promenljive vezane za lokaciju sfere
int brojac;
float visina;
float sirina;
int collision;

	//promenljive vezane za lokaciju spikesa
float lift;

	//promenljive vezane za lokaciju kamere
float cam_pos;
//

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("ImeAplikacije");

	glutDisplayFunc(on_display);
	glutMouseFunc(on_mouse);
	glutKeyboardFunc(on_keyboard);
	glutReshapeFunc(on_reshape);


	glEnable(GL_DEPTH_TEST);
	glClearColor(0.75, 0.75, 0.75, 0);

	//inicijalizacija globalnih promenljivih
	timer_active = 0;
	brojac = 0;
	visina = 0;
	sirina = 0;
	collision = 0; //nije doslo do kolizije

	cam_pos = 0;

	lift = 0;
	//

	glutMainLoop();
	return 0;
}

void draw_new_spike(const char *side){
	glEnable(GL_LIGHTING);
	

	glColor3f(0, 0, 0.7);

	glPushMatrix();
	if(strcmp("left", side) == 0){
		glTranslatef(-6.8, 0, 0);
		glRotatef(90, 0, 1, 0);
	}
	else{
		glTranslatef(6.8, 0, 0);
		glRotatef(-90, 0, 1, 0);
	}
	glutSolidCone(1, 2, 20, 8);
	glPopMatrix();
}

static void on_display(void){
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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)3/5, 1, 50);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//pozicioniranje kamere
	gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
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
	glTranslatef(sirina, visina, 0);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();


	glTranslatef(0, -visina, 0);
		//piramide - spikes
	draw_new_spike("left");
	draw_new_spike("right");

	// glutWireTetrahedron();
	//glutWireCone(1, 2, 20, 8);

	//

	glutSwapBuffers();
}

static void on_keyboard(unsigned char key, int x, int y){
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
		case 's':
		case 'S':
			timer_active = 0;
			break;
	
	}
}

static void on_reshape(int width, int height){
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)width/height, 1, 20);
}

static void on_mouse(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON){
		if(!timer_active){
				timer_active = 1;
				glutTimerFunc(20, on_timer, 0);
		}
	}

	cam_pos = x/10;
}

static void on_timer(int value){
	if(value != 0)
		return;

	//menjanje vrednosti promenljivih koje ucestvuju u animaciji

	if(sirina > 6.8){
		collision = 1;
	}
	else if(sirina < -6.8){
		collision = 0;
	}

	brojac = (collision==0) ? brojac+1 : brojac -1;
	visina = sin(2*brojac*2*M_PI/180);
	sirina = brojac*2*M_PI/180;
		
	if(lift < -14)
		lift = 14;
	lift -= 0.04;
	//

	glutPostRedisplay();

	if(timer_active){
		timer_active = 1;
		glutTimerFunc(20, on_timer, value);
	}
}