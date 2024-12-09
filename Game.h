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
#include <climits>
#include <utility>

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

    inline int max(int a, int b) {
        return (a>b) ? a : b;
    }

    inline int min(int a, int b) {
        return (a<b) ? a : b;
    }

    inline int dynamicDepth(int size) {
        if(size <=3) {
            return 4;
        }
        return 2;
    }

    // Function to evaluate the board state
    int evaluateBoard(const std::vector<std::vector<int>>& board, bool isPlayerX) {
        int score = 0;
        // Check rows and columns for a win
        for (int i = 0; i < size; i++) {
            int playerCount = 0;
            int oppCount = 0;
            for(int j=0; j < size; j++) {
                if (board[i][j] == isPlayerX) {
                    playerCount++;
                } 
                else if (board[i][j] == isPlayerX) {
                    oppCount++;
                } 
            }
            if(playerCount > 0 && oppCount == 0) {
                score += playerCount;
            }
            if(oppCount > 0 && playerCount == 0) {
                score -= oppCount;
            }

            playerCount = 0;
            oppCount = 0;
            for(int j=0; j < size; j++) {
                if (board[j][i] == isPlayerX) {
                    playerCount++;
                } 
                else if (board[j][i] == isPlayerX) {
                    oppCount++;
                } 
            }
            if(playerCount > 0 && oppCount == 0) {
                score += playerCount;
            }
            if(oppCount > 0 && playerCount == 0) {
                score -= oppCount;
            }
        }

        int player1Diagonal = 0, opp1Diagonal = 0;
        int player2Diagonal = 0, opp2Diagonal = 0;
    
        // Check diagonals for a win
        for(int i=0; i< size; i++) {
            if(board[i][i] == isPlayerX) {
                player1Diagonal++;
            }
            if(board[i][i] == !isPlayerX) {
                opp1Diagonal++;
            }
            if(board[i][size - 1 - i] == isPlayerX) {
                player2Diagonal++;
            }
            if(board[i][size - 1 - i] == !isPlayerX) {
                opp2Diagonal++;
            }
        }
        if(player1Diagonal > 0 && opp1Diagonal == 0) {
            score+=player1Diagonal;
        }
        if(opp1Diagonal > 0 && player1Diagonal == 0) {
            score+=opp1Diagonal;
        }
        if(player2Diagonal > 0 && opp2Diagonal == 0) {
            score+=player2Diagonal;
        }
        if(opp2Diagonal > 0 && player2Diagonal == 0) {
            score+=opp2Diagonal;
        }

        if(size % 2 == 1) {
            int mid = size / 2;
            if(board[mid][mid] == isPlayerX) {
                score +=3;
            }
        }
    
        return score;
    }

    // Function to generate the game tree
    void generateGameTree(GameStateNode* node, int depth, bool maximizingPlayer) {
        if (depth == 0 || gameOver) { // Base case: stop if depth is 0 or game is over
            return;
        }
    
        // Determine the size of the board
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (node->board[i][j] == 0) { // If the cell is empty
                    std::vector<std::vector<int>> newBoard = node->board;
                    newBoard[i][j] = maximizingPlayer ? 2 : 1; // Assigns the player's number to the board
                
                    GameStateNode* child = new GameStateNode(newBoard); // Create a new game state node
                    node->children.push_back(child); // Add the child node to the current node's children
                    generateGameTree(child, depth - 1, !maximizingPlayer); // Recursively generate the tree for the child node
                }
            }
        }
    }

    bool checkWin2(const std::vector<std::vector<int>>& board, int player) {
        // Determine the size of the board
    
        // Check rows for a win
        for (int i = 0; i < size; i++) {
            if (board[i][0] == player && board[i][1] == player && board[i][size -1] == player) return true; // If all cells in the row are the same and match the player's number
            if (board[0][i] == player && board[1][i] == player && board[size -1][i] == player) return true; // If all cells in the column are the same and match the player's number
        }
    
        // Check diagonals for a win
        if (board[0][0] == player && board[1][1] == player && board[size-1][size -1] == player) return true; // If all cells in the main diagonal are the same and match the player's number
        if (board[0][size -1] == player && board[1][1] == player && board[size -1][0] == player) return true; // If all cells in the anti-diagonal are the same and match the player's number
    
        return false; // No win found
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

    int maxReward(GameStateNode* node, int depth, bool playerTurn, int alpha = INT_MIN, int beta = INT_MAX) {
        // Base case: check for terminal states (game over or depth limit reached)
        if (depth == 0 || gameOver || isBoardFull(node->board)) {
            return evaluateBoard(node->board, playerTurn); // Evaluate the board and return the score
        }

        int reward = playerTurn ? INT_MIN : INT_MAX;
        for (int i=0; i< size; i++) {
                for(int j=0; j<size; j++) {
                    if(node->board[i][j]) {
                        int current = maxReward(node, depth - 1, playerTurn, alpha, beta);
                        if(node->board[i][j] == playerTurn) {
                            reward = max(reward, current);
                            alpha = max(alpha, reward);
                        } else {
                            reward = min(reward, current);
                            alpha = min(alpha, reward);
                        }
                        if(beta <= alpha) {
                            break;
                        }
                    }
                }
                }
                return reward;
    
        // if (playerTurn) {
        //     int maximumReward = INT_MIN;
        //     for (int i=0; i< size; i++) {
        //         for(int j=0; j<size; j++) {
        //         if(node->board[i][j] == 0) {
        //             node->board[i][j] = 2;
        //             int current = maxReward(node, depth - 1, false, alpha, beta);
        //             node->board[i][j] = 0;
        //             maximumReward = max(maximumReward, current);
        //             alpha = max(alpha, maximumReward);
        //                 if(beta <= alpha) {
        //                     break;
        //                     }
        //             }
        //         }
        //     }
        //     return maximumReward;
        // } else {
        //     int minimumReward = INT_MAX;
        //     for (int i=0; i< size; i++) {
        //         for(int j=0; j<size; j++) {
        //             if(node->board[i][j] == 0) {
        //                 node->board[i][j] = 1;
        //                 int current = maxReward(node, depth - 1, false, alpha, beta);
        //                 node->board[i][j] = 0;
        //                 minimumReward = min(minimumReward, current);
        //                 alpha = min(alpha, minimumReward);
        //                 if(beta <= alpha) {
        //                     break;
        //                 }
        //             }
        //         }
        //     }
        // return minimumReward;
        // }             
    }

    // Minimax function
    int minimaxAlphaBeta(GameStateNode* node, int depth, bool playerTurn, int alpha, int beta) {
        // Base case: check for terminal states (game over or depth limit reached)
        if (depth == 0 || isBoardFull(node->board) || checkWin2(node->board, 1) || checkWin2(node->board, 2)) {
            return evaluateBoard(node->board, playerTurn); // Evaluate the board and return the score
        }

        if(playerTurn) {
            int maximumReward = INT_MIN;
            for (auto& child : node->children) {
                int current = minimaxAlphaBeta(child, depth - 1, false, alpha, beta);
                maximumReward = max(maximumReward, current);
                alpha = max(alpha, maximumReward);
                if(beta <= alpha) {
                    break;
                }
            }
            return maximumReward;
        } else {
            int minimumReward = INT_MAX;
            for (auto& child : node->children) {
                int current = minimaxAlphaBeta(child, depth - 1, false, alpha, beta);
                minimumReward = min(minimumReward, current);
                alpha = min(alpha, minimumReward);
                if(beta <= alpha) {
                    break;
                }
            }
            return minimumReward;
        }
    }

    // Function for the AI move using minimax with alpha-beta pruning
    void hardAI() {
        if (!playerX && !gameOver) { // Only proceed if it's not Player X's turn and the game is not over
            GameStateNode* root = new GameStateNode(board); // Create a root node with the current board state
            int depth = dynamicDepth(size);
            generateGameTree(root, depth, true); // Generate the game tree up to depth 3
        
            int bestScore = INT_MIN; // Initialize the best score for the AI
            GameStateNode* bestMove = nullptr; // Variable to store the best move
        
            for(auto child : root->children) {
                int score = minimaxAlphaBeta(child, depth - 1, true, INT_MIN, INT_MAX); 
                if(score > bestScore) { // If a better score is found
                    bestScore = score; // Update the best score
                    bestMove = child;
                }
            }
                
            if(bestMove) {
                board = bestMove->board;
            }
        
            playerX = true;
            delete root;        
        }
    }

    ~Game() {}
};

#endif
