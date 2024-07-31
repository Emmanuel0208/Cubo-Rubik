#include <windows.h>
#include <GL/glut.h>
#include <vector>
#include <cmath>

// Definición de M_PI si no está definida
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

char title[] = "3D Rubik's Cube";

// Estructura para representar un cubo individual
struct Cube {
    float x, y, z;            // Posición del cubo
    float size;               // Tamaño del cubo
    float rotationX, rotationY, rotationZ; // Rotaciones en los ejes X, Y y Z
    Cube(float x, float y, float z, float size)
        : x(x), y(y), z(z), size(size), rotationX(0), rotationY(0), rotationZ(0) {}
};

std::vector<Cube> cubes;     // Vector para almacenar todos los cubos
float cubeSize = 1.0f;       // Tamaño de cada cubo
float separation = 2.2f;     // Separación entre los cubos

// Variables para la rotación de la cámara
float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f;
float cameraDistance = -30.0f;
bool isDragging = false;
int lastMouseX, lastMouseY;
float rotationSpeed = 0.1f;

// Función para inicializar OpenGL
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Color de fondo negro y opaco
    glClearDepth(1.0f);                   // Profundidad de fondo más lejana
    glEnable(GL_DEPTH_TEST);              // Habilitar prueba de profundidad para culling
    glDepthFunc(GL_LEQUAL);               // Tipo de prueba de profundidad
    glShadeModel(GL_SMOOTH);              // Habilitar sombreado suave
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Corrección de perspectiva

    // Inicializar los cubos en sus posiciones
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                cubes.emplace_back(x * separation, y * separation, z * separation, cubeSize);
            }
        }
    }
}

// Función para dibujar un cubo individual
void drawCube(const Cube& cube) {
    glPushMatrix(); // Guardar la matriz actual
    glTranslatef(cube.x, cube.y, cube.z); // Trasladar el cubo a su posición
    glRotatef(cube.rotationX, 1.0f, 0.0f, 0.0f); // Aplicar rotación en X
    glRotatef(cube.rotationY, 0.0f, 1.0f, 0.0f); // Aplicar rotación en Y
    glRotatef(cube.rotationZ, 0.0f, 0.0f, 1.0f); // Aplicar rotación en Z

    glBegin(GL_QUADS); // Iniciar el dibujo de las caras del cubo
    // Caras del cubo con diferentes colores
    // Cara superior
    glColor3f(0.0f, 1.0f, 0.0f); // Verde
    glVertex3f(cube.size, cube.size, -cube.size);
    glVertex3f(-cube.size, cube.size, -cube.size);
    glVertex3f(-cube.size, cube.size, cube.size);
    glVertex3f(cube.size, cube.size, cube.size);

    // Cara inferior
    glColor3f(1.0f, 0.5f, 0.0f); // Naranja
    glVertex3f(cube.size, -cube.size, cube.size);
    glVertex3f(-cube.size, -cube.size, cube.size);
    glVertex3f(-cube.size, -cube.size, -cube.size);
    glVertex3f(cube.size, -cube.size, -cube.size);

    // Cara frontal
    glColor3f(1.0f, 0.0f, 0.0f); // Rojo
    glVertex3f(cube.size, cube.size, cube.size);
    glVertex3f(-cube.size, cube.size, cube.size);
    glVertex3f(-cube.size, -cube.size, cube.size);
    glVertex3f(cube.size, -cube.size, cube.size);

    // Cara trasera
    glColor3f(1.0f, 1.0f, 0.0f); // Amarillo
    glVertex3f(cube.size, -cube.size, -cube.size);
    glVertex3f(-cube.size, -cube.size, -cube.size);
    glVertex3f(-cube.size, cube.size, -cube.size);
    glVertex3f(cube.size, cube.size, -cube.size);

    // Cara izquierda
    glColor3f(0.0f, 0.0f, 1.0f); // Azul
    glVertex3f(-cube.size, cube.size, cube.size);
    glVertex3f(-cube.size, cube.size, -cube.size);
    glVertex3f(-cube.size, -cube.size, -cube.size);
    glVertex3f(-cube.size, -cube.size, cube.size);

    // Cara derecha
    glColor3f(1.0f, 0.0f, 1.0f); // Magenta
    glVertex3f(cube.size, cube.size, -cube.size);
    glVertex3f(cube.size, cube.size, cube.size);
    glVertex3f(cube.size, -cube.size, cube.size);
    glVertex3f(cube.size, -cube.size, -cube.size);
    glEnd(); // Terminar el dibujo de las caras

    glPopMatrix(); // Restaurar la matriz anterior
}

