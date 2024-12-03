#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <GL/gl.h>
#include "AppController.h"
#include "Rectangle.h"
#include "Button.h"
#include "Game.h" 
#include <iostream>

class Controller : public AppController {
    Game game;
    Rectangle* r;
    int count;

    Button button3;
    Button button4;
    Button button5;

    void init() {
        r = new Rectangle[count];

        float x = -0.9f;
        float y = 0.9f;
        float w = 1.8f / count;
        float h = 1.8f / count;

        for (int i = 0; i < count; i++) {
            r[i] = Rectangle(x, y, w, h);
            x += w;
        }
    }

public:

    Controller(){
        game.AIOn();
        game.playerOFirst();

        button3 = Button("3 x 3", -0.9f, -0.75f);
        button4 = Button("4 x 4", -0.3f, -0.75f);
        button5 = Button("5 x 5", 0.3f, -0.75f);

        count = 3;
        init();
        
    }

    void leftMouseDown(float x, float y) {
        game.handleMouseClick(x, y);
        
        if (button3.contains(x, y)) {
            std::cout << "Create 3 x 3" << std::endl;
            delete[] r;
            count = 3;
            init();
        }
        else if (button4.contains(x, y)) {
            std::cout << "Create 4 x 4" << std::endl;
            delete[] r;
            count = 4;
            init();
        }
        else if (button5.contains(x, y)) {
            std::cout << "Create 5 x 5" << std::endl;
            delete[] r;
            count = 5;
            init();
        }
    };

    void render(){
        game.draw();

      /*  button3.draw();
        button4.draw();
        button5.draw();

        for (int i = 0; i < count; i++) {
            r[i].draw();
        }*/
    }
};

#endif
