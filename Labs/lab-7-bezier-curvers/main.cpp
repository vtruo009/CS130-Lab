// Name: Van Truong
// Quarter, Year: Winter21
// Lab: Bezier Curve
//
// This file is to be modified by the student.
// main.cpp
////////////////////////////////////////////////////////////
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <vector>
#include <cstdio>
#include <math.h>
#include "vec.h"
#include <iostream>

using namespace std;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

vector<vec2> control_points;

float factorial(int n) {
	float product = 1;
	for (int i = 1; i <= n; ++i) {
		product *= i;
	}
	return product;
}

float combination(int n, int i) {
	return factorial(n) / (factorial(i) * factorial(n - i));
}

float binomial(float n, float k, float t) {
	// for n control points, the degree is n - 1
	return combination(n - 1, k) * pow(t, k) * pow(1 - t, n - 1 - k);
}

void GL_render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
    
    glBegin(GL_LINE_STRIP);

    // TODO: just for example, you will need to change this.
    /*glVertex2f(-.5f,-.5f);
    glVertex2f(.5f,-.5f);
    glVertex2f(.5f,.5f);
    glVertex2f(-.5f,.5f);*/
    
    if (control_points.size() >= 2) {
		for (float t = 0.01; t <= 1.0; t += 0.01) {
			vec2 curve;
			for (unsigned int k = 0; k < control_points.size(); ++k) {
				curve += binomial(control_points.size(), k, t) * control_points.at(k);
			}
			glColor3f(1.0f,0.0f,0.0f);
			glVertex2f(curve[0], curve[1]);
		}
		
	}

    glEnd();
    
    
    glFlush();
}

void GL_mouse(int button,int state,int x,int y)
{
    y=WINDOW_HEIGHT-y;
    GLdouble mv_mat[16];
    GLdouble proj_mat[16];
    GLint vp_mat[4];
    glGetDoublev(GL_MODELVIEW_MATRIX,mv_mat);
    glGetDoublev(GL_PROJECTION_MATRIX,proj_mat);
    glGetIntegerv(GL_VIEWPORT,vp_mat);

    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        double px,py,dummy_z; // we don't care about the z-value but need something to pass in
        gluUnProject(x,y,0,mv_mat,proj_mat,vp_mat,&px,&py,&dummy_z);
        // TODO: the mouse click coordinates are (px,py).
        vec2 coordinate;
        coordinate[0] = px;
        coordinate[1] = py;
        control_points.push_back(coordinate);
		glutPostRedisplay();
    }
}

//Initializes OpenGL attributes
void GLInit(int* argc, char** argv)
{
    glutInit(argc, argv);
    //glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    //glMatrixMode(GL_PROJECTION_MATRIX);
    //glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
    glutCreateWindow("CS 130 - <Van Truong>");
    glutDisplayFunc(GL_render);
    glutMouseFunc(GL_mouse);
}

int main(int argc, char** argv)
{
    GLInit(&argc, argv);
    glutMainLoop();
    return 0;
}
