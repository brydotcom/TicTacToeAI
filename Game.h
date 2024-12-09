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

    // Function to evaluate the board state
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
   
    return 0; // If no winner is found, return a neutral score of 0
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

// Function to handle player input
std::pair<int, int> getPlayerMove(const std::vector<std::vector<int>>& board) {
    int x, y;
    while (true) {
        std::cout << "Enter your move (row and column): ";
        std::cin >> x >> y; // Prompt the player to enter row and column
       
        if (x >= 0 && x < size && y >= 0 && y < size && board[x][y] == 0) {
            return {x, y}; // Return the valid move as a pair of coordinates
        } else {
            std::cout << "Invalid move. Try again." << std::endl; // Inform the player of invalid input
        }
    }
}
// Minimax function
int minimax(GameStateNode* node, int depth, bool playerTurn) {
    // Base case: check for terminal states (game over or depth limit reached)
    if (depth == 0 || node->children.empty() || gameOver) {
        return evaluateBoard(node->board, playerTurn); // Evaluate the board and return the score
    }
   
    if (playerTurn) {
        int best = -1000; // Initialize the best score for maximizing player
        for (GameStateNode* child : node->children) {
            int eval = minimax(child, depth -1, false);
            best = std::max(best, eval); // Recursively call minimax for each child
        }
        node->score = best;
        return best; // Return the best score found
    } else {
        int worst = 1000; // Initialize the best score for minimizing player
        for (auto child : node->children) {
            int eval = minimax(child, depth - 1, true);
            worst = std::min(worst, eval); // Recursively call minimax for each child
        }
        node->score = worst;
        return worst; // Return the best score found
    }
}

// Minimax with alpha-beta pruning
int minimaxAlphaBeta(GameStateNode* node, int depth, bool playerTurn, int alpha, int beta) {
    // Base case: check for terminal states (game over or depth limit reached)
    if (depth == 0 || gameOver) {
        return evaluateBoard(node->board, playerTurn); // Evaluate the board and return the score
    }
   
    if (playerTurn) {
        int best = -1000; // Initialize the best score for maximizing player
        for (auto child : node->children) {
            best = std::max(best, minimaxAlphaBeta(child, depth - 1, false, alpha, beta)); // Recursively call minimax with alpha-beta pruning
            alpha = std::max(alpha, best); // Update alpha
           
            if (beta <= alpha) {
                break; // Beta cut-off: stop exploring further if beta is less than or equal to alpha
            }
        }
        return best; // Return the best score found
    } else {
        int best = 1000; // Initialize the best score for minimizing player
        for (auto child : node->children) {
            best = std::min(best, minimaxAlphaBeta(child, depth - 1, true, alpha, beta)); // Recursively call minimax with alpha-beta pruning
            beta = std::min(beta, best); // Update beta
           
            if (beta <= alpha) {
                break; // Alpha cut-off: stop exploring further if beta is less than or equal to alpha
            }
        }
        return best; // Return the best score found
    }
}

bool checkWinTwo(const std::vector<std::vector<int>>& board, int player) {
  
   // Determine the size of the board
   
    // Check rows for a win
    for (int i = 0; i < size; ++i) {
        if (board[i][0] == player && board[i][1] == player && board[i][size -1] == player) return true; // If all cells in the row are the same and match the player's number
        if (board[0][i] == player && board[1][i] == player && board[size -1][i] == player) return true; // If all cells in the column are the same and match the player's number
    }
   
    // Check diagonals for a win
    if (board[0][0] == player && board[1][1] == player && board[size -1][size -1] == player) return true; // If all cells in the main diagonal are the same and match the player's number
    if (board[0][size -1] == player && board[1][1] == player && board[size -1][0] == player) return true; // If all cells in the anti-diagonal are the same and match the player's number
   
    return false; // No win found
}


 // Function for the AI move using minimax with alpha-beta pruning
void hardAI() {
    if (!playerX && !gameOver) { // Only proceed if it's not Player X's turn and the game is not over
        GameStateNode* rootNode = new GameStateNode(board); // Create a root node with the current board state
       
        generateGameTree(rootNode, 3, true); // Generate the game tree up to depth 3
       
        int bestScore = -1000; // Initialize the best score for the AI
        std::pair<int, int> bestMove; // Variable to store the best move
       
        for(auto child : rootNode->children) {
            int score = minimaxAlphaBeta(child, 2, false, -1000, 1000); // Evaluate each child using alpha-beta pruning
            if(score > bestScore) { // If a better score is found
                bestScore = score; // Update the best score
               
                // Find the position of the best move by comparing the child board with the current board
                for(int i = 0; i < size; ++i) {
                    for(int j = 0; j < size; ++j) {
                        if(child->board[i][j] != board[i][j]) { // Identify the cell that differs
                            bestMove = {i, j}; // Set the best move
                            break;
                        }
                    }
                }
            }
        }
       
        board[bestMove.first][bestMove.second] = 2; // AI makes its move by placing 'O' in the best position
        if (checkWin2(board, 2)) { // Check if AI has won
            setWinner(2); // Announce AI as the winner
            gameOver = true; // Set the game over flag
        } else if (isBoardFull(board)) { // Check if the board is full
            std::cout << "It's a draw!" << std::endl; // Announce a draw
            gameOver = true; // Set the game over flag
        }
        // Cleanup memory allocated for the game tree nodes
        for(auto child : rootNode->children) {
            delete child; // Delete each child node
        }
        delete rootNode; // Delete the root node

         playerX = !playerX; 
        
    }


}

    ~Game() {}
};

#endif
