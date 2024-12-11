#ifndef GAME_H
#define GAME_H

#include <GL/gl.h>
#include <iostream>
#include <cmath>
#include <GL/freeglut_std.h>
#include "Button.h"
#include <vector>
#include "Graph.h"
#include "Texture.h"
#include "LinkedList.h"
#include <utility>
#include <limits>

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
    GameStateNode(std::vector<std::vector<int>> boardState): board(boardState), score(0) {}
};

class Game{

private:
    bool playerX, gameOver, homeScreen;
    int size;
    std::vector<std::vector<int>> board;
    bool easy, medium, hard, none;
    Texture* home;
    Texture* reset;

public:
    int winner = 0; // Set to no winner
    Game(){
        playerX = true;
        size=3;
        gameOver = false;
        homeScreen = false;
        board.resize(size, std::vector<int>(size,0));

        home = new Texture(-1.0f, 1.0f, 0.15f, 0.15f, "./assets/homeScreen.png");
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
//old code in relation to first step of building turn randomizer
// switch (start.turnP()) {
//                     case 1: PlayerX(true);
//                     std::cout << "You go first" <<std::endl;
//                 break;
//                     case 2: PlayerX(false); 
//                     if (noAI() == true) {
//                     std::cout<< "Player O goes first" <<std::endl;
//                     } else
//                     std::cout << "AI goes first" <<std::endl;
//                 break;
//                     default: PlayerX(true);
//                     std::cout << "You go first" <<std::endl;
//                 }
    bool noAI() {
        return none;
    }

    bool PlayerX(bool playerX){
        return playerX;
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
                int rowC = 0, colC = 0;
            for(int j=0; j<size; j++) {
                rowC += (board[i][j] == player);
                colC += (board[j][i] == player);
                if (rowC == size || colC == size) {
    std::cout << "Player " << player << " wins!" << std::endl;
    return true;
                }
            }
        }
        //check diagonal wins
        bool d1win = true;
        bool d2win = true;
        int mainD = 0, antD = 0;

        for(int i=0; i<size; i++) {
            mainD += (board[i][i]==player);
            antD += (board[i][size - i - 1] == player);
           if (mainD == size || antD == size) {
    winner = player;
    std::cout << "Player " << player << " wins!" << std::endl;
    return true;
}

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

    // Evaluate the board state
    int evaluateBoard(const std::vector<std::vector<int>>& board, bool isPlayerX) {
     // Determine the size of the board (should be 3 for Tic-Tac-Toe)
   
        playerX = !playerX;
            // Check rows for a win
            for (int i = 0; i < size; i++) {
                if (board[i][0] == board[i][1] && board[i][1] == board[i][size - 1]) { // Check if all cells in the row are the same and not empty
                    if (board[i][0] == (isPlayerX ? 1 : 2)) return 10; // If it's a win for the player, return +10
                    else if (board[i][0] == (isPlayerX ? 2 : 1)) return -10; // If it's a win for the opponent, return -10
                }
       
        // Check columns for a win
        if (board[0][i] == board[1][i] && board[1][i] == board[size - 1][i]) { // Check if all cells in the column are the same and not empty
            if (board[0][i] == (isPlayerX ? 1 : 2)) return 10; // If it's a win for the player, return +10
            else if (board[0][i] == (isPlayerX ? 2 : 1)) return -10; // If it's a win for the opponent, return -10
        }
    }
   
    // Check diagonals for a win
    if (board[0][0] == board[1][1] && board[1][1] == board[size - 1][size - 1]) { // Check main diagonal
        if (board[0][0] == (isPlayerX ? 1 : 2)) return 10; // If it's a win for the player, return +10
        else if (board[0][0] == (isPlayerX ? 2 : 1)) return -10; // If it's a win for the opponent, return -10
    }
   
    if (board[size - 1][0] == board[1][1] && board[1][1] == board[0][size - 1]) { // Check anti-diagonal
        if (board[size - 1][0] == (isPlayerX ? 1 : 2)) return 10; // If it's a win for the player, return +10
        else if (board[size - 1][0] == (isPlayerX ? 2 : 1)) return -10; // If it's a win for the opponent, return -10
    }
   
    return 0; // If no winner is found, return neutral score 0
}

// Evaluating non-terminal states
int evaluateBoard(const std::vector<std::vector<int>>& board) {
   
    int score = 0;
    return score;
}


bool checkWin2(const std::vector<std::vector<int>>& board, int player) {
    int size = board.size();
    int winCondition = size; // all possible directions included for win

    // Check rows and columns
    for (int i = 0; i < size; i++) {
        // Check row
        bool rowWin = true;
        for (int j = 0; j < size; j++) {
            if (board[i][j] != player) {
                rowWin = false;
                break;
            }
        }
        if (rowWin) return true;
        // Check column
        bool colWin = true;
        for (int j = 0; j < size; j++) {
            if (board[j][i] != player) {
                colWin = false;
                break;
            }
        }
        if (colWin) return true;
    }

    // Check main diagonal
    bool mainDiagWin = true;
    for (int i = 0; i < size; i++) {
        if (board[i][i] != player) {
            mainDiagWin = false;
            break;
        }
    }
    if (mainDiagWin) return true;
    bool altDiagWin = true;
    for (int i = 0; i < size; i++) {
        if (board[i][size - 1 - i] != player) {
            altDiagWin = false;
            break;
        }
    }
    if (altDiagWin) return true;

    return false; // No win
}


// Function to check if the board is full
bool isBoardFull(const std::vector<std::vector<int>>& board) {
    for (const auto& row : board) {
        for (int cell : row) {
            if (cell == 0) return false; // If there's an empty cell, the board is not full
        }
    }
    return true; // All cells are filled, the board is full
}

//no longer used
// // Generate game tree with depth limit
// void generateGameTree(GameStateNode* node, int depth, bool maximizingPlayer) {
//     if (depth == 0 || checkWin2(node->board, 1) || checkWin2(node->board, 2) || isBoardFull(node->board)) {
//         return; // Stop generation at terminal state or depth limit
//     }

//     for (int i = 0; i < size; ++i) {
//         for (int j = 0; j < size; ++j) {
//             if (node->board[i][j] == 0) { // Only consider empty cells
//                 std::vector<std::vector<int>> newBoard = node->board;
//                 newBoard[i][j] = maximizingPlayer ? 2 : 1; // Simulate move
//                 GameStateNode* child = new GameStateNode(newBoard); // Create new node
//                 node->children.push_back(child); // Add child
//                 generateGameTree(child, depth - 1, !maximizingPlayer); // Recursive call
//             }
//         }
//     }
// }
// bool checkWinTwo(const std::vector<std::vector<int>>& board, int player) {
  
//    // Determine the size of the board
   
//     // Check rows for a win
//     for (int i = 0; i < size; ++i) {
//         if (board[i][0] == player && board[i][1] == player && board[i][size -1] == player) return true; // If all cells in the row are the same and match the player's number
//         if (board[0][i] == player && board[1][i] == player && board[size -1][i] == player) return true; // If all cells in the column are the same and match the player's number
//     }
   
//     // Check diagonals for a win
//     if (board[0][0] == player && board[1][1] == player && board[size -1][size -1] == player) return true; // If all cells in the main diagonal are the same and match the player's number
//     if (board[0][size -1] == player && board[1][1] == player && board[size -1][0] == player) return true; // If all cells in the anti-diagonal are the same and match the player's number
   
//     return false; // No win found
// }

void hardAI() {
    if (!playerX && !gameOver) {
        int bestScore = std::numeric_limits<int>::min();
        int bestMoveRow = -1, bestMoveCol = -1;

        // Iterate over the board to find the best possible move
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board[i].size(); ++j) {
                if (board[i][j] == 0) { // Empty cell
                    board[i][j] = 2; // Hypothetical AI move
                    GameStateNode testNode(board);
                    int score = minimax(&testNode, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), getDepth());
                    board[i][j] = 0; // Undo hypothetical move
                    if (score > bestScore) {
                        bestScore = score;
                        bestMoveRow = i;
                        bestMoveCol = j;
                    }
                }
            }
        }

        if (bestMoveRow != -1 && bestMoveCol != -1) {
            board[bestMoveRow][bestMoveCol] = 2;
            playerX = true;
        }
    }
}

