#ifndef TIC_TAC_TOE_AI
#define TIC_TAC_TOE_AI

#include <iostream>
#include <cstdlib>
#include "GameState.h"
#include "Graph.h"
#include "LinkedList.h"

inline int max(int a, int b){
    return (a > b) ? a : b;
}

inline int min(int a, int b){
    return (a < b) ? a : b;
}

inline int dynamicDepth(int gridSize){
    if (gridSize <= 3){
        return 4;
    }
    return 2;
}

inline int evaluate(GameState& game, int player) {
    int score = 0;

    for (int i = 0; i < game.size; i++) {
        int playerCount = 0, opponentCount = 0;
        for (int j = 0; j < game.size; j++) {
            if (game.grid[i][j] == player){
                playerCount++;
            }
            if (game.grid[i][j] == !player){
                opponentCount++;
            }
        }
        if (playerCount > 0 && opponentCount == 0){
            score += playerCount;
        }
        if (opponentCount > 0 && playerCount == 0){
            score -= opponentCount;
        }


        playerCount = opponentCount = 0;
        for (int j = 0; j < game.size; j++) {
            if (game.grid[j][i] == player){
                playerCount++;
            }
            if (game.grid[j][i] == !player){
                opponentCount++;
            }
        }
        if (playerCount > 0 && opponentCount == 0){
            score += playerCount;
        }
        if (opponentCount > 0 && playerCount == 0){
            score -= opponentCount;
        }
    }


    int playerDiag1 = 0, opponentDiag1 = 0;
    int playerDiag2 = 0, opponentDiag2 = 0;


    for (int i = 0; i < game.size; i++) {
        if (game.grid[i][i] == player){
            playerDiag1++;
        }
        if (game.grid[i][i] == !player){
            opponentDiag1++;
        }
        if (game.grid[i][game.size - i - 1] == player){
            playerDiag2++;
        }
        if (game.grid[i][game.size - i - 1] == !player){
            opponentDiag2++;
        }
    }
    if (playerDiag1 > 0 && opponentDiag1 == 0){
        score += playerDiag1;
    }
    if (opponentDiag1 > 0 && playerDiag1 == 0){
        score -= opponentDiag1;
    }
    if (playerDiag2 > 0 && opponentDiag2 == 0){
        score += playerDiag2;
    }
    if (opponentDiag2 > 0 && playerDiag2 == 0){
        score -= opponentDiag2;
    }


    if (game.size % 2 == 1) {
        int center = game.size / 2;
        if (game.grid[center][center] == player){
            score += 3;
        }
    }
    return score;
}

const int INT_MIN = -214783648;
const int INT_MAX = 214783648;

inline int maxReward(GameState& game, int player, int depth, int alpha = INT_MIN, int beta = INT_MAX) {
    if (game.done || depth == 0) {
        return evaluate(game, player);
    }
    int reward = (game.currentTurn == player) ? INT_MIN : INT_MAX;
    for (int i = 0; i < game.size; i++) {
        for (int j = 0; j < game.size; j++) {
            GameState temp = game;
            if (temp.play(i, j)) {
                int curr = maxReward(temp, player, depth - 1, alpha, beta);
                if (game.currentTurn == player) {
                    reward = max(reward, curr);
                    alpha = max(alpha, reward);
                } else {
                    reward = min(reward, curr);
                    beta = min(beta, reward);
                }
                if (beta <= alpha) break;
            }
        }
    }
    return reward;
}

inline Vec miniMaxAI(GameState& game, int player) {
    int depth = 3;
    int bestReward = INT_MIN;
    Vec bestMove(-1, -1);
    for (int i = 0; i < game.size; i++) {
        for (int j = 0; j < game.size; j++) {
            GameState temp = game;
            if (temp.play(i, j)) {
                int reward = maxReward(temp, player, depth - 1);
                if (reward > bestReward) {
                    bestReward = reward;
                    bestMove = Vec(i, j);
                }
            }
        }
    }
    return bestMove;
}

inline Vec askHuman(GameState game){
    int x, y;
    std::cout << "Enter Coordinates (" << (game.currentTurn ? "O" : "X") << "):";
    std::cin >> x >> y;
    return Vec(x, y);
}

#endif
