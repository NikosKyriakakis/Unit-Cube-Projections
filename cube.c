#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/freeglut.h>
#include <GL/glut.h>

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define POINTS	3
#define DIMENSION 4.0
#define DEGREES 4
#define AXIS_LENGTH 1.0

enum vertexLetter {
	A, B, C, D, E, F, G, H
};

static GLfloat vertices[][POINTS] = {
	{ 0.5, 0.5, 0.5 },
	{ -0.5, 0.5, 0.5 },
	{ -0.5, -0.5, 0.5 },
	{ 0.5, -0.5, 0.5 },
	{ 0.5, 0.5, -0.5 },
	{ -0.5, 0.5, -0.5 },
	{ -0.5, -0.5, -0.5 },
	{ 0.5, -0.5, -0.5 }
};

static GLint viewAngleAzimuth = 10;
static GLint viewAngleElevation = 15;
static GLint aspectRatio = 1;
static GLint fieldOfPerspective = 55;

static void drawAxices(void)
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    	glVertex3f(0.0, 0.0, 0.0);
    	glVertex3f(AXIS_LENGTH, 0.0, 0.0);
    	glVertex3f(0.0, 0.0, 0.0);
    	glVertex3f(0.0, AXIS_LENGTH, 0.0);
    	glVertex3f(0.0, 0.0, 0.0);
    	glVertex3f(0.0, 0.0, AXIS_LENGTH);
    glEnd();
    
    glRasterPos3f(AXIS_LENGTH, 0.0, 0.0);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)"X");
    glRasterPos3f(0.0, AXIS_LENGTH, 0.0);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)"Y");
    glRasterPos3f(0.0, 0.0, AXIS_LENGTH);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)"Z");
}

static void setEyePosition(void)
{
    glRotatef(viewAngleElevation, 1, 0, 0);
    glRotatef(viewAngleAzimuth, 0, 1, 0);
}

static void project(void) 
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(-DIMENSION * aspectRatio, DIMENSION * aspectRatio, 
			-DIMENSION, DIMENSION, -DIMENSION, DIMENSION);
    	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void specialKeyPress(int keyCode, int x, int y)
{
	switch(keyCode) {
		case GLUT_KEY_RIGHT:
			viewAngleAzimuth += DEGREES;
			break;
		case GLUT_KEY_LEFT:
			viewAngleAzimuth -= DEGREES;
			break;
		case GLUT_KEY_UP:
			viewAngleElevation += DEGREES;
			break;
		case GLUT_KEY_DOWN:
			viewAngleElevation -= DEGREES;
			break;
	}
	
	viewAngleAzimuth %= 360;
	viewAngleElevation %= 360;
	
	project();
	glutPostRedisplay();
}

static void drawCubePerspective(void)
{
	glBegin(GL_QUADS);
	
	// front => ABCD red 
	glColor3f(1.0, 0.0, 0.0);
	glVertex3fv(vertices[A]);
	glVertex3fv(vertices[B]);
	glVertex3fv(vertices[C]);
	glVertex3fv(vertices[D]);
	
	// back => FEHG yellow 
	glColor3f(1.0, 1.0, 0.0);
	glVertex3fv(vertices[F]);
	glVertex3fv(vertices[E]);
	glVertex3fv(vertices[H]);
	glVertex3fv(vertices[G]);
	
	// right => EADH green 
	glColor3f(0.0, 1.0, 0.0);
	glVertex3fv(vertices[E]);
	glVertex3fv(vertices[A]);
	glVertex3fv(vertices[D]);
	glVertex3fv(vertices[H]);
	
	// left => BFGC blue 
	glColor3f(0.0, 0.0, 1.0);
	glVertex3fv(vertices[B]);
	glVertex3fv(vertices[F]);
	glVertex3fv(vertices[G]);
	glVertex3fv(vertices[C]);
	
	// top => EFBA turquoise 
	glColor3f(0.0, 1.0, 1.0);
	glVertex3fv(vertices[E]);
	glVertex3fv(vertices[F]);
	glVertex3fv(vertices[B]);
	glVertex3fv(vertices[A]);
	
	// bottom => DCGH pink 
	glColor3f(1.0, 0.0, 1.0);
	glVertex3fv(vertices[D]);
	glVertex3fv(vertices[C]);
	glVertex3fv(vertices[G]);
	glVertex3fv(vertices[H]);
		
	glEnd();
}

static void drawCubeOblique(void)
{
	glBegin(GL_QUADS);
	
	// top => EFBA pink 
	glColor3f(1.0, 0.0, 1.0);
	glVertex3fv(vertices[E]);
	glVertex3fv(vertices[F]);
	glVertex3fv(vertices[B]);
	glVertex3fv(vertices[A]);
	
	// right => EADH green 
	glColor3f(0.0, 1.0, 0.0);
	glVertex3fv(vertices[E]);
	glVertex3fv(vertices[A]);
	glVertex3fv(vertices[D]);
	glVertex3fv(vertices[H]);
	
	// front => ABCD red 
	glColor3f(1.0, 0.0, 0.0);
	glVertex3fv(vertices[A]);
	glVertex3fv(vertices[B]);
	glVertex3fv(vertices[C]);
	glVertex3fv(vertices[D]);
		
	glEnd();
}

static void reshape(const int width, const int height)
{
	aspectRatio = 1;
	if (height > 0)
		aspectRatio = (GLfloat)(width / height);
	glViewport(0, 0, width, height);
	project();
}

static void cavalierDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat identityMatrix[16]; 
    glGetFloatv(GL_MODELVIEW_MATRIX, identityMatrix);
    
    const GLfloat cavalierAngle = (M_PI / 180.0) * (-45.0);
    const GLfloat projectionLength = 1 / tan(-cavalierAngle);
    
    identityMatrix[8] = -cos(cavalierAngle);
    identityMatrix[9] = sin(cavalierAngle);
            
    glLoadMatrixf(identityMatrix);
    drawCubeOblique();
    	
    glutSwapBuffers();
    glutReportErrors();
    glFlush();
}

static void cabinetDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat identityMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, identityMatrix);
    
    const GLfloat cabinetAngle = (M_PI / 180.0) * (-63.4);
	const GLfloat projectionLength = 1 / tan(-cabinetAngle);

    identityMatrix[8] = -cos(cabinetAngle) / 2.0;
    identityMatrix[9] = sin(cabinetAngle) / 2.0;
    
	glLoadMatrixf(identityMatrix);
    drawCubeOblique();
    
    glutSwapBuffers();
    glutReportErrors();
    glFlush();
}

static void perspectiveDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfPerspective, aspectRatio, 0.1, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -4.0);
	
	setEyePosition();
	drawCubePerspective();
	drawAxices();
	
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);	
	
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Perspective-Projection");
	glutDisplayFunc(perspectiveDisplay);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeyPress);
		
	glutInitWindowPosition(500, 0);
	glutCreateWindow("Cavalier-Projection");
  	glutDisplayFunc(cavalierDisplay);
  	glutReshapeFunc(reshape);
  	
	glutInitWindowPosition(1000, 0);
	glutCreateWindow("Cabinet-Projection");
  	glutDisplayFunc(cabinetDisplay);
  	glutReshapeFunc(reshape);
  	
	glutMainLoop();
	
	return EXIT_SUCCESS;
}