// if laggy, change depth, values in min, to be smaller
int getDepth() {
    int empty = 0;
    for (const auto& row : board)
        for (int cell : row)
            if (cell == 0) ++empty;

    if (board.size() <= 3) return std::min(9, empty); //3x3
    if (board.size() <= 5) return std::min(4, empty); //4x4 and 5x5
    return std::min(4, empty); // for larger boards
}

int minimax(GameStateNode* node, bool isMaximizing, int alpha, int beta, int depth) {
    // Terminal state checks
    if (checkWin2(node->board, 2)) return 10 - depth;
    if (checkWin2(node->board, 1)) return depth - 10;
    if (isBoardFull(node->board)) return 0;
    if (depth == 0) return evaluateBoard(node->board);

    if (isMaximizing) {
        int maxEval = std::numeric_limits<int>::min();
        for (int i = 0; i < node->board.size(); ++i) {
            for (int j = 0; j < node->board[i].size(); ++j) {
                if (node->board[i][j] == 0) {
                    node->board[i][j] = 2;
                    int eval = minimax(node, false, alpha, beta, depth - 1);
                    node->board[i][j] = 0; //empties
                    maxEval = std::max(maxEval, eval);
                    alpha = std::max(alpha, eval);
                    if (beta <= alpha) return maxEval;
                }
            }
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (int i = 0; i < node->board.size(); ++i) {
            for (int j = 0; j < node->board[i].size(); ++j) {
                if (node->board[i][j] == 0) {
                    node->board[i][j] = 1;
                    int eval = minimax(node, true, alpha, beta, depth - 1);
                    node->board[i][j] = 0; //empties
                    minEval = std::min(minEval, eval);
                    beta = std::min(beta, eval);
                    if (beta <= alpha) return minEval;
                }
            }
        }
        return minEval;
    }
}


        ~Game() {} 
    };
#endif
