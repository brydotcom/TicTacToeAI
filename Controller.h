#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <GL/freeglut.h>
#include "AppController.h"
#include "Square.h"
#include "Game.h"
#include "GameBoard.h"

class Controller : public AppController {
    Game game;
    Square state;
    GameBoard gameBoard;

public:
    Controller(){
        
    }

    void render() {
        Game();
    }

    // Children can optionally define functions listed below
    void leftMouseDown(float x, float y) {
        gameBoard.handleMouseClick(x, y);
    };
    
    void leftMouseUp(float x, float y) {};

    void rightMouseDown(float x, float y) {};
    void rightMouseUp(float x, float y) {};

    void mouseMotion(float x, float y) {};
    
    void keyboardDown(unsigned char key, float x, float y) {};
    

    // void leftMouseDown(float mx, float my) {
    //     game.handleMouseClick(mx, my);
    // }

    // void sharePlayer(){
    //     game.checkWin(state.getState())
    // }

    ~Controller(){}
};

#endif
