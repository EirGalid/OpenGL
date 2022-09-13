#include<windows.h>
#include <GL/glui.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>

// initializing the user interface
int user_interface;

//define window position
int window_x, window_y;
int window_width=600, window_height=600;

//setting up parameters
int fovy=160;
int D;
int aspect;
int s;

struct point3D { double x, y, z, w; };
struct matrix4x4
{
    double  a, b, c, d,
            e, f, g, h,
            i, j, k, l,
            m, n, o, p;
};
point3D C[8];
int X = -200, Y = 0, Z = 100, ANGLEY = 0, ANGLEZ = 0, ANGLEX=0; 

void set_identity_matrix(matrix4x4 &mat)
{
    mat.a = 1; mat.b = 0; mat.c = 0; mat.d = 0;
    mat.e = 0; mat.f = 1; mat.g = 0; mat.h = 0;
    mat.i = 0; mat.j = 0; mat.k = 1; mat.l = 0;
    mat.m = 0; mat.n = 0; mat.o = 0; mat.p = 1;
}

void mult(matrix4x4 mat, point3D &p)
{
    point3D temp;
    temp.x = mat.a * p.x + mat.b * p.y + mat.c * p.z + mat.d * p.w;
    temp.y = mat.e * p.x + mat.f * p.y + mat.g * p.z + mat.h * p.w;
    temp.z = mat.i * p.x + mat.j * p.y + mat.k * p.z + mat.l * p.w;
    temp.w = mat.m * p.x + mat.n * p.y + mat.o * p.z + mat.p * p.w;
    p = temp;
}

void translate(point3D &p, double tx, double ty, double tz)
{
    matrix4x4 temp;
    set_identity_matrix(temp);
    temp.d = tx;
    temp.h = ty;
    temp.l = tz;
    mult(temp, p);
}

void scale(point3D &p, double s)
{
    matrix4x4 temp;
    set_identity_matrix(temp);
    temp.a *= s;
    temp.f *= s;
    temp.k *= s;
    mult(temp, p);
}

void rotateX(point3D &p, double degrees)
{
    matrix4x4 temp;
    set_identity_matrix(temp);
    double sinTheta = sin(degrees * M_PI / 180);
    double cosTheta = cos(degrees * M_PI / 180);
    temp.f = cosTheta;
    temp.g = -sinTheta;
    temp.j = sinTheta;
    temp.k = cosTheta;
    mult(temp, p);
}

void rotateY(point3D &p, double degrees)
{
    matrix4x4 temp;
    set_identity_matrix(temp);
    double sinTheta = sin(degrees * M_PI / 180);
    double cosTheta = cos(degrees * M_PI / 180);
    temp.a = cosTheta;
    temp.c = sinTheta;
    temp.i = -sinTheta;
    temp.k = cosTheta;
    mult(temp, p);
}

void rotateZ(point3D &p, double degrees)
{
    matrix4x4 temp;
    set_identity_matrix(temp);
    double sinTheta = sin(degrees * M_PI / 180);
    double cosTheta = cos(degrees * M_PI / 180);
    temp.a = cosTheta;
    temp.b = -sinTheta;
    temp.e = sinTheta;
    temp.f = cosTheta;
    mult(temp, p);
}

void draw_line(double x1, double y1, double x2, double y2)
{
    glBegin(GL_LINES);
    glVertex2d(x1, y1);
    glVertex2d(x2, y2);
    glEnd();
}

void setup_cube()
{
    C[0].x = -0.5; C[0].y = -0.5; C[0].z = -0.5; C[0].w = 1;
    C[1].x =  0.5; C[1].y = -0.5; C[1].z = -0.5; C[1].w = 1;
    C[2].x =  0.5; C[2].y =  0.5; C[2].z = -0.5; C[2].w = 1;
    C[3].x = -0.5; C[3].y =  0.5; C[3].z = -0.5; C[3].w = 1;
    C[4].x = -0.5; C[4].y = -0.5; C[4].z = 0.5; C[4].w = 1;
    C[5].x =  0.5; C[5].y = -0.5; C[5].z = 0.5; C[5].w = 1;
    C[6].x =  0.5; C[6].y =  0.5; C[6].z = 0.5; C[6].w = 1;
    C[7].x = -0.5; C[7].y =  0.5; C[7].z = 0.5; C[7].w = 1;
}

