#include <GL/glut.h>
#include <stdio.h>

float control_points[4][2] = {
  {100, 100},
  {100, 200},
  {200, 200},
  {200, 100}
};
int points_count = 0;

int DIMEN_X = 600;
int DIMEN_Y = 600;

void plotPoint (int x, int y, float color)
{
  glBegin(GL_POINTS);
  glColor3f(color, 0.1, 0.5);
  glVertex2i(x, y);
  glEnd();
}

float * lerp (float start[2], float end[2], float t)
{
  static float req[2];
  req[0] =  start[0] + t * (end[0] - start[0]);
  req[1] = start[1] + t * (end[1] - start[1]);
  printf("X0: %f  X1: %f \t Y0: %f  Y1: %f \t t: %f   OUTX: %f   OUTY: %f \n",start[0] ,end[0], start[1], end[1], t, req[0], req[1]);
  return req;
}

void bezier (float controls[][2], int number, float t)
{
  //printf("Number: %d\n", number);
  float control_outputs[number-1][2];
  for (int i = 0; i < number - 1; i++)
  {
    float *temp = lerp(controls[i], controls[i+1], t);
    control_outputs[i][0] = *(temp);
    control_outputs[i][1] = *(temp+1);
  }

  if (number == 2)
  {
    plotPoint(control_outputs[0][0], control_outputs[0][1], 0.5);
  } else {
    bezier(control_outputs, number - 1, t);
  }
}

void test ()
{
  float s[2] = {100, 100};
  float s1[2] = {100, 200};
  lerp(s, s1, 0.5);
}

void mouse(int button, int state, int x, int y)
{
  // printf("x:%d \t y:%d \n", x,y);
  // 	if (button == GLUT_LEFT_BUTTON)
  // 	{
	// 	if (state == GLUT_DOWN) {
  //     int i = 0;
  //     for (i=0; i<points_count; i++)
  //     {
  //       if (control_points[i][0] == x && control_points[i][1] == DIMEN_Y-y)
  //       {
  //
  //       }
  //     }
	// 	  a = x;
	// 	  b = DIMEN_Y - y;
	// 	}
	// 	if (state == GLUT_UP) {
  //     a = x;
  //     b = DIMEN_Y - y;
  //     glutPostRedisplay();
	// 	}
  // 	}
}

void init()
{
  glClearColor(0.3, 0.2, 1.0, 0.1);
  glColor3f(1.0f, 0.0f, 0.0f);
  gluOrtho2D(0.0, DIMEN_X, 0.0, DIMEN_Y);
  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  glPointSize(10.0f);

}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  plotPoint((int)control_points[0][0], (int)control_points[0][1], 1.0);
  plotPoint((int)control_points[1][0], (int)control_points[1][1], 1.0);
  plotPoint((int)control_points[2][0], (int)control_points[2][1], 1.0);
  plotPoint((int)control_points[3][0], (int)control_points[3][1], 1.0);
  int i = 0;
  for (i=0; i<1000; i++)
  {
    bezier(control_points, 4, (float)i/(float)999);
  }
  glFlush();
}

void main(int argc, char **argv)
{
  printf("Starting ....\n");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(DIMEN_X, DIMEN_Y);
	glutCreateWindow("Bezier");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
  init();
	glutMainLoop();
}
