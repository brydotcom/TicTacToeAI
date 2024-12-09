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

// inline int evaluate(GameState& game, int player){
//     if (game.hasWon(player)){
//         return 10;
//     }
//     else if (game.hasWon(!player)){
//         return -10;
//     }
//     else{
//         return 0;
//     }
// }

inline int dynamicDepth(int gridSize){
    if (gridSize <= 3){
        return 4;
    }
    // if (gridSize == 4){
    //     return 3;
    // }
   
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


// inline int maxReward(Vertex<GameState>* node, int player, int depth, int alpha = -1000, int beta = 1000) {
//     if (depth == 0 || node->edgeList.size() == 0) {
//         return evaluate(node->data, player);
//     }


//     int reward = (node->data.currentTurn == player) ? -1000 : 1000;


//     for (size_t i = 0; i < node->edgeList.size(); i++) {
//         auto edge = node->edgeList[i];
//         int curr = maxReward(edge->to, player, depth - 1, alpha, beta);
//         if (node->data.currentTurn == player) {
//             reward = max(reward, curr);
//             alpha = max(alpha, reward);
//         } else {
//             reward = min(reward, curr);
//             beta = min(beta, reward);
//         }
//         if (beta <= alpha) break;
//     }


//     return reward;
// }


// inline Vec miniMaxAI(GameState& game, int player) {


//     Graph<GameState> g;
//     Vertex<GameState>* start = new Vertex<GameState>(game);
//     g.addVertex(start);


//     LinkedList<Vertex<GameState>*> toExpand;
//     toExpand.append(start);


//     while (toExpand.size() > 0) {


//         Vertex<GameState>* v = toExpand.removeFirst();


//         if (!v->data.done) {
//             for (int i = 0; i < game.size; i++) {
//                 for (int j = 0; j < game.size; j++) {


//                     GameState temp = v->data;
//                     if (temp.play(i, j)) {
//                         // if (temp.hasWon(player)){
//                         //     return Vec(i, j);
//                         // }
//                         Vertex<GameState>* u = new Vertex<GameState>(temp);
//                         g.addVertex(u);
//                         g.addDirectedEdge(v, u, 0);
//                         toExpand.append(u);
//                     }
//                 }
//             }
//         }
//     }


//     int bestReward = -1000;
//     Vec bestMove(-1, -1);


//     // int depth = dynamicDepth(game.size);


//     for (size_t i = 0; i < start->edgeList.size(); i++) {
//         auto edge = start->edgeList[i];
//         int depth = dynamicDepth(game.size);


//         int reward = maxReward(edge->to, player, depth);


//         if (reward > bestReward) {
//             bestReward = reward;
//             bestMove = edge->to->data.lastMove;
//         }
//     }


//     if (bestMove.x == -1 || bestMove.y == -1){
//         std::cerr << "AI couldn't find a valid move." << std::endl;
//     }


//     return bestMove;
// }

const int INT_MIN = -2147483648;
const int INT_MAX = 2147483647;

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

inline Vec minimaxAI(GameState& game, int player) {
    // this is to help with the 4x4 boards
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

// inline Vec pressHuman(GameState game, float x, float y){
//     int pos_x, pos_y;
//     Square** square;
    
//     square = new Square*[game.size];

//     for(int i = 0; i < game.size; i++){
//         square[i] = new Square[game.size];
//     }
    
//     for(int i = 0; i < game.size; i++){
//         for(int j = 0; j < game.size; j++){
//             if(square[i][j].contains(x, y)){
//                 pos_x = i;
//                 pos_y = j;
//             }
//         }
//     }
//     return Vec(pos_x, pos_y);
// }

inline Vec squareHuman(GameState game, int i, int j){
    return Vec(i, j);
}

#endif
