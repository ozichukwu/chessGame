#ifndef CHESSAI_H
#define CHESSAI_H
#include <cctype>

using namespace std;

//---- MinimaxResult ----
struct MinimaxResult {
    int score;
    short bestMoveFromR;
    short bestMoveFromC;
    short bestMoveToR;
    short bestMoveToC;
    bool moveFound;
};

class AI {
public:
    MinimaxResult compute(const char board[8][8], int depth); //assumes it's AI's turn
};

struct M {
    short fromR[384];
    short fromC[384];
    short toR[384];
    short toC[384];
    bool promotion[384] = {}; //set all to false
    short num = 0;
};

M generateAllMoves(const char board[8][8], bool AITurn);
MinimaxResult minimax(const char board[8][8], int depth, int alpha, int beta, bool maximizingPlayer, bool aiTurn);


#endif
