/*
 * OGL01Shape3D.cpp: 3D Shapes
 */
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h

 /* Global variables */
char title[] = "3D Shapes";

/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void drawCube(float x, float y, float z, float tama�o)
{
    glPushMatrix();
    glTranslatef(x, y, z);

    glBegin(GL_QUADS);

    // Top face 
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f(tama�o, tama�o, -tama�o);
    glVertex3f(-tama�o, tama�o, -tama�o);
    glVertex3f(-tama�o, tama�o, tama�o);
    glVertex3f(tama�o, tama�o, tama�o);

    // Bottom face
    glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(tama�o, -tama�o, tama�o);
    glVertex3f(-tama�o, -tama�o, tama�o);
    glVertex3f(-tama�o, -tama�o, -tama�o);
    glVertex3f(tama�o, -tama�o, -tama�o);

    // Front face 
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(tama�o, tama�o, tama�o);
    glVertex3f(-tama�o, tama�o, tama�o);
    glVertex3f(-tama�o, -tama�o, tama�o);
    glVertex3f(tama�o, -tama�o, tama�o);

    // Back face 
    glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(tama�o, -tama�o, -tama�o);
    glVertex3f(-tama�o, -tama�o, -tama�o);
    glVertex3f(-tama�o, tama�o, -tama�o);
    glVertex3f(tama�o, tama�o, -tama�o);

    // Left face 
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-tama�o, tama�o, tama�o);
    glVertex3f(-tama�o, tama�o, -tama�o);
    glVertex3f(-tama�o, -tama�o, -tama�o);
    glVertex3f(-tama�o, -tama�o, tama�o);

    // Right face 
    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(tama�o, tama�o, -tama�o);
    glVertex3f(tama�o, tama�o, tama�o);
    glVertex3f(tama�o, -tama�o, tama�o);
    glVertex3f(tama�o, -tama�o, -tama�o);
    glEnd();  // End of drawing color-cube

    glPopMatrix();
}

/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

    // Render a color-cube consisting of 6 quads with different colors
    glLoadIdentity();                 // Reset the model-view matrix
    glTranslatef(5.0f, 0.0f, -25.0f);  // Move right and into the screen

    float separation = 2.2f;

    

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            for (int z = -1; z <= 1; ++z) {
                drawCube(x * separation, y * separation, z * separation, 1.0f);
            }
        }
    }

    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
    glutInitWindowSize(640, 480);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow(title);          // Create window with the given title
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);       // Register callback handler for window re-size event
    initGL();                       // Our own OpenGL initialization
    glutMainLoop();                 // Enter the infinite event-processing loop
    return 0;
}