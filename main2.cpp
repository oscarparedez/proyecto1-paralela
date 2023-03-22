#include <iostream>
#include <math.h>
#include <GL/glut.h>

using namespace std;

float p, a, b;

const int WIDTH = 1920;
const int HEIGHT = 1080;
const float radius = 25.0f;

float particles[6];
float alfas[3];
float beta[3];

bool alfaParticle[6] = {true, false, false, false, false, false};

float arrayX[6] = {50.0f, -200.0f, -40.0f, 60.0f, 150.0f, 300.0f};
float arrayY[6] = {20.0f, 170.0f, 50.0f, 60.0f, -150.0f, -300.0f};

float x = 0.0f, y = 0.0f;
float arrayVX[6] = {10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
float arrayVY[6] = {-10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (int j = 0; j < 6; j++)
    {
        if (alfaParticle[j])
            glColor3f(0.0f, 0.0f, 1.0f);
        else
            glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(arrayX[j], arrayY[j]);
        const int numSegments = 16;
        for (int i = 0; i <= numSegments; i++)
        {
            float angle = i * 2.0f * 3.1415926f / numSegments;
            float dx = radius * cosf(angle);
            float dy = radius * sinf(angle);
            glVertex2f(arrayX[j] + dx, arrayY[j] + dy);
        }
        glEnd();
    }

    glutSwapBuffers();
}

void update(int value)
{
    for (int i = 0; i < 6; i++)
    {
        arrayX[i] += arrayVX[i];
        arrayY[i] += arrayVY[i];
        int cont = 0;
        float temp = 9999.99f;
        float arrayT[6] = {9999.99f, 9999.99f, 9999.99f, 9999.99f, 9999.99f, 9999.99f};
        if (alfaParticle[i] == false)
        {
            for (int j = 0; j < 6; j++)
            {
                if (alfaParticle[j] && i != j)
                {
                    arrayT[j] = sqrt(pow(arrayX[j] - arrayX[i], 2) + pow(arrayY[j] - arrayY[i], 2));
                    if (arrayT[j] < temp)
                    {
                        temp = arrayT[j];
                        cont = j;
                    }
                }
            }
            if (temp < 100.0f)
            {
                const float dx = pos.x - otherPos.x;
                const float dy = pos.y - otherPos.y;
                const float nx = dx * (1 / temp);
                const float ny = dy * (1 / temp);
                arrayVX[i] -= nx / temp;
                arrayVY[i] -= ny / temp;
                // else arrayVY[i] = arrayVY[i]+5.0f;
                // else arrayVX[i] = arrayVX[i]+5.0f;
                /* arrayVY[i] = arrayVY[cont]-5.5f;
                arrayVX[i] = arrayVX[cont]-5.5f; */
            }
        } /* else {
            for (int j = 0; j < 6; j++) {
                if (alfaParticle[j] && i != j) {
                    arrayT[j] = sqrt(pow(arrayX[j]-arrayX[i], 2) + pow(arrayY[j]-arrayY[i], 2));
                    if (arrayT[j] < temp) {
                        temp = arrayT[j];
                        cont = j;
                    }
                }
            }
            if (temp < 100.0f) {
                cout<<i<<endl;
                arrayVY[i] = -arrayVY[cont];
                arrayVX[i] = -arrayVX[cont];
            }
        } */

        if (arrayX[i] < -WIDTH / 2 + radius || arrayX[i] > WIDTH / 2 - radius)
        {
            arrayVX[i] = -arrayVX[i];
        }
        if (arrayY[i] < -HEIGHT / 2 + radius || arrayY[i] > HEIGHT / 2 - radius)
        {
            arrayVY[i] = -arrayVY[i];
        }
    }

    glutPostRedisplay();

    const int delay = 16;
    glutTimerFunc(delay, update, 0);
}

int main(int argc, char **argv)
{
    p = 6;
    a = 2;
    b = 1;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Screensaver");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WIDTH / 2, WIDTH / 2, -HEIGHT / 2, HEIGHT / 2, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);

    glutMainLoop();

    return 0;
}