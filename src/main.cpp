#include <GL/freeglut.h>

#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <random>
#include <ctime>

#include "funciones.h"
#define WIDTH 800
#define HEIGHT 600
using namespace std;

void drawLineGraph(std::vector<double>& data, double min, double max, float r, float g, float b) {
    double diff = max - min;
    double step = (double)WIDTH / (data.size() - 1);

    glLineWidth(2.0f);
    glColor3f(r, g, b);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < data.size(); ++i) {
        double x = i * step;
        double y = ((data[i] - min) / diff) * HEIGHT;
        glVertex2f(x, y); // invert y to match OpenGL's bottom-left origin
    }
    glEnd();
}

void display() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    double max = std::max(*std::max_element(media_fin.begin(), media_fin.end()), *std::max_element(max_min_fin.begin(), max_min_fin.end()));
    double min = std::min(*std::min_element(media_fin.begin(), media_fin.end()), *std::min_element(max_min_fin.begin(), max_min_fin.end()));
    drawLineGraph(media_fin, min, max, 1.0f, 0.0f, 0.0f); // red color
    drawLineGraph(max_min_fin, min, max, 0.0f, 1.0f, 0.0f); // green color

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void initializeGLUT() {
    int argc = 0;
    char *argv[1] = {(char*)"Something"};
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Line Graph");
}

int main(){
    srand(time(NULL));
    vector<individuo> poblacion;
    for(int i=0;i<cantidad;i++){
        individuo tmp((rand()%(max_x-min_x+1))+min_x,(rand()%(max_y-min_y+1))+min_y,(rand()%(max_z-min_z+1))+min_z);
        poblacion.push_back(tmp);
    }
    ordenar(poblacion);
    --gen;
    muta_ini(poblacion);
    while(gen--){
        if(media_fin[media_fin.size()-1]==max_min_fin[max_min_fin.size()-1]){
            break;
        }
        vector<individuo> hijos;
        for(int i=0;i<poblacion.size();i++){
            individuo tmp(poblacion[i].x,poblacion[i].y,poblacion[i].z);
            hijos.push_back(tmp);
        }
        ordenar(hijos);
        poblacion.clear();
        while(iguales(hijos)){
            if(iguales2(hijos)){
                break;
            }
            else{
                shuffle(hijos.begin(), hijos.end(), default_random_engine(time(0)));
            }
        }
        cruzar(hijos);
        ordenar(hijos);
        muta_ini(hijos);
        poblacion=hijos;
    }
    initializeGLUT();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
}