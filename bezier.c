#include <GL/glut.h>
#include <stdio.h>

float control_points[100][2] = {

};
int points_count = 0;

int DIMEN_X = 600;
int DIMEN_Y = 600;

int STATE = -1;
int ADD_STATE = 0;
int DELETE_STATE = 1;
int DRAG_STATE = 2;

float TOUCH_BUFFER = 4;
int RES = 1000; // Pixel resolution

float drag_x, drag_y;

/*! \brief Removes coordinates from control points array.
*
*
*   \param x: x-coordinate to be removed.
*   \param y: y-coordinate to be removed.
*/
void remove_coordinates(float x, float y)
{
  int temp_count = points_count;
  for (int i = 0; i <= temp_count; i++)
  {
    if (control_points[i][0] - x < TOUCH_BUFFER && control_points[i][0] - x > -1 * TOUCH_BUFFER)
    {
      if (control_points[i][1] - y < TOUCH_BUFFER && control_points[i][1] - y > -1 * TOUCH_BUFFER)
      {
        control_points[i][0] = control_points[i+1][0];
        control_points[i][1] = control_points[i+1][1];
        temp_count--;
      }
    }
  }
  points_count = temp_count;
}

/*! \brief Adds coordinates to the control points array.
*
*
*   \param x: x-coordinate to be added.
*   \param y: y-coordinate to be added.
*/
void add_coordinates(float x, float y)
{
  control_points[points_count][0] = x;
  control_points[points_count][1] = y;
  points_count++;
}

/*! \brief Replaces a control point in the array.
*
*
*   \param x_old: x-coordinate to be replaced.
*   \param y_old: y-coordinate to be replaced.
*   \param x_new: new x-coordinate
*   \param y_new: new y-coordinate
*/
void replace_coordinates(float x_old, float y_old, float x_new, float y_new)
{
  for (int i = 0; i < points_count; i++)
  {

    if (control_points[i][0] - x_old < TOUCH_BUFFER && control_points[i][0] - x_old > -1 * TOUCH_BUFFER)
    {
      if (control_points[i][1] - y_old < TOUCH_BUFFER && control_points[i][1] - y_old > -1 * TOUCH_BUFFER)
      {
        control_points[i][0] = x_new;
        control_points[i][1] = y_new;
        break;
      }
    }
  }
}

/*! \brief Plots point on the window.
*
*
*   \param x: x-coordinate to be plotted.
*   \param y: y-coordinate to be plotted.
*   \param color: r value in the rgb scheme.
*/
void plotPoint (int x, int y, float color)
{
  glBegin(GL_POINTS);
  glColor3f(color, 0.1, 0.5);
  glVertex2i(x, y);
  glEnd();
}

/*! \brief Point on the line segment between 2 points.
*
*
*   \param start: 2 dimensional start vertex.
*   \param end: 2 dimensional end vertex.
*   \param t: float value - point on the line segment between the vertices.
*/
float * lerp (float start[2], float end[2], float t)
{
  static float req[2];
  req[0] =  start[0] + t * (end[0] - start[0]);
  req[1] = start[1] + t * (end[1] - start[1]);
  return req;
}

/*! \brief Constructs a bezier curve recursively using deCasteljau's algorithm.
*
*
*   \param controls: 2 column matrix of vertices recursively lerped.
*   \param number: number of coordinates
*   \param t: distance on the line segment between two vertices
*/
void bezier (float controls[][2], int number, float t)
{
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

/*! \brief OpenGL mouse callback.
*
*
*   \param button: Button clicked on the mouse.
*   \param state : State of the button clicked.
*   \param x,y: Inverted coordinates on the window
*/
void mouse(int button, int state, int x, int y)
{
  y = DIMEN_Y - y;
  	if (button == GLUT_LEFT_BUTTON)
  	{
		if (state == GLUT_DOWN) {
      if (STATE == ADD_STATE)
      {
        add_coordinates(x, y);
      } else if (STATE == DELETE_STATE)
      {
        remove_coordinates(x,y);
      } else if (STATE == DRAG_STATE)
      {
        drag_x = x;
        drag_y = y;
      }
		}
		if (state == GLUT_UP && STATE == DRAG_STATE) {
      replace_coordinates(drag_x, drag_y, x, y);
      glutPostRedisplay();
		}
  	}
    glutPostRedisplay();
}

/*! \brief OpenGL keyboard callback.
*
*
*   \param key: Key on the keyboard.
*/
void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
    case 'a':  // Set's state to ADD
      STATE = ADD_STATE;
      break;
    case 'd': // Set's state to DELETE
      STATE = DELETE_STATE;
      break;
    case 'b': // Set's state to DRAG
      STATE = DRAG_STATE;
      break;
  }
}

/*! \brief Initializes OpenGL parameters.
*
*
*/
void init()
{
  glClearColor(0.3, 0.2, 1.0, 0.1);
  glColor3f(1.0f, 0.0f, 0.0f);
  gluOrtho2D(0.0, DIMEN_X, 0.0, DIMEN_Y);
  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  glPointSize(10.0f);
}

/*! \brief OpenGL display callback. Runs the bezier curve implementation.
*
*/
void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  int i = 0;
  if (points_count > 0)
  {
    for (i = 0; i < points_count; i++)
    {
      plotPoint((int)control_points[i][0], (int)control_points[i][1], 1.0);
    }
    if (points_count > 1)
    {
      for (i=0; i<RES; i++)
      {
        bezier(control_points, points_count, (float)i/(float)(RES-1));
      }
    }
  }
  glFlush();
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(DIMEN_X, DIMEN_Y);
	glutCreateWindow("Bezier");
  STATE = ADD_STATE;
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  init();
	glutMainLoop();
}
