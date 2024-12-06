#ifndef BUTTON_H
#define BUTTON_H

#include "GL/freeglut_std.h"
#include <string>
#include <iostream>

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
            glColor3f(0.2f, 0.36f, 0.21f); //default
        }

        //draw rectangle
        glLineWidth(2.0f);
        glBegin(GL_POLYGON);
            glVertex2f(x - w / 2, y - h / 2);
            glVertex2f(x + w / 2, y - h / 2);
            glVertex2f(x + w / 2, y + h / 2);
            glVertex2f(x - w / 2, y + h / 2);
        glEnd();

        //draw border
        glColor3f(1.0,1.0, 1.0);
        glLineWidth(3.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(x - w / 2, y - h / 2);
            glVertex2f(x + w / 2, y - h / 2);
            glVertex2f(x + w / 2, y + h / 2);
            glVertex2f(x - w / 2, y + h / 2);
        glEnd();

        drawText(x, y, text);
    }

    void setHover(bool hovered) { this->hovered = hovered; }

    void setPressed(bool isPressed) { this->isPressed = isPressed; }

    bool isClicked(float mx, float my) {
        std::cout<<"Button is clicked"<<std::endl;
        float l = x - w / 2;
        float r = x + w / 2;
        float b = y - h / 2;
        float t = y + h / 2;
        return mx > l && mx < r && my > b && my < t;
    }

    void drawText(float x, float y, const std::string& text) {
        glColor3f(1.0f, 1.0f, 1.0f);

        // //find width of text
        int textWidth = 0;
        for(char c : text) {
            textWidth += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }

        // //find center position
        glRasterPos2f(x - textWidth / 400.0f, y - 0.01f); // Center text
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
    }

    void reset() {
        hovered = false;
        isPressed = false;
    }

    ~Button() = default;
};

#endif
