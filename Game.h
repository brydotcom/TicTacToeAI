#ifndef GAME_H
#define GAME_H

#include <GL/gl.h>
#include <iostream>
#include "GameBoard.h"
#include "State.h"
#include "Square.h"
#include "Button.h"
#include <vector>

struct Vec{
    int x;
    int y;

    Vec(){
        x = 0;
        y = 0;
    }

    Vec(int x, int y){
        this->x = x;
        this->y = y;
    }

    void set(int x, int y){
        this->x = x;
        this->y = y;
    }

};

class Game{

enum GameState {MENU, PLAYING, END};

private:
GameBoard gameBoard;
State currentPlayer;
GameState gs;
bool playerX, AI;
Button home;
Button reset;

//check if a player has won
bool checkWin(State player){
    int size = gameBoard.getSize();
    int count;
    //horizontal win
    for(int i=0; i<size; i++) {
        count=0;
        for(int j=0; j<size; j++) {
            if(gameBoard.getSquareState(i,j)==player){
                size++;
                std::cout<<"size = "<<size<<std::endl;
            }
            if(count==size){
                std::cout<<player<<" won!"<<std::endl;
                return true;
            }
        }
    }
    //vertical win
    for(int i=0; i<size; i++) {
        count=0;
        for(int j=0; j<size; j++) {
            if(gameBoard.getSquareState(i,j)==player){
                size++;
                std::cout<<"size = "<<size<<std::endl;
            }
            if(count==size){
                std::cout<<player<<" won!"<<std::endl;
                return true;
            }
        }
    }
    //left diagonal win
    count=0;
    for(int i=0; i<size; i++) {
        if(gameBoard.getSquareState(i,i)==player){
            count++;
            std::cout<<"size = "<<size<<std::endl;
        }
        if(count==size) {
            std::cout<<player<<" won!"<<std::endl;
            return true;
        }
    }
    //right diagonal win
    count = 0;
    for(int i=0; i<size; i++) {
        if(gameBoard.getSquareState(i,i)==player){
            count++;
            std::cout<<"size = "<<size<<std::endl;
        }
        if(count==size) {
            std::cout<<player<<" won!"<<std::endl;
            return true;
        }
    }

    return false;
}

void resetGame() {
    gameBoard.reset();
    gs = PLAYING;
    playerX = true;
}

Vec easyAI(const Game& state) {
    std::vector<Vec> available;
    for(int i=0; i< state.gameBoard.getSize(); i++) {
        for(int j=0; j < state.gameBoard.getSize(); j++) {
            if(state.gameBoard.getSquareState(i, j) == EMPTY) {
                available.push_back(Vec(i,j));
            }
        }
    }
    return available[rand() % available.size()]; //picks a random move
}

Vec mediumAI(const Game& state, State player) {
    for(int i=0; i<state.gameBoard.getSize(); i++) {
        for(int j=0; j<state.gameBoard.getSize(); j++) {
            if(state.gameBoard.getSquareState(i, j) == EMPTY) {
                Game temp = state;
                temp.gameBoard.setSquareState(i, j, player);
                if(temp.checkWin(player)) {
                    return Vec(i,j);
                }
            }
        }
    }
    return easyAI(state);
}

public:
// void selectThree(){
//     unPressButtons();
//     button3.setDown(true);
//     std::cout<<"Will change to 3x3"<<std::endl;
//     for(int i=0; i<size; i++){
//         delete[] r[i];
//     }
//     delete[] r;
//     size=3;
//     init();
// }

// void selectFour(){
//     unPressButtons();
//     button4.setDown(true);
//     std::cout<<"Will change to 4x4"<<std::endl;
//     for(int i=0; i<size; i++){
//         delete[] r[i];
//     }
//     delete[] r;
//     size=4;
//     init();
// }

// void selectFive(){
//     unPressButtons();
//     button5.setDown(true);
//     std::cout<<"Will change to 5x5"<<std::endl;
//     for(int i=0; i<size; i++){
//         delete[] r[i];
//     }
//     delete[] r;
//     size=5;
//     init();
// }

void gameStart(){
    resetGame();
}

bool getPlayer(){
    return playerX;
}

void changePlayer(bool playerX) {
    this->playerX = playerX;
}

void makeMove(int r, int c) {
    if(gameBoard.getSquareState(r, c) == EMPTY) {
        // gameBoard.setSquareState(r, c, playerX ? PLAYER_X : PLAYER_O);
        if(checkWin(playerX ? PLAYER_X : PLAYER_O)) {
            gs = END;
        } else {
            playerX = !playerX;
        }
    }
}

void draw(){
    if(gs == PLAYING) {
        gameBoard.draw();
    }
    if(gs == END){
        if(checkWin(PLAYER_X)){
            // XWon.draw();
        } else if(checkWin(PLAYER_O)){
            // OWon.draw();
        } else {
            // Tie.draw();
        }
    }
}

    Game(){
        playerX = true;
        AI = false;
        GameBoard boards(-1,-1.1,3, playerX);
        boards.draw();

        home = Button(-0.95f, 0.95f, 0.15f, "Home", false, false);
        reset = Button(0.8f, 0.95f, 0.15f, "Retry", false, false);
        home.draw();
        reset.draw();
        // int x=-1;
        // int y=-1;
        // int size=3;
        // glColor3f(1.0f, 0.0f, 0.0f); // Red for X
        //     glBegin(GL_LINES);
        //     glVertex2f(x, y);
        //     glVertex2f(x + size, y + size);
        //     glVertex2f(x, y + size);
        //     glVertex2f(x + size, y);
        //     glEnd();

        //     glColor3f(0.0f, 0.0f, 1.0f); // Blue for O
        //     glBegin(GL_LINE_LOOP);
        //     for (int i = 0; i < 360; ++i) {
        //         float angle = i * M_PI / 180.0f;
        //         glVertex2f(x + size / 2 + cos(angle) * size / 3,
        //                    y + size / 2 + sin(angle) * size / 3);
        //     }
        //     glEnd();

        glFlush();

        // quitGame = Button("Quit", -0.3, 0.0, 0.2, 0.2);
        // OWon = Button("Player O Won!", -0.3, 0.6, 0.2, 0.2);
        // XWon = Button("Player X Won!", -0.3, 0.6, 0.2, 0.2);
    }

};

#endif
