#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <omp.h>
#include <vector>

using namespace std;

float p, a;

const int WIDTH = 1920;
const int HEIGHT = 1080;
const float radius = 10.0f;
float friction = 0.9f;

vector<bool> alfaParticle;
vector<float> arrayVX;
vector<float> arrayVY;
vector<float> arrayY;
vector<float> arrayX;

void display()
{
    static int frame_count = 0;
    static int previous_time = glutGet(GLUT_ELAPSED_TIME);
    glClear(GL_COLOR_BUFFER_BIT);
    for (int j = 0; j < alfaParticle.size(); j++)
    {
        if (alfaParticle.at(j))
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

    frame_count++;
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    float elapsed_time = (current_time - previous_time) / 1000.0f; // Convert to seconds
    if (elapsed_time > 1.0f) // Update every second
    {
        float fps = frame_count / elapsed_time;
        printf("FPS: %.2f\n", fps);
        frame_count = 0;
        previous_time = current_time;
    }
}

void update(int value)
{
    double start_time = omp_get_wtime();
    for (int i = 0; i < alfaParticle.size(); i++)
    {

        int cont = 0;
        float d = 9999.99f;
        vector<float> arrayT;
        for (int k = 0; k < alfaParticle.size(); k++)
            arrayT.push_back(9999.9f);
        if (alfaParticle[i] == false)
        {
            for (int j = 0; j < alfaParticle.size(); j++)
            {
                if (alfaParticle[j] && i != j)
                {
                    arrayT[j] = sqrt(pow(arrayX[j] - arrayX[i], 2) + pow(arrayY[j] - arrayY[i], 2));
                    if (arrayT[j] < d)
                    {
                        d = arrayT[j];
                        cont = j;
                    }
                }
            }
            if (d == 0.0f)
                d = 1;
            if (d < 500.0f)
            {
                const float dx = arrayX[i] - arrayX[cont];
                const float dy = arrayY[i] - arrayY[cont];
                const float nx = dx * (1 / d);
                const float ny = dy * (1 / d);
                d = fmax(d, 0.5);
                arrayVX[i] = arrayVX[i] - ((nx * 100) / d);
                arrayVY[i] = arrayVY[i] - ((ny * 100) / d);
                arrayVX[i] = arrayVX[i] * friction;
                arrayVY[i] = arrayVY[i] * friction;
            }
        }
        arrayX[i] = arrayX[i] + arrayVX[i];
        arrayY[i] = arrayY[i] + arrayVY[i];

        if (arrayX[i] < -WIDTH / 2 + radius || arrayX[i] > WIDTH / 2 - radius)
        {
            arrayVX[i] = -arrayVX[i];
        }
        if (arrayY[i] < -HEIGHT / 2 + radius || arrayY[i] > HEIGHT / 2 - radius)
        {
            arrayVY[i] = -arrayVY[i];
        }
    }

    double end_time = omp_get_wtime();

    double elapsed_time = end_time - start_time;
    printf("Elapsed time: %.4f seconds\n", elapsed_time);
    glutPostRedisplay();

    glutTimerFunc(0, update, 0);
}

int main(int argc, char **argv)
{
    p = 6;
    a = 2;

    if ( argc == 1) {
        printf("Usage: mainp numeroParticulasAlfas numeroParticulasBeta\n");
        exit (1);
    }

    int a = atoi(argv[1]);
    int p = atoi(argv[2]);

    for (int i = 0; i < a; i++)
    {
        float g = 1.0f;
        if (i % 2 == 0)
            g = -1.0f;
        alfaParticle.push_back(true);
        float v = (float(rand()) / (float)(RAND_MAX)) * 10 * g;
        float xp = (float(rand()) / (float)(RAND_MAX)) * 700 * g;
        float yp = (float(rand()) / (float)(RAND_MAX)) * 400 * g;
        arrayVX.push_back(v);
        arrayVY.push_back(v);
        arrayX.push_back(xp);
        arrayY.push_back(yp);
    }

    for (int i = 0; i < p; i++)
    {
        float g = 1.0f;
        if (i % 2 == 0)
            g = -1.0f;
        alfaParticle.push_back(false);
        float xp = (float(rand()) / (float)(RAND_MAX)) * 700 * g;
        float yp = (float(rand()) / (float)(RAND_MAX)) * 400 * g;
        arrayVX.push_back(0.0f);
        arrayVY.push_back(0.0f);
        arrayX.push_back(xp);
        arrayY.push_back(yp);
    }

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