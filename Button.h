#ifndef BUTTON_H
#define BUTTON_H

#include "GL/freeglut_std.h"
#include <string>

class Button{
private:
float x, y, w, h;
std::string text;
bool hovered;
bool isPressed;

public:

    Button() : x(0.0f), y(0.0f), w(0.4f), h(0.2f), text("Button"), hovered(false), isPressed(false){}

    Button(float x, float y, float w, float h, std::string text, bool hovered, bool isPressed) : x(x), y(y), w(w), h(h), text(text), hovered(hovered), isPressed(isPressed) {}

    void draw() {

        if(isPressed) {
            glColor3f(0.6f, 0.2f, 0.2f); //clicked
        } else if(hovered) {
            glColor3f(0.8f, 0.8f, 0.8f); //hovered over
        } else {
            glColor3f(0.5f, 0.5f, 0.5f); //default
        }

        //draw rectangle
        glLineWidth(2.0f);
        glBegin(GL_POLYGON);
            glVertex2f(x - w, y - h);
            glVertex2f(x + w, y - h);
            glVertex2f(x + w, y + h);
            glVertex2f(x - w, y + h);
        glEnd();

        //draw border
        glColor3f(0.0,0.0, 0.0);
        glLineWidth(3.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(x - w, y - h);
            glVertex2f(x + w, y - h);
            glVertex2f(x + w, y + h);
            glVertex2f(x - w, y + h);
        glEnd();

        drawText(x, y, text);
    }

    void setHover(bool hovered) {
        this->hovered = hovered;
    }

    void setPressed(bool isPressed) {
        this->isPressed = isPressed;
    }

    bool isClicked(float mx, float my) {
        return mx > x - w && mx < x + w && my > y - h && my < y + h;
    }

    void drawText(float x, float y, const std::string& text) {
        glColor3f(0.0f, 0.0f, 0.0f); // Black text

        // //find width of text
        int textWidth = 0;
        for(char c : text) {
            textWidth += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }

        // //find center position
        float textX = x - (textWidth / 2.0f);
        float textY = y - 0.012;
        glRasterPos2f(x - (text.size() * 0.01f), y - 0.01f); // Center text
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    }


    ~Button() = default;
};

#endif
