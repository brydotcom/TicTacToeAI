#ifndef CIRCLE_H
#define CIRCLE_H

#include "GL/freeglut.h"

class Circle {
    float x;
    float y;
    float r;
    float b;
    float g;
    float size;

public:
    Circle() {
        x = 0.0f;
        y = 0.0f;
        r = 0.0;
        g = 0.0f;
        b = 0.0f;
        size = 0.2f;
    }

    Circle(float x, float y, float r, float g, float b, float size) {
        this->x = x;
        this->y = y;
        this->r = r;
        this->g = g;
        this->b = b;
        this->size = size;
    }

    void draw() {
        //
    }
};

#endif
