#ifndef BUTTON_H
#define BUTTON_H

#include "GL/freeglut.h"
#include <string>

class Button{
private:
float x, y, size;
std::string text;
bool hovered;

public:
bool click;

    Button() : x(0.0f), y(0.0f), size(0.2f), text("Button"), hovered(false), click(false){}

    Button(float x, float y, float size, std::string text, bool hovered, bool click) : x(x), y(y), size(size), text(text), hovered(hovered), click(click) {}

    void draw() const {
        //draw rectangle
        glColor3f(hovered ? 0.7f : 0.5,0.5,0.5);
        glBegin(GL_POLYGON);
            glVertex2f(x - size / 2, y - size / 2);
            glVertex2f(x + size / 2, y - size / 2);
            glVertex2f(x + size / 2, y + size / 2);
            glVertex2f(x - size / 2, y + size / 2);
        glEnd();

        //draw border
        glColor3f(0,0,0);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(x - size / 2, y - size / 2);
            glVertex2f(x + size / 2, y - size / 2);
            glVertex2f(x + size / 2, y + size / 2);
            glVertex2f(x - size / 2, y + size / 2);
        glEnd();

        //draw label
        glColor3f(0.0f, 0.0f, 0.0f);
        glRasterPos2f(x - text.size() * 0.01f, y - 0.01f);
        for(char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    void checkHover(int x, int y, int size) {
        //normal coordinates
        float nx = (2.0f * x) / size - 1.0f;
        float ny = 1.0f - (2.0f * y) / size;

        hovered = (nx >= x - size / 2 && nx <= x + size / 2 && ny >= y - size / 2 && ny <= y + size / 2);
    }

    void handleClick() {
        click = hovered;
    }

    void resetClick() {
        click = false;
    }

    ~Button() = default;
};

#endif
