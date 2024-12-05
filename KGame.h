#ifndef GAME_H
#define GAME_H

#include <GL/gl.h>
#include <iostream>
#include <cmath>
#include <GL/freeglut_std.h>
#include "Button.h"
#include <vector>

struct Vec{
    int x;
    int y;

    Vec() : x(0), y(0) {}

    Vec(int x, int y) : x(x), y(y) {}

    void set(int x, int y){
        this->x = x;
        this->y = y;
    }

};

class Game{

private:
    bool playerX, AI, gameOver;
    Button home;
    Button reset;
    int size;
    std::vector<std::vector<int>> board;
    bool showStart, showEnd, showGame;
    Button three;
    Button four;
    Button five;

public:
    Game(){
        playerX = true;
        AI = false;
        size=5;
        showStart = true;
        showEnd = false;
        showGame = false;
        board.resize(size, std::vector<int>(size,0));
        
        home = Button(-0.95f, 0.95f, 0.2, 0.15f, "Home", false, false);
        reset = Button(0.8f, 0.95f, 0.2, 0.15f, "Retry", false, false);  
    }

    void drawingBoard() {
        glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //draw grid lines
        glLineWidth(10.0f);
        glColor3f(0.0f,0.0f,0.5f);
        glBegin(GL_LINES);
            for(int i=1; i<size; i++) {
                float position = -1.0 + (2.0 * i / size); //find grid position
                //vertical lines
                glVertex2f(position, -1.0f);
                glVertex2f(position,1.0f);
                //horizontal lines
                glVertex2f(-1.0, position);
                glVertex2f(1.0f ,position);
            }
        glEnd();

        //draw x and o's
        for(int i=0; i<size; i++) {
            for(int j=0; j<size; j++) {
                glLineWidth(8.0f);
                float cellWidth = 2.0f / size;
                float x = -1.0 + j * cellWidth + cellWidth / 2.0;
                float y = 1.0 - i * cellWidth - cellWidth / 2.0;

                if(board[i][j] == 1) { //1 is X
                    // std::cout<<"Player X"<< std::endl;
                    glColor3f(0.5, 0.0, 0.0);
                    glLineWidth(8.0f);
                    glBegin(GL_LINES);
                        glVertex2f(x - 0.2f * cellWidth, y + 0.2f * cellWidth); // Top-left to bottom-right diagonal of X
                        glVertex2f(x + 0.2f * cellWidth, y - 0.2f * cellWidth);
                        glVertex2f(x + 0.2f * cellWidth, y + 0.2f * cellWidth); // Top-right to bottom-left diagonal of X
                        glVertex2f(x - 0.2f * cellWidth, y - 0.2f * cellWidth);
                        glEnd();
                    glEnd();
                } else if(board[i][j] == 2) { //2 is O
                    // std::cout<<"Player O"<< std::endl;
                    glLineWidth(7.0f);
                    glColor3f(0.0, 0.5, 0.0); // Set color to blue for O
                    glBegin(GL_LINE_LOOP);
                        for (int k = 0; k < 100; ++k) {
                            float r = 0.3 * cellWidth;
                            float angle = 2 * M_PI * k / 100; // Calculate angle for circle points
                            glVertex2f(x + r * cellWidth * cos(angle), y + r * cellWidth * sin(angle)); // Circle points
                        }
                    glEnd();
                }
            }
        }
        glutSwapBuffers();
    }

