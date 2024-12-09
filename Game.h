#ifndef GAME_H
#define GAME_H

#include <GL/gl.h>
#include <iostream>
#include <cmath>
#include <GL/freeglut_std.h>
#include "Button.h"
#include <vector>
#include "Graph.h"
#include "LinkedList.h"

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

struct GameStateNode {
    std::vector<std::vector<int>> board; 
    int score;  
    std::vector<GameStateNode*> children;  

    GameStateNode(std::vector<std::vector<int>> boardState)
        : board(boardState), score(0) {}
    
    ~GameStateNode() {
        for(auto child : children) {
            delete child;
        }
    }
};

class Game{

private:
    bool playerX, gameOver, homeScreen;
    Texture* home;
    Texture* reset;
    int size;
    std::vector<std::vector<int>> board;
    bool easy, medium, hard, none;

public:
    int winner = 0; // Set to no winner
    Game(){
        playerX = true;
        size=3;
        gameOver = false;
        homeScreen = false;
        board.resize(size, std::vector<int>(size,0));
        home = new Texture(-1.0f, 1.0f, 0.15f, 0.15f, "./assets/homeSymbol.png");
        reset = new Texture(0.85, 1.0, 0.15, 0.15,"./assets/arrow.png"); 
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
                glVertex2f(1.0f, position);
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

    void unsetButtons() {
        easy = false;
        medium = false;
        hard = false;
        none = false;
    }

    void setEasyAI(bool set) { 
        unsetButtons();
        easy = set; 
    }
    void setMediumAI(bool set) { 
        unsetButtons();
        medium = set;
    }
    void setHardAI(bool set) { 
        unsetButtons();
        hard = set;
    }

    void setNoAi(bool set) {
        unsetButtons();
        none = set;
    }

    bool homeSelected() const { return homeScreen; }

    void draw() {
        drawingBoard();
        home->draw();
        reset->draw();
    }

    void handleMouseClick(int button, int state, float x, float y) {
       if(gameOver) return;

        //converts mouse coords to normal coords
        float normX = (2.0f * (x / glutGet(GLUT_WINDOW_WIDTH))) - 1.0f;
        float normY = 1.0f - (2.0f * (y / glutGet(GLUT_WINDOW_HEIGHT)));
        
        if(button == 0 && state == 0) {
            if(reset->contains(normX, normY)) {
                resetGame();
                return;
            }
            if(home->contains(normX, normY)) {
                homeScreen = true;
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

                    if(checkWin(playerX ? 2 : 1)) { //if playerX is true, it passes a 2, else it passes a 1
                        setWinner(playerX ? 2 : 1);
                        std::cout<<"mouse: "<<winner<<std::endl;
                        gameOver = true;
                        return;
                    }
                    if(isDraw()) {
                        std::cout<<"It's a draw!"<<std::endl;
                        gameOver = true;
                        return;
                    }

                    if(!playerX){
                        if(easy){
                            easyAI();
                            std::cout<<"Changing to easy AI"<<std::endl;
                        } else if(medium){
                            mediumAI();
                            std::cout<<"Changing to medium AI"<<std::endl;
                        } else if (hard){
                            hardAI();
                            std::cout<<"Changing to hard AI"<<std::endl;
                        } else if(none){
                            
                            std::cout<<"Changing to no AI"<<std::endl;
                        }
                        drawingBoard();
                        if(checkWin(playerX ? 2 : 1)) {
                            setWinner(playerX ? 2 : 1);
                            std::cout<<"mouse: "<<winner<<std::endl;
                            gameOver = true;
                            return;
                        }
                        if(isDraw()) {
                            std::cout << "It's a draw" << std::endl;
                            gameOver = true;
                            return;
                        }   
                    }
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
                winner = player;
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
                winner = player;
                std::cout << "Player " << player << " wins!" << std::endl;
                return true;
            }
        }

        //check diagonal win left to right 
        bool d1win = true;
        bool d2win = true;
        for(int i=0; i<size; i++) {
            if(board[i][i] != player) {
                d1win = false;
            }
            if(board[i][size - i - 1] != player) {
                d2win = false;
            } 
        }

        if(d1win || d2win) {
            winner = player;
            std::cout << "Player " << player << " wins!" << std::endl;
            return true;
        }

        std::cout << "No winner: "<<player<< std::endl;
        //no winner
        return false;
    }


    void setWinner(int winner) {
        this->winner = winner;
    }

    int returnWinner() {
        // this->winner = winner;
        std::cout<<"returnWinner: "<<winner<<std::endl;
        return winner;
    }

    bool isDraw() {
        for(int i=0; i<size; i++) {
            for(int j=0; j<size; j++) {
                if(board[i][j] == 0) {
                    return false;
                }
            }
        }
        if(!gameOver) {
            winner = 0;
            std::cout<<"It's a draw"<< std::endl;
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
        homeScreen = false;
        winner = 0;
        board.clear();
        board.resize(size, std::vector<int>(size, 0));
        drawingBoard();
    }

    bool gameDone() const {
        return gameOver;
    }

    void easyAI() {
        if (!playerX && !gameOver) {
            for (int i = 0; i < size; i++) {
                for(int j = 0; j < size; j++) {
                    if (board[i][j] == 0) {
                        board[i][j] = 2; // AI plays as 'O' (2)
                        if(checkWin(2)) {
                            setWinner(2);
                            gameOver = true;
                        }
                        playerX = true;  // Switch to player X after AI move.
                        return;  // Only one move at a time.
                    }
                }
            }
        }
    }

    void mediumAI() {
        if (!playerX && !gameOver) { 
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (board[i][j] == 0) { 
                        board[i][j] = 2; //player 2 prediction
                        if (checkWin(2)) { 
                            playerX = true; 
                            return;
                        } else {
                            board[i][j] = 0; 
                        }
                    }
                }
            }

            // Player X prediction        
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (board[i][j] == 0) { 
                        board[i][j] = 1; //player 1 prediction
                        if (checkWin(1)) { //sets checkWin() to true
                            board[i][j] = 2; 
                            playerX = true; 
                            return;
                        } else {
                            board[i][j] = 0; 
                        }
                    }
                }
            }
   
