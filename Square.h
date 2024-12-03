#ifndef SQUARE_H
#define SQUARE_H

#include <GL/freeglut.h>
#include "State.h"
#include <GL/gl.h>
#include <cmath>
#include <iostream>

class Square {
private:
    float x, y, size;
    State state;

public:
    Square() : x(0.0f), y(0.0f), size(0.2f), state(EMPTY) {}

    Square(float x, float y, float size) : x(x), y(y), size(size), state(EMPTY){}

    State getState() const { return state; }

    void setState(State state) { 
        this->state = state;
    }
    
    void drawBoard(int size, State state) const {

        float x = -1.0f;
        float y = -1.0f;
        float box = 2.0f / size;

        //white background
        // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //draw top line
        glColor3f(0.0f,0.0f,0.5f);
        glBegin(GL_LINES);
            glVertex2f(-1.0, 1.0f);
            glVertex2f(1.0f ,1.0f);
        glEnd();

        //vertical lines
        for(int i=1; i<size; i++) {
            float offsetX = x + i * box;
            glBegin(GL_LINES);
                glVertex2f(offsetX, y);
                glVertex2f(offsetX, y + size);
            glEnd();
        }

        //horizontal lines
        for(int i=1; i<size; i++) {
            float offsetY = y + i * box;
            glBegin(GL_LINES);
                glVertex2f(x, offsetY);
                glVertex2f(x + size, offsetY);
            glEnd();
        }
        
        //draw square and its state
        if(state == PLAYER_X) {
            for (int i = 0; i < size; ++i) {
                for(int j=0; j<size; j++) {
                    float cellWidth = 2.0f / size;
                    float x = -1.0 + j * cellWidth + cellWidth / 2.0; 
                    float y = 1.0 - i * cellWidth - cellWidth / 2.0; 
                    glColor3f(1.0,0.0,0.0);
                    glBegin(GL_LINES);
                        glVertex2f(x - 0.167 * cellWidth, y + 0.167 * cellWidth);
                        glVertex2f(x + 0.167 * cellWidth, y - 0.167 * cellWidth);
                        glVertex2f(x - 0.167 * cellWidth, y - 0.167 * cellWidth);
                    glEnd();
                }
            }
                
            std::cout << "Changing x" << std::endl;
            // glColor3f(1.0f, 0.0f, 0.0f); // Red for X
            // glBegin(GL_LINES);
            //     glVertex2f(x, y);
            //     glVertex2f(x + size, y + size);
            //     glVertex2f(x, y + size);
            //     glVertex2f(x + size, y);
            // glEnd();
        } else if (state == PLAYER_O) {
            glColor3f(0.0f, 0.0f, 1.0f); // Blue for O
            glBegin(GL_LINE_LOOP);
            for (int i = 0; i < 360; ++i) {
                float angle = i * M_PI / 180.0f;
                glVertex2f(x + size / 2 + cos(angle) * size / 3,
                           y + size / 2 + sin(angle) * size / 3);
            }
            glEnd();
        }
    }

    void reset() { state = EMPTY; }

    bool contains(float mx, float my) {
        if (mx >= x - size / 2 && mx <= x + size / 2 && my <= y - size / 2 && my >= y + size / 2) {
            return true;
        }
        return false;
    }

    float getX() const { return x; }

    float getY() const { return y; }
    
    float getSize() const { return size; }

    void setX(float x) {
        this->x = x;
    }

    void setY(float y) {
        this->y = y;
    }

    void setSize(float size) {
        this->size = size;
    }

    ~Square() = default;
};

#endif
