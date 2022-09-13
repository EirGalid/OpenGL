#include<windows.h>
#include <GL/glui.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>

float xy_aspect;
int xs=30,ys=30,xf=300,yf=60;
int width=7;
int user_interface;

void myGlutReshape( int x, int y )
{
  xy_aspect = (float)x / (float)y;
  glViewport( 0, 0, x, y );
  glutPostRedisplay();
}

//function for setting pixel color
void pxcolor(int x, int y, float i)//i=intensity
{
    glBegin(GL_POINTS);
    i=1-i/255;
    glColor4f(1.0f,0.0f,0.0f,i); 
    glVertex2i(x,y);
    glEnd();
    glFlush();
}

float max(float a,float b)
{
    if(a>b)
        return a;
    else
        return b;
}

//modified bresenham
void plotAA()
{
    int x0=xs, y0=ys, x1=xf, y1=yf; 

//AA line width
 int dx = abs(x1-x0); 
 int sx; //slope value for x

 if (x0<x1)
    sx=1;
 else 
    sx=-1;

 int dy = abs(y1-y0);
 int sy; //slope value for y
 if(y0<y1)
    sy=1;
 else 
    sy=-1;

 int e1=dx-dy, e2, x2, y2; //e1=error1, e2=error2
 float ds=dx+dy; //distance sum=ds

 if (ds==0)
    ds=1;
 else 
    ds=sqrt((float)dx*dx+(float)dy*dy); //norm 2

 for (width=(width+1)/2; ; )//general loop for all line steps
 {
    //pixel loop
    pxcolor(x0, y0, max(0,255*(abs(e1-dx+dy)/ds-width+1))); //chooses the most dominant color based on the width and distance, adds 1 at the end in case of a zero denominator
    e2 = e1; 
    x2 = x0;  //quick swapping 

    if (2*e2>=-dx) //calculating x step
    { 
       for (e2+=dy, y2=y0; e2 < ds*width && (y1!=y2 || dx>dy); e2+=dx) //from y0 with a new error (e2), incrementing by dx 
            pxcolor(x0, y2+=sy, max(0,255*(abs(e2)/ds-width+1)));//change intensity based on the new error value
        if (x0==x1) 
            break;
        e2=e1;
        e1-=dy;
        x0+=sx;
    }
    
    if (2*e2<dy) //calculating y step
    { 
      for (e2=dx-e2; e2 < ds*width && (x1!=x2 || dx<dy); e2+=dy)
        pxcolor(x2+=sx, y0, max(0,255*(abs(e2)/ds-width+1)));
        if (y0==y1)
            break;
        e1+=dx;
        y0+=sy;
    } 
 } 
}

void MyInit()
{
    glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
    plotAA();
    glFlush();
}

int main(int argC,char *argV[])
{   
    glutInit(&argC,argV);
    glutInitWindowPosition(0, 0);
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glMatrixMode(GL_PROJECTION);
	user_interface = glutCreateWindow("Anti-Aliased Line Drawing");
	gluOrtho2D(0, 600, 0, 600);
    glutDisplayFunc(MyInit);
	glutReshapeFunc(myGlutReshape);

	//glEnable(GL_DEPTH_TEST); 

    GLUI *glui = GLUI_Master.create_glui("User Controls");
	GLUI_Spinner *linewidth_spinner=
    glui->add_spinner( "Line Width:", GLUI_SPINNER_INT, &width);
	linewidth_spinner->set_int_limits(1,60);
    GLUI_Spinner *x0_spinner=
	glui->add_spinner( "x0:", GLUI_SPINNER_INT, &xs);
    x0_spinner->set_int_limits(1,600);
    GLUI_Spinner *y0_spinner=
	glui->add_spinner( "y0:", GLUI_SPINNER_INT, &ys);
    y0_spinner->set_int_limits(1,600);
    GLUI_Spinner *x1_spinner=
	glui->add_spinner( "x1:", GLUI_SPINNER_INT, &xf);
    x1_spinner->set_int_limits(1,600);
    GLUI_Spinner *y1_spinner=
	glui->add_spinner( "y1:", GLUI_SPINNER_INT, &yf);
    y1_spinner->set_int_limits(1,600);
	glui->set_main_gfx_window(user_interface);
	GLUI_Master.set_glutIdleFunc(NULL);

	glutMainLoop();
}