void perspective_projection(point3D &p)
{
    matrix4x4 temp;
    set_identity_matrix(temp);
    temp.a = D / (D + p.z);
    temp.f = temp.a;
    temp.k = D / p.z;
    mult(temp, p);
}

void draw_cube()
{
    for(int i = 0; i < 4; i++)
    {
        draw_line(C[i].x, C[i].y, C[(i + 1) % 4].x, C[(i + 1) % 4]. y);
        draw_line(C[i + 4].x, C[i + 4].y, C[(i + 1) % 4 + 4].x, C[(i + 1) % 4 + 4]. y);
        draw_line(C[i].x, C[i].y, C[i + 4].x, C[i + 4]. y);
    }
}

void draw_axes()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2d(-240, 0);
    glVertex2d( 240, 0);
    glVertex2d(0, -180);
    glVertex2d(0, 180);
    glEnd();
    glPopAttrib();
}

void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    setup_cube();

    for(int i = 0; i < 8; i++)
        scale(C[i], s);

    for(int i = 0; i < 8; i++)
        rotateZ(C[i], -ANGLEZ);

    for(int i = 0; i < 8; i++)
        rotateY(C[i], -ANGLEY);
        
    for(int i = 0; i < 8; i++)
        rotateX(C[i], -ANGLEX);

    for(int i = 0; i < 8; i++)
        translate(C[i], X, Y, Z);

    for(int i = 0; i < 8; i++)
       perspective_projection(C[i]);

    glColor3ub(0, 0, 255);
    draw_cube();
    D = (window_width / 2.0) / (aspect * tan(fovy / 2.0 * M_PI / 180.0) );
}

//reshape function
void reshape(int window_width, int window_height)
{
    if(window_height == 0) window_height = 1;
    aspect = (double)window_width / (double)window_height;
    D = (window_width / 2.0) / (aspect * tan(fovy / 2.0 * M_PI / 180.0) ); 
}

void glui_stuff()
{
    GLUI *glui = GLUI_Master.create_glui("3D TRANSFORMATIONS");
      glui->add_statictext("Translation");

    GLUI_Spinner *tx_spinner=
	    glui->add_spinner( "tx:", GLUI_SPINNER_FLOAT, &X);
        tx_spinner->set_float_limits(-240,240);

    GLUI_Spinner *ty_spinner=
	    glui->add_spinner( "ty:", GLUI_SPINNER_FLOAT, &Y);
        ty_spinner->set_float_limits(-180,180);

    GLUI_Spinner *tz_spinner=
	    glui->add_spinner( "tz:", GLUI_SPINNER_FLOAT, &Z);
        tz_spinner->set_float_limits(-180,180);         

    glui -> add_column(true);
    glui->add_statictext("Rotation & Scaling");
    
    GLUI_Spinner *rotx_spinner=
	    glui->add_spinner( "X axis rotation:", GLUI_SPINNER_INT, &ANGLEX);
        rotx_spinner->set_int_limits(1,600);

    GLUI_Spinner *roty_spinner=
	    glui->add_spinner( "Y axis rotation:", GLUI_SPINNER_INT, &ANGLEY);
        roty_spinner->set_int_limits(1,600);

    GLUI_Spinner *rotz_spinner=
	    glui->add_spinner( "Z axis rotation:", GLUI_SPINNER_INT, &ANGLEZ);
        rotz_spinner->set_int_limits(1,600);

    GLUI_Spinner *s_spinner=
	    glui->add_spinner( "Scale:", GLUI_SPINNER_FLOAT, &s);
        s_spinner->set_int_limits(1,600);

    glui->add_column(true);
    glui->add_statictext("Perspective Projection-specific variables");

    GLUI_Spinner *fovy_spinner=
	    glui->add_spinner( "fovy:", GLUI_SPINNER_INT, &fovy);
        fovy_spinner->set_int_limits(1,600);

    GLUI_Spinner *aspect_spinner=
	    glui->add_spinner( "aspect:", GLUI_SPINNER_INT, &aspect);
        aspect_spinner->set_int_limits(1,600);
        aspect_spinner->set_speed(0.05);

    GLUI_Spinner *D_spinner=
	    glui->add_spinner( "D scale:", GLUI_SPINNER_INT, &D);
        D_spinner->set_int_limits(1,600);

   glui -> set_main_gfx_window(user_interface);
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
    user_interface=glutCreateWindow("3D TRANSFORMATIONS");
    gluOrtho2D(-240, 240, -180, 180);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glui_stuff();    
    glutMainLoop();
    return 0;
}