            int x, y;
            do {
                x = rand() % size;
                y = rand() % size;
            } while (board[x][y] != 0); 

            board[x][y] = 2; 
            playerX = true;  
        }
    }

    bool checkWin2(int player){
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
                winner = player;
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
                winner = player;
                std::cout << "Player " << player << " wins!" << std::endl;
                return true;
            }
        }

        //check diagonal win left to right 
        bool d1win = true;
        bool d2win = true;
        for(int i=0; i<size; i++) {
            if(board[i][i] != player) {
                d1win = false;
            }
            if(board[i][size - i - 1] != player) {
                d2win = false;
            } 
        }

        if(d1win || d2win) {
            winner = player;
            std::cout << "Player " << player << " wins!" << std::endl;
            return true;
        }

        std::cout << "No winner: "<<player<< std::endl;
        //no winner
        return false;
    }

    int minimax (std::vector<std::vector<int>>& board, int depth, bool maximizing){
        if (checkWin(2)) return 10 - depth;
        if (checkWin(1)) return depth - 10;
        if (isDraw()) return 0;
        //maximizing your score (picking what runs as the best reward move)
        if (maximizing) {
            int best = -1000;
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    if (board[i][j] == 0) {
                        board[i][j] = 2;
                        best = std::max(best, minimax(board, depth + 1, false));
                        board[i][j] = 0;
                    }
                }
            }
            return best;
        }

        //minimizes opponent score (so ai can assume the player picked the best possible move)
        else {
            int best = 1000;
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    if (board[i][j] == 0) {
                        board[i][j] = 1;
                        best = std::min(best, minimax(board, depth + 1, true));
                        board[i][j] = 0;
                    }
                }
            }
            return best;
        }
    }

    void hardAI(){
        if(!playerX && !gameOver){
            int highestValue = -1000;
            int bestRow = -1;
            int bestColumn = -1;
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (board[i][j] == 0) {  
                        board[i][j] = 2;
                        int moveVal = minimax(board, 0, false);  
                        board[i][j] = 0; 
                        if (moveVal > highestValue) {
                            bestRow = i;
                            bestColumn = j;
                            highestValue = moveVal;
                        }
                    }
                }
            }   
            board[bestRow][bestColumn] = 2;  
            playerX = true;  
            drawingBoard();

            if (checkWin(2)) {  
                return;
            }
            if (isDraw()) {  
                std::cout << "It's a draw" << std::endl;
                return;
            }
        }
    }
 
    ~Game() {}
};

#endif