// Función de visualización
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpiar buffers de color y profundidad
    glMatrixMode(GL_MODELVIEW);     // Operar en la matriz de vista de modelo

    glLoadIdentity(); // Reiniciar la matriz de vista de modelo
    glTranslatef(0.0f, 0.0f, cameraDistance); // Trasladar la cámara
    glRotatef(angleX, 1.0f, 0.0f, 0.0f); // Rotar la cámara en X
    glRotatef(angleY, 0.0f, 1.0f, 0.0f); // Rotar la cámara en Y
    glRotatef(angleZ, 0.0f, 0.0f, 1.0f); // Rotar la cámara en Z

    // Dibujar todos los cubos
    for (const auto& cube : cubes) {
        drawCube(cube);
    }

    glutSwapBuffers(); // Intercambiar los buffers frontales y traseros
}

// Función de redimensionamiento de la ventana
void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1; // Prevenir división por 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height); // Configurar el viewport

    glMatrixMode(GL_PROJECTION); // Operar en la matriz de proyección
    glLoadIdentity(); // Reiniciar la matriz de proyección
    gluPerspective(45.0f, aspect, 0.1f, 100.0f); // Configurar la perspectiva
}

// Función para aplicar rotaciones a los cubos en una sección
void applyRotationToSection(float angle, char axis, std::vector<int>& indices) {
    for (int index : indices) {
        Cube& cube = cubes[index];
        if (axis == 'x') {
            cube.rotationX += angle;
        }
        else if (axis == 'y') {
            cube.rotationY += angle;
        }
        else if (axis == 'z') {
            cube.rotationZ += angle;
        }
    }
}

// Función para manejar la entrada del teclado
void keyboard(unsigned char key, int x, int y) {
    std::vector<int> indices;
    switch (key) {
    case '1': // Rotar cara superior en el eje Y
        for (int i = 0; i < cubes.size(); ++i) {
            if (cubes[i].y == separation) {
                indices.push_back(i);
            }
        }
        applyRotationToSection(90.0f, 'y', indices);
        break;
    case '2': // Rotar cara media horizontal en el eje Y
        for (int i = 0; i < cubes.size(); ++i) {
            if (cubes[i].y == 0) {
                indices.push_back(i);
            }
        }
        applyRotationToSection(90.0f, 'y', indices);
        break;
    case '3': // Rotar cara inferior en el eje Y
        for (int i = 0; i < cubes.size(); ++i) {
            if (cubes[i].y == -separation) {
                indices.push_back(i);
            }
        }
        applyRotationToSection(90.0f, 'y', indices);
        break;
    case '4': // Rotar cara izquierda en el eje X
        for (int i = 0; i < cubes.size(); ++i) {
            if (cubes[i].x == -separation) {
                indices.push_back(i);
            }
        }
        applyRotationToSection(90.0f, 'x', indices);
        break;
    case '5': // Rotar cara media vertical en el eje X
        for (int i = 0; i < cubes.size(); ++i) {
            if (cubes[i].x == 0) {
                indices.push_back(i);
            }
        }
        applyRotationToSection(90.0f, 'x', indices);
        break;
    case '6': // Rotar cara derecha en el eje X
        for (int i = 0; i < cubes.size(); ++i) {
            if (cubes[i].x == separation) {
                indices.push_back(i);
            }
        }
        applyRotationToSection(90.0f, 'x', indices);
        break;
    case '7': // Rotar cara frontal en el eje Z
        for (int i = 0; i < cubes.size(); ++i) {
            if (cubes[i].z == separation) {
                indices.push_back(i);
            }
        }
        applyRotationToSection(90.0f, 'z', indices);
        break;
    case '8': // Rotar cara media vertical en el eje Z
        for (int i = 0; i < cubes.size(); ++i) {
            if (cubes[i].z == 0) {
                indices.push_back(i);
            }
        }
        applyRotationToSection(90.0f, 'z', indices);
        break;
    case '9': // Rotar cara trasera en el eje Z
        for (int i = 0; i < cubes.size(); ++i) {
            if (cubes[i].z == -separation) {
                indices.push_back(i);
            }
        }
        applyRotationToSection(90.0f, 'z', indices);
        break;
    case 27: // ESC para salir
        exit(0);
    }
    glutPostRedisplay(); // Solicitar redibujado
}

// Función para manejar eventos de ratón
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else {
            isDragging = false;
        }
    }
}

// Función para manejar el movimiento del ratón
void motion(int x, int y) {
    if (isDragging) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;
        angleY += dx * rotationSpeed;
        angleX += dy * rotationSpeed;
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay(); // Solicitar redibujado
    }
}

// Función principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(50, 50);
    glutCreateWindow(title);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    initGL();
    glutMainLoop(); // Entrar en el bucle de eventos
    return 0;
}
