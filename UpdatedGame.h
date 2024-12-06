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
    bool playerX, gameOver;
    bool easyAIsetting, mediumAIsetting, hardAIsetting;
    bool showAiScreen;
    Button home;
    Button reset;
    int size;
    std::vector<std::vector<int>> board;
    bool showStart, showEnd, showGame, ignoreClick;


    Button titleButton;
    Button startButton;
    Button easyAiButton;
    Button mediumAiButton;
    Button hardAiButton;
    Button noAiButton;
    // Button humanButton;
    Button threeButton;
    Button fourButton;
    Button fiveButton; 
    Button start;

public:
    Game(){
        playerX = true;
        easyAIsetting = false;
        mediumAIsetting = false;
        hardAIsetting = false;
        size=3;
        showStart = true;
        showAiScreen = false; 
        showEnd = false;
        showGame = false;
        ignoreClick = true;
        board.resize(size, std::vector<int>(size,0));
        
        //start screen buttons
        titleButton = Button(0.05f, 0.90f, 1.00f, 0.2f, "Tic-Tac-Toe", false, false);
        startButton = Button(0.0f, 0.0f, 0.40f, 0.2f, "START", false, false);
        //AI Buttons
        easyAiButton = Button(0.0f, 0.2f, 0.70f, 0.2f, "Easy AI", false, false);
        mediumAiButton = Button(0.0f, -0.1f, 0.70f, 0.2f, "Medium AI", false, false);
        hardAiButton = Button(0.0f, -0.4f, 0.70f, 0.2f, "Hard AI", false, false);
        noAiButton = Button(0.0f, -0.7f, 0.70f, 0.2f, "No AI", false, false);

        //gameboard size buttons
        threeButton = Button(0.75f, 0.90f, 0.40f, 0.2f, "3 x 3", false, false);
        fourButton = Button(0.75f, 0.90f, 0.40f, 0.2f, "4 x 4", false, false);
        fiveButton = Button(0.75f, 0.90f, 0.40f, 0.2f, "5 x 5", false, false);

        //game screen buttons
        home = Button(-0.9f, 0.95f, 0.2f, 0.15f, "Home", false, false);
        reset = Button(0.9f, 0.95f, 0.2f, 0.15f, "Retry", false, false); 
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

    void setStart(){
        showStart = true;
        showGame = false;
        showEnd = false;
        showAiScreen = false;
    }

    void setAiScreen(){
        showStart = false; 
        showAiScreen = true;
        showGame = false; 
        showEnd = false;
    }
    void setGame() {
        showStart = false;
        showGame = true;
        showEnd = false;
        showAiScreen = false;
        ignoreClick = true;
    }

    void setEnd() {
        showStart = false;
        showGame = false;
        showEnd = true;
    }

    void startScreen() {
        glClearColor(0.4f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        threeButton.draw(); 
        fourButton.draw(); 
        fiveButton.draw(); 
        titleButton.draw(); 
        startButton.draw(); 
        // humanButton.draw(); 
    }

    void AiScreen(){
        glClearColor(0.4f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        easyAiButton.draw();
        mediumAiButton.draw();
        hardAiButton.draw();
        noAiButton.draw();
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
        // home.draw();
        // reset.draw();
    }

    void updateState() {
        if(showStart) startScreen();
        else if (showAiScreen) AiScreen();
        else if (showGame) gameScreen();
        else if (showEnd) endScreen();
        glutSwapBuffers();
    }

    void handleMouseClick(int button, int state, float x, float y) {
        if(showEnd) {
            return;
        }

        //converts mouse coords to normal coords
        float normX = (2.0f * (x / glutGet(GLUT_WINDOW_WIDTH))) - 1.0f;
        float normY = 1.0f - (2.0f * (y / glutGet(GLUT_WINDOW_HEIGHT)));
        
        if(state == 0) {
            if(threeButton.isClicked(normX, normY)) {
                resizeBoard(3);
            } else if(fourButton.isClicked(normX, normY)) {
                resizeBoard(4);
            } else if(fiveButton.isClicked(normX, normY)) {
                resizeBoard(5);
            }

            if(easyAiButton.isClicked(normX, normY)){
                easyAIsetting = true;
                setGame();
            }else if (noAiButton.isClicked(normX, normY)){
                setGame();
            }else if(mediumAiButton.isClicked(normX, normY)){
                mediumAIsetting = true;
                setGame();
            }

            if(showStart) {
                if(startButton.isClicked(normX, normY)) {
                    std::cout << "Start Button Clicked!" << std::endl;
                    setAiScreen();
                }
            } 
        }
        if(button == 0 && state == 0) {
                if(showGame) {
                    if(ignoreClick) {
                        ignoreClick = false;
                        return;
                    }
                //determine square clicked
                int row = static_cast<int>((1.0f - normY) * size / 2);
                int col = static_cast<int>((normX + 1.0f) * size / 2);

                //make sure click is within bounds
                if (row >= 0 && row < size && col >= 0 && col < size) {
                    //make sure cell is empty
                    if(board[row][col] == 0) { //if cell is empty
                        board[row][col] = playerX ? 1 : 2;
                        // std:: cout << playerX;
                        playerX = !playerX; 
                        drawingBoard();

                        if(checkWin(playerX ? 2 : 1)) {
                            showEnd = true;
                            return;
                        }
                        if(isDraw()) {
                            showEnd = true;
                            std::cout<<"It's a draw!"<<std::endl;
                            return;
                        }

                        if(!playerX){
                            if(easyAIsetting){
                                easyAI();
                            }else if(mediumAIsetting){
                                mediumAI();
                            } 

                            drawingBoard();
                            if(checkWin(playerX ? 2 : 1)) {
                                showEnd = true;
                                return;
                            }
                            if(isDraw()) {
                                showEnd = true;
                                std::cout << "It's a draw" << std::endl;
                                return;
                            }
                            
                        }
                    }
                }  
            } 
            // else if(showEnd) {
            //     if(normX > -0.5 && normX < 0.5f && normY > 0.0f && normY < 0.3f) {
            //         resetGame();
            //     }
            // }  
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
        std::cout<<"Board size changed to "<<size<<"x"<<size<<std::endl;
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
            for(int j = 0; j < size; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = 2; // AI plays as 'O' (2)
                    playerX = true;  // Switch to player X after AI move.
                    return;  // Only one move at a time.
                }
            }
        }
    }
}


 void mediumAI() {
    if (!playerX && mediumAIsetting && !gameOver) {
        // Try to find a winning move
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == 0) { // Empty cell
                    board[i][j] = 2; // AI's move
                    if (checkWin(2)) { // Check if AI wins
                        playerX = true; // Switch to player's turn after AI move
                        return;
                    } else {
                        board[i][j] = 0; // Undo the move if it doesn’t win
                    }
                }
            }
        }

        // Try to block player's winning move
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == 0) { // Empty cell
                    board[i][j] = 1; // Pretend it's the player's move
                    if (checkWin(1)) { // Check if player wins
                        board[i][j] = 2; // Block the move by playing AI's move here
                        playerX = true; // Switch to player's turn after AI move
                        return;
                    } else {
                        board[i][j] = 0; // Undo the move if it doesn't block
                    }
                }
            }
        }

        // Random move as fallback if no winning or blocking move
        int x, y;
        do {
            x = rand() % size;
            y = rand() % size;
        } while (board[x][y] != 0); // Keep searching until we find an empty spot

        board[x][y] = 2; // AI makes the move
        playerX = true;  // Switch to player X turn
    }
}


    ~Game() {}

};

#endif