    void startScreen() {
        glClearColor(0.4f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        three.draw(); //3x3 board
        four.draw(); //4x4 board
        five.draw(); //5x5 board
        
        glutSwapBuffers();
    }

    void gameScreen() {
        drawingBoard();
        home.draw();
        reset.draw(); 
    }

    void endScreen() {
        glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(isDraw()) {

        } else if(checkWin(playerX)) {

        }
        home.draw();
        reset.draw();
        glutSwapBuffers();
    }

    void updateState() {
        if(showStart) startScreen();
        else if (showGame) gameScreen();
        else if (showEnd) endScreen();
    }

    void handleMouseClick(int button, int state, float x, float y) {
        //converts mouse coords to normal coords
        if(button == 0 && state == 0) {
            float normX = (2.0f * (x / glutGet(GLUT_WINDOW_WIDTH))) - 1.0f;
            float normY = 1.0f - (2.0f * (y / glutGet(GLUT_WINDOW_HEIGHT)));

            if(showStart) {
                if(normX > -0.5 && normX < 0.5f && normY > 0.0f && normY < 0.3f) {
                    showStart = false;
                    showGame = true;
                    showEnd = false;
                }
            } else if(showGame) {
                //determine square clicked
                int row = static_cast<int>((1.0f - normY) * size / 2);
                int col = static_cast<int>((normX + 1.0f) * size / 2);

                //make sure click is within bounds
                if (row >= 0 && row < size && col >= 0 && col < size) {
                    //make sure cell is empty
                    if(board[row][col] == 0) { //if cell is empty
                        board[row][col] = playerX ? 1 : 2;
                        playerX = !playerX; 
                        drawingBoard();

                        if(checkWin(playerX ? 2 : 1)) {
                            gameOver = true;
                            return;
                        }
                        if(isDraw()) {
                            gameOver = true;
                            std::cout<<"It's a draw!"<<std::endl;
                            return;
                        }
                    }
                }  
            } else if(showEnd) {
                if(normX > -0.5 && normX < 0.5f && normY > 0.0f && normY < 0.3f) {
                    showStart = true;
                    showGame = false;
                    showEnd = false;
                    resetGame();
                }
            }
            glutPostRedisplay();
        }         
    }

    // check if a player has won
    bool checkWin(int player){
        // Check rows  for a win
        for (int i = 0; i < size; ++i) {
            bool rowWin = true;
            for(int j=0; j<size; j++) {
                if(board[i][j] != player) {
                    rowWin = false;
                    break;
                }
            }
            if(rowWin) {
                std::cout << "Player " << player << " wins!" << std::endl;
                return true;
            }
        }

        //check columns for a win
        for (int i = 0; i < size; ++i) {
            bool colWin = true;
            for(int j=0; j<size; j++) {
                if(board[j][i] != player) {
                    colWin = false;
                    break;
                }
            }
            if(colWin) {
                std::cout << "Player " << player << " wins!" << std::endl;
                return true;
            }
        }

        //check diagonal win left to right 
        bool d1win = true;
        for(int i=0; i<size; i++) {
            if(board[i][i] != player) {
                d1win = false;
                break;
            }
        }

        bool d2win = true;
        for(int i=0; i<size; i++) {
            if(board[i][size - i - 1] != player) {
                d2win = false;
                break;
            }
        }

        if(d1win || d2win) {
            std::cout << "Player " << player << " wins!" << std::endl;
            return true;
        }

        //no winner
        return false;
    }

    bool isDraw() {
        for(int i=0; i<size; i++) {
            for(int j=0; j<size; j++) {
                if(board[i][j] == 0) {
                    return false;
                }
            }
        }
        return true;
    }

    void resizeBoard(int size) {
        this->size = size;
        board.clear();
        board.resize(size, std::vector<int>(size, 0));
        resetGame();
    }

    void resetGame() {
        playerX = true;
        gameOver = false;
        board.resize(size, std::vector<int>(size, 0));
        drawingBoard();
    }

    void easyAI() {
        if (!playerX && !gameOver) {
            for (int i = 0; i < size; i++) {
                for(int j=0; j < size; j++) {
                    if (board[i][j] == 0) {
                        board[i][j] = !playerX;
                        break;
                    }
                }
            }
            playerX = !playerX;
        }
    }

    void mediumAI() {
        if(!playerX && AI && !gameOver) {
            for(int i=0; i<size; i++) {
                int x = rand() % size;
                int y = rand() % size;
                while(board[x][y] != 0) {
                    x = rand() % size;
                    y = rand() % size;
                }
                board[x][y] = !playerX;
                break;
            }
            playerX = !playerX;
        }
    }

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


    ~Game() {
    // for(int i=0; i<size; i++) {
    //     delete[] board[i];
    // }
    // delete[] board;
    }

};

#endif
