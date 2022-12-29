#include <nd-xy.h>
#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

XY_lat lat;
xor256s_t r;
double X = 0;
uint64_t Count = 0;
double T = 0.01;
double J[3] = {1.0, 1.0, 1.0};
float COLOR;

double XY_dTheta(XY_lat *_lat) {
  double del_th = 0;
  double th = 0;
  double tmp;

  for (uint64_t i = 0; i < _lat->N; i++) {
    th += _lat->S[i];
  }
  th /= (double)_lat->N;
  for (uint64_t i = 0; i < _lat->N; i++) {
    tmp = _lat->S[i] - th;
    del_th += tmp * tmp;
  }
  return del_th / (double) _lat->N;
}

void draw_arrow(GLfloat px, GLfloat py, GLfloat al, double th) {
  GLfloat alx = al/3;
  GLfloat aly = al;
  GLfloat theta = (GLfloat) th * 180.0f / (2.0f * 3.14159f);
  theta += 90.0;

  glTranslatef(px, py, 0);
  glRotatef((GLfloat) theta, 0, 0, 1);
  glBegin(GL_TRIANGLES);
    glColor3f(COLOR, 1 - 2  * COLOR, 3 * COLOR); // white
    glVertex2f(0 + 1.5f*alx, 0 + aly);
    glVertex2f(0 - 1.5f*alx, 0 + aly);
    glVertex2f(0, 0 + 2*aly);
  glEnd();

  glBegin(GL_QUADS);
    glVertex2f(0 + alx/3, 0 + aly);
    glVertex2f(0 - alx/3, 0 + aly);
    glVertex2f(0 - alx/3, 0 - 2*aly);
    glVertex2f(0 + alx/3, 0 - 2*aly);
  glEnd();
  glRotatef(-(GLfloat) theta, 0, 0, 1);
  glTranslatef(-px,- py, 0);
}

void draw_XY_lat(void) {
  GLfloat dpx = 2.0f / (GLfloat) (lat.r + 1);
  GLfloat h = dpx / 3.0f;
  GLfloat px = dpx - 1, py;
  uint64_t n = 0;

  XY_evolve(&lat, 1/T, 64*lat.N, J, 0, &r);

  //printf("\tMAG: %f, %f\r", T, XY_M2_N2(&lat));
  if (Count % 100 == 0) {
    X = 0.0;
    Count = 0;
  }
  X += XY_dTheta(&lat);
  Count++;
  printf("\ttheta var: %f, %f\r", T, X/(double) Count);
  fflush(stdout);

  for (uint64_t i = 0; i < lat.r; i++) {
    py = dpx - 1;
    for (uint64_t j = 0; j < lat.r; j++) {
      COLOR = (GLfloat) lat.S[n] / (2 * 3.14159f);
      draw_arrow(px, py, h * (1 + 0.5f * 0.25f * XY_is_align(&lat, J, n)), lat.S[n]);
      py += dpx;
      n++;
    }
    px += dpx;
  }
  glFlush();
}

void display(int id) {  // Display function will draw the image.
    (void) id;
    glClearColor( 0, 0, 0, 1);  // (In fact, this is the default.)
    glClear( GL_COLOR_BUFFER_BIT );
    draw_XY_lat();
    glutSwapBuffers(); // Required to copy color buffer onto the screen.
    glutTimerFunc(20, display, 0);
}


int main( int argc, char** argv ) {  // Initialize GLUT and
  time_t ltime;
  double r_angle;

  time(&ltime);
  lat = XY_init(2, 64);
  //r = xor256s_init(479242);
  r = xor256s_init((uint64_t) ltime);
  XY_rand(&lat, &r);

  // not for random.
  //r_angle = rand_uni(&r) * 2 * 3.14159;
  //for (uint64_t i = 0; i < lat.N; i++) {
  //  lat.S[i] = r_angle; // all at same angle
  //}

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowPosition(100,100);     // Location of window in screen coordinates.
  glutInitWindowSize(750,750);         // Size of display area, in pixels.
  glutCreateWindow("F_TERM"); // Parameter is window title.
  glutDisplayFunc(draw_XY_lat);
  glutTimerFunc(0, display, 0);
  glutMainLoop(); // Run the event loop!  This function does not return.
                  // Program ends when user closes the window.
  XY_free(&lat);
  return 0;
}
