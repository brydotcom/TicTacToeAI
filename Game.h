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

// int board[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

public:
    Game(){
        playerX = true;
        AI = false;
        size=5;

        board.resize(size, std::vector<int>(size,0));
        
        home = Button(-0.95f, 0.95f, 0.15f, "Home", false, false);
        reset = Button(0.8f, 0.95f, 0.15f, "Retry", false, false);
        
        // drawingBoard();
        home.draw();
        reset.draw();   

        // quitGame = Button("Quit", -0.3, 0.0, 0.2, 0.2);
        // OWon = Button("Player O Won!", -0.3, 0.6, 0.2, 0.2);
        // XWon = Button("Player X Won!", -0.3, 0.6, 0.2, 0.2);
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

    void handleMouseClick(int button, int state, float x, float y) {
        int count = 0;
        // if(checkWin(playerX)) {
        //     gameOver = true;
        //     std::cout<<"Game Over!"<<std::endl;
        // } 

        //converts mouse coords to normal coords
        if(button == 0 && state == 0) {
            float normX = (2.0f * (x / glutGet(GLUT_WINDOW_WIDTH))) - 1.0f;
            float normY = 1.0f - (2.0f * (y / glutGet(GLUT_WINDOW_HEIGHT)));

            //determine square clicked
            int row = static_cast<int>((1.0f - normY) * size / 2);
            int col = static_cast<int>((normX + 1.0f) * size / 2);

            //make sure click is within bounds
            if (row >= 0 && row < size && col >= 0 && col < size) {
                //make sure cell is empty
                if(board[row][col] == 0) { //if cell is empty
                    board[row][col] = playerX ? 1 : 2;
                    playerX = !playerX; 
                    count++;
                    drawingBoard();
                    if(count == size * size) {
                        gameOver = true;
                        exit(0);
                    }
                }
            }
                    // else if(isDraw()) {
                    //     std::cout<<"It's a draw!"<<std::endl;
                    //     gameOver = true;
                    // }
        }
    glutPostRedisplay();           
}

// check if a player has won
bool checkWin(int player){
    // Check rows  for a win
    for (int i = 0; i < size; ++i) {
        bool rowWin = true;
        for(int j=1; j<size; j++) {
            if(board[i][j] != player) {
                rowWin = false;
                break;
            }
        }
        if(rowWin) {
            std::cout << "Player " << (playerX ? 2 : 1) << " wins!" << std::endl;
            return true;
        }
    }

    //check columns for a win
    for (int i = 0; i < size; ++i) {
        bool colWin = true;
        for(int j=1; j<size; j++) {
            if(board[j][i] != board[0][i] || board[0][i] == 0) {
                colWin = false;
                break;
            }
        }
        if(colWin) {
            std::cout << "Player " << (playerX ? 2 : 1) << " wins!" << std::endl;
            return true;
        }
    }

    //check diagonal win
    bool d1win = true;
    bool d2win = true;
    for(int i=1; i<size; i++) {
        if(board[i][i] != board[0][0] || board[0][0] ==0) {
            d1win = false;
        }
        if(board[i][size - i - 1] != board[0][size - 1] || board[0][size - 1] ==0) {
            d1win = false;
        }
    }

    if(d1win || d2win) {
        std::cout << "Player " << (playerX ? 2 : 1) << " wins!" << std::endl;
        return true;
    }

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

void resetGame() {
    playerX = true;
    gameOver = false;
    board.resize(size, std::vector<int>(size, 0));
    drawingBoard();
}

void easyAI() {
    if (!playerX) {
        for (int i = 0; i < size; i++) {
            for(int j=0; j<size; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = !playerX;
                    break;
                }
            }
        }
        playerX = !playerX;
    }
}

// Vec mediumAI(const Game& state, State player) {
//     for(int i=0; i<state.gameBoard.getSize(); i++) {
//         for(int j=0; j<state.gameBoard.getSize(); j++) {
//             if(state.gameBoard.getSquareState(i, j) == EMPTY) {
//                 Game temp = state;
//                 temp.gameBoard.setSquareState(i, j, player);
//                 if(temp.checkWin(player)) {
//                     return Vec(i,j);
//                 }
//             }
//         }
//     }
//     return easyAI(state);
// }

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
