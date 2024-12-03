#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "GameState.h"
#include "Square.h"
#include "State.h"
#include <GL/freeglut_std.h>
#include <iostream>

class GameBoard {
private:
Square** board;
State state;
float x, y;
int size;
bool currentPlayer;

public:
GameBoard() : GameBoard(0.0f, 0.0f, 3.0f, false){
    initialBoard(size);
}

GameBoard(float x, float y, float size, bool currentPlayer) {
    this->x = x;
    this->y = y;
    this->size = size;
    initialBoard(size);
    this->currentPlayer = currentPlayer;
}

State getSquareState(int r, int c) const {
    return board[r][c].getState();
}

void setSquareState(int r, int c, State state) const {
    board[r][c].setState(state);
}

int getSize(){
    return size;
}

void setSize(int size){
    this->size = size;
}

void initialBoard(int size){
    float squareSize = 2.0f / size;
    float intX = -1.0f; //starts at left edge
    float intY = 1.0f; //starts at top edge

    board = new Square*[size];
    for(int i=0; i<size; i++) {
        board[i] = new Square[size];
        for(int j=0; j<size; j++) {
            float squareX = intX + j * squareSize;
            float squareY = intY - (i+1) * squareSize;
            board[i][j] = Square(squareX, squareY, squareSize);
            board[i][j].setState(EMPTY);
        }
    }
}

int getSize() const {
    return size;
}

void reset() {
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            board[i][j].setState(EMPTY);
        }
    }
}

void draw() {
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            board[i][j].drawBoard(3, state);
        }
    }
}

void handleMouseClick(float x, float y) {
    //converts mouse coords to normal coords
    float normX = (2.0f * (x / glutGet(GLUT_WINDOW_WIDTH))) - 1.0f;
    float normY = 1.0f - (2.0f * (y / glutGet(GLUT_WINDOW_HEIGHT)));

    //determine square clicked
    int row = static_cast<int>((1.0f - normY) / (2.0f / size));
    int col = static_cast<int>((normX - 1.0f) / (2.0f / size));

    std::cout<<"Mouse CLick ("<<x<<", "<<y<<")"<<std::endl;
    std::cout<<"Normal Coord ("<<normX<<", "<<normY<<")"<<std::endl;
    std::cout<<"Row: "<<row<<", Col: "<<col<<std::endl;

    bool clicked = false;

    //make sure click is within bounds
    for(int r=0; r<size; r++) {
        for(int c=0; c<size; c++) {
            Square& square = board[r][c];
            // std::cout<<"Checking Square [Row: "<<r<<", Col: "<<c<<"]"<<std::endl;
            // std::cout<<"Square bounds: x="<<square.getX()<<" to  "<<square.getX()+square.getSize()<<std::endl;
            // std::cout<<"y="<<square.getY()<<" to  "<<square.getY()+square.getSize()<<std::endl;
            if(normX >= square.getX() && normX <= square.getX() + square.getSize() && normY >= square.getY() && normY <= square.getY() + square.getSize()) {
                //check if square is empty
                if(square.getState() == EMPTY) {
                    std::cout<<"Square clicked: [Row: "<<r<<", Col: "<<c<<std::endl;
                    //update square state based on who clicked it
                    square.setState(PLAYER_X);
                    

                    // currentPlayer = !currentPlayer;

                    clicked = true;
                    // //check for win or draw
                    // game.checkWin();

                    // //switch players
                    // game.changePlayer(!game.getPlayer());
                } else {
                    std::cout<<"Square is already occupied!"<<std::endl;
                }
                return;
            } 
        }
    }
    if(!clicked) {
        std::cout<<"Clicked outside board"<<std::endl;
    }
}

~GameBoard() {
    for(int i=0; i<size; i++) {
        delete[] board[i];
    }
    delete[] board;
}

};

#endif
