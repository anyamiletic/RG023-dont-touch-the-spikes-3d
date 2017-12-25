#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_mouse(int button, int state, int x, int y);
static void on_timer(int value);

//deklaracije globalnih promenljivih
int timer_active;
int ugao;
float visina;
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
	ugao = 0;
	visina = 0;
	//

	glutMainLoop();
	return 0;
}

static void on_display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)3/5, 1, 50);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//pozicioniranje kamere
	gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
	//

	//pozicioniranje objekata

		//glavna lopta
	glColor3f(1, 0, 0);
	glutWireSphere(1.5, 20, 20);

		//piramide - spikes
	glColor3f(0, 0, 0);
	glTranslatef(-5, 0, 0);
	glRotatef(ugao, 0, 0, 1);
	glutWireTetrahedron();
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

}

static void on_timer(int value){
	if(value != 0)
		return;

	//menjanje vrednosti promenljivih koje ucestvuju u animaciji
	ugao++;
	
	//

	glutPostRedisplay();

	if(timer_active){
		timer_active = 1;
		glutTimerFunc(20, on_timer, value);
	}
}