#include "chessAI.h"

// Class member definitions
MinimaxResult AI::compute(const char board[8][8], int depth) {
    return  minimax(board, depth, -1000000, 1000000, true, true);
}


// Declarations
void addMove(M& m, short fr, short fc, short tr, short tc, bool promote = false);
bool isInside(int r, int c);
M generatePawnMoves(const char board[8][8], int r, int c);
M generateKnightMoves(const char board[8][8], int r, int c);
M generateSlidingMoves(const char board[8][8], int r, int c, int dR, int dC);
M generateBishopMoves(const char board[8][8], int r, int c);
M generateRookMoves(const char board[8][8], int r, int c);
M generateQueenMoves(const char board[8][8], int r, int c);
M generateKingMoves(const char board[8][8], int r, int c);
bool isKingInCheck(const char board[8][8], bool aiKing);
M filterLegalMoves(const char board[8][8], bool aiTurn);
M generateAllMoves(const char board[8][8], bool aiTurn);
void applyMove(const char board[8][8], char newBoard[8][8],
    short fromRow, short fromCol, short toRow, short toCol, bool promote = false);
int evaluateBoard(const char board[8][8]);
MinimaxResult minimax(const char board[8][8], int depth, int alpha, int beta, bool maximizingPlayer, bool aiTurn);


//---- Move generation ----
void addMove(M& m, short fr, short fc, short tr, short tc, bool promote) {
    m.fromR[m.num] = fr;
    m.fromC[m.num] = fc;
    m.toR[m.num] = tr;
    m.toC[m.num] = tc;
    m.promotion[m.num] = promote;
    m.num++;
}

bool isInside(int r, int c) { return (r >= 0 && r < 8 && c >= 0 && c < 8); }

// ---------- Pawn Moves ----------------
M generatePawnMoves(const char board[8][8], int r, int c) {
    M moves;
    char pawn = board[r][c];
    bool isAI = isupper(pawn); // AI pieces are uppercase.
    int direction = isAI ? 1 : -1; // AI moves down, opponent moves down.
    int startRow = isAI ? 1 : 6;   // Starting row for double moves.

    int forwardRow = r + direction;
    if (isInside(forwardRow, c) && board[forwardRow][c] == '.') {
        bool promotion = (isAI && forwardRow == 7) || (!isAI && forwardRow == 0);
        addMove(moves, r, c, forwardRow, c, promotion);

        // Double move from starting position.
        if (r == startRow) {
            int doubleRow = r + 2 * direction;
            if (isInside(doubleRow, c) && board[doubleRow][c] == '.')
                addMove(moves, r, c, doubleRow, c);
        }
    }

    // Diagonal captures.
    for (int dc = -1; dc <= 1; dc += 2) {
        int captureCol = c + dc;
        if (isInside(forwardRow, captureCol)) {
            char target = board[forwardRow][captureCol];
            if (target != '.' && ((isAI && islower(target)) || (!isAI && isupper(target)))) {
                bool promotion = (isAI && forwardRow == 7) || (!isAI && forwardRow == 0);
                addMove(moves, r, c, forwardRow, captureCol, promotion);
            }
        }
    }
    return moves;
}

// ---------- Knight Moves ----------------
M generateKnightMoves(const char board[8][8], int r, int c) {
    M moves;
    char knight = board[r][c];
    bool isAI = isupper(knight);
    int knightOffsets[8][2] = { {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
                                {1, -2}, {1, 2}, {2, -1}, {2, 1} };
    for (int i = 0; i < 8; i++) {
        int newRow = r + knightOffsets[i][0];
        int newCol = c + knightOffsets[i][1];
        if (isInside(newRow, newCol)) {
            char target = board[newRow][newCol];
            if (target == '.' || (isAI && islower(target)) || (!isAI && isupper(target)))
                addMove(moves, r, c, newRow, newCol);
        }
    }
    return moves;
}

// ------------ Sliding Moves Helper (for bishops, rooks, queens) --------------
M generateSlidingMoves(const char board[8][8], int r, int c, int dR, int dC) {
    M moves;
    char piece = board[r][c];
    bool isAI = isupper(piece);
    int newRow = r + dR;
    int newCol = c + dC;
    while (isInside(newRow, newCol)) {
        char target = board[newRow][newCol];
        if (target == '.') addMove(moves, r, c, newRow, newCol);
        else {
            if ((isAI && islower(target)) || (!isAI && isupper(target)))
                addMove(moves, r, c, newRow, newCol);
            break; // Blocked by any piece.
        }
        newRow += dR;
        newCol += dC;
    }
    return moves;
}

// --------- Bishop Moves -----------
M generateBishopMoves(const char board[8][8], int r, int c) {
    M moves;
    int directions[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
    for (int i = 0; i < 4; i++) {
        M temp = generateSlidingMoves(board, r, c, directions[i][0], directions[i][1]);
        for (int j = 0; j < temp.num; j++)
            addMove(moves, temp.fromR[j], temp.fromC[j], temp.toR[j], temp.toC[j], temp.promotion[j]);
    }
    return moves;
}

// --------------------------
// Rook Moves
// --------------------------
M generateRookMoves(const char board[8][8], int r, int c) {
    M moves;
    int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
    for (int i = 0; i < 4; i++) {
        M temp = generateSlidingMoves(board, r, c, directions[i][0], directions[i][1]);
        for (int j = 0; j < temp.num; j++)
            addMove(moves, temp.fromR[j], temp.fromC[j], temp.toR[j], temp.toC[j], temp.promotion[j]);
    }
    return moves;
}

// --------------------------
// Queen Moves (Bishop + Rook)
// --------------------------
M generateQueenMoves(const char board[8][8], int r, int c) {
    M moves;
    M bishopMoves = generateBishopMoves(board, r, c);
    for (int i = 0; i < bishopMoves.num; i++)
        addMove(moves, bishopMoves.fromR[i], bishopMoves.fromC[i],
            bishopMoves.toR[i], bishopMoves.toC[i], bishopMoves.promotion[i]);

    M rookMoves = generateRookMoves(board, r, c);
    for (int i = 0; i < rookMoves.num; i++)
        addMove(moves, rookMoves.fromR[i], rookMoves.fromC[i],
            rookMoves.toR[i], rookMoves.toC[i], rookMoves.promotion[i]);
    return moves;
}

// --------------------------
// King Moves
// --------------------------
M generateKingMoves(const char board[8][8], int r, int c) {
    M moves;
    char king = board[r][c];
    bool isAI = isupper(king);
    int offsets[8][2] = { {-1, -1}, {-1, 0}, {-1, 1},
                          { 0, -1},          { 0, 1},
                          { 1, -1}, { 1, 0}, { 1, 1} };
    for (int i = 0; i < 8; i++) {
        int newRow = r + offsets[i][0];
        int newCol = c + offsets[i][1];
        if (isInside(newRow, newCol)) {
            char target = board[newRow][newCol];
            if (target == '.' || (isAI && islower(target)) || (!isAI && isupper(target)))
                addMove(moves, r, c, newRow, newCol);
        }
    }
    // Note: Castling is not implemented in this simple version.
    return moves;
}

// ------ isKingInCheck ---------
// 'aiKing' indicates which side's king we are checking.
bool isKingInCheck(const char board[8][8], bool aiKing) {
    char king = 'K'; //symbol of ai's (or opponent's, if aiKing == false) king 
    char queen = 'q'; //symbol of ai's opponent piece (depends on aiKing)
    char knight = 'n';
    char bishop = 'b';
    char rook = 'r';
    char pawn = 'p';
    if (!aiKing) { king = 'k'; queen = 'Q'; knight = 'N'; bishop = 'B'; rook = 'R'; pawn = 'P'; }

    //find location of king
    int r = -1;
    int c = -1;
    for (int i = 0; i < 64; i++) {
        if (board[i / 64][i % 64] == king) {
            r = i / 8;
            c = i % 8;
            break;
        }
    }
    if (r == -1 || c == -1) return true; //king not found, assume checkmate

    //check for knight threats
    int knightDir[8][2] = { {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {2, -1}, {2, 1}, {1, -2}, {1, 2} };
    for (int i = 0; i < 8; i++) {
        int dr = r + knightDir[i][0];
        int dc = c + knightDir[i][1];
        if (dr < 0 || dr >= 8 || dc < 0 || dc >= 8) continue;
        if (board[dr][dc] == knight) return true;
    }
    //check for pawn threats
    int pawnDir[2][2] = { {1, -1}, {1, 1} };
    if (!aiKing) pawnDir[0][0] = pawnDir[1][0] = -1;
    for (int i = 0; i < 2; i++) {
        int dr = r + pawnDir[i][0];
        int dc = c + pawnDir[i][1];
        if (dr < 0 || dr >= 8 || dc < 0 || dc >= 8) continue;
        if (board[dr][dc] == pawn) return true;
    }
    //check for bishop/queen threats
    int bishopDir[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
    for (int i = 0; i < 4; i++) {
        int dr = r;
        int dc = c;
        while (1) {
            dr += bishopDir[i][0];
            dc += bishopDir[i][1];
            if (dr < 0 || dr >= 8 || dc < 0 || dc >= 8) break;
            char piece = board[dr][dc];
            if (piece != '.') {
                if (board[dr][dc] == bishop || board[dr][dc] == queen) return true;
                break;
            }
        }
    }
    //check for rook/queen threats
    int rookDir[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
    for (int i = 0; i < 4; i++) {
        int dr = r;
        int dc = c;
        while (1) {
            dr += rookDir[i][0];
            dc += rookDir[i][1];
            if (dr < 0 || dr >= 8 || dc < 0 || dc >= 8) break;
            char piece = board[dr][dc];
            if (piece != '.') {
                if (board[dr][dc] == rook || board[dr][dc] == queen) return true;
                break;
            }
        }
    }
    return false;
}

// Filters pseudo-legal moves into legal moves (i.e. those that do not leave the king in check).
M filterLegalMoves(const char board[8][8], bool aiTurn) {
    M pseudoMoves = generateAllMoves(board, aiTurn);
    M legalMoves;
    char newBoard[8][8];

    // For each pseudo-legal move, simulate the move and check if the king is in check.
    for (int i = 0; i < pseudoMoves.num; i++) {

        applyMove(board, newBoard, pseudoMoves.fromR[i], pseudoMoves.fromC[i],
            pseudoMoves.toR[i], pseudoMoves.toC[i], pseudoMoves.promotion[i]);

        if (!isKingInCheck(newBoard, aiTurn)) {
            addMove(legalMoves, pseudoMoves.fromR[i], pseudoMoves.fromC[i],
                pseudoMoves.toR[i], pseudoMoves.toC[i], pseudoMoves.promotion[i]);
        }
    }
    return legalMoves;
}

// ---------- All Moves ----------------
M generateAllMoves(const char board[8][8], bool aiTurn) {
    M moves;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {

            char piece = board[r][c];
            if (piece == '.') continue;

            bool AIPiece = isupper(piece);
            if (aiTurn != AIPiece) continue;

            M temp;
            char lowerCasePiece = tolower(piece);
            if (lowerCasePiece == 'p') temp = generatePawnMoves(board, r, c);
            else if (lowerCasePiece == 'n') temp = generateKnightMoves(board, r, c);
            else if (lowerCasePiece == 'b') temp = generateBishopMoves(board, r, c);
            else if (lowerCasePiece == 'r') temp = generateRookMoves(board, r, c);
            else if (lowerCasePiece == 'q') temp = generateQueenMoves(board, r, c);
            else if (lowerCasePiece == 'k') temp = generateKingMoves(board, r, c);

            // Append temp moves into allMoves.
            for (int i = 0; i < temp.num; i++) {
                addMove(moves, temp.fromR[i], temp.fromC[i], temp.toR[i], temp.toC[i], temp.promotion[i]);
            }
        }
    }
    return moves;
}

// ------- End Move Generation ----------

// ----- Move Application --------
// Applies a move on a board by copying the board into newBoard and then making the move.
void applyMove(const char board[8][8], char newBoard[8][8],
    short fromRow, short fromCol, short toRow, short toCol, bool promote) {

    // Copy the board.
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            newBoard[r][c] = board[r][c];
        }
    }
    char piece = newBoard[fromRow][fromCol];
    newBoard[toRow][toCol] = piece;
    newBoard[fromRow][fromCol] = '.';
    if (promote) {
        // Promote pawn to queen.
        newBoard[toRow][toCol] = islower(piece) ? 'q' : 'Q';
    }
}

// ------- Evaluation Function ----------
// A simple evaluation function based on material. (Values are arbitrary)
int evaluateBoard(const char board[8][8]) {
    int score = 0;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            char piece = board[r][c];
            if (piece == '.') continue;
            int value = 0;
            switch (tolower(piece)) {
            case 'p': value = 100; break;
            case 'n': value = 320; break;
            case 'b': value = 330; break;
            case 'r': value = 500; break;
            case 'q': value = 900; break;
            case 'k': value = 20000; break;
            }
            score += isupper(piece) ? value : -value;
        }
    }

    // King safety penalty:
    // Subtract a penalty if white's king is in check, add a penalty if black's king is in check.
    int CHECK_PENALTY = 50; // Adjust this value based on tuning.
    if (isKingInCheck(board, true)) {
        score -= CHECK_PENALTY;
    }
    if (isKingInCheck(board, false)) {
        score += CHECK_PENALTY;
    }

    return score;
}


// -------- Minimax with Alpha–Beta Pruning -----------
MinimaxResult minimax(const char board[8][8], int depth, int alpha, int beta, bool maximizingPlayer, bool aiTurn) {
    // Base condition: reached leaf node.
    if (depth == 0) {
        MinimaxResult result;
        result.score = evaluateBoard(board);
        result.moveFound = false;
        return result;
    }

    M moves = filterLegalMoves(board, aiTurn);
    if (moves.num == 0) {
        // No legal moves (stalemate or checkmate)
        MinimaxResult result;
        result.score = evaluateBoard(board);
        result.moveFound = false;
        return result;
    }

    MinimaxResult bestResult;
    bestResult.moveFound = false;

    if (maximizingPlayer) {
        bestResult.score = -1000000;

        for (int i = 0; i < moves.num; i++) {
            char newBoard[8][8];
            applyMove(board, newBoard, moves.fromR[i], moves.fromC[i], moves.toR[i], moves.toC[i], moves.promotion[i]);

            MinimaxResult childResult = minimax(newBoard, depth - 1, alpha, beta, false, !aiTurn);

            if (!bestResult.moveFound || childResult.score > bestResult.score) {
                bestResult.score = childResult.score;
                bestResult.bestMoveFromR = moves.fromR[i];
                bestResult.bestMoveFromC = moves.fromC[i];
                bestResult.bestMoveToR = moves.toR[i];
                bestResult.bestMoveToC = moves.toC[i];
                bestResult.moveFound = true;
            }
            if (bestResult.score > alpha)
                alpha = bestResult.score;
            if (beta <= alpha) break; // beta cutoff
        }
    }
    else {
        bestResult.score = 1000000;
        for (int i = 0; i < moves.num; i++) {
            char newBoard[8][8];
            applyMove(board, newBoard, moves.fromR[i], moves.fromC[i], moves.toR[i], moves.toC[i], moves.promotion[i]);

            MinimaxResult childResult = minimax(newBoard, depth - 1, alpha, beta, true, !aiTurn);

            if (!bestResult.moveFound || childResult.score < bestResult.score) {
                bestResult.score = childResult.score;
                bestResult.bestMoveFromR = moves.fromR[i];
                bestResult.bestMoveFromC = moves.fromC[i];
                bestResult.bestMoveToR = moves.toR[i];
                bestResult.bestMoveToC = moves.toC[i];
                bestResult.moveFound = true;
            }
            if (bestResult.score < beta)
                beta = bestResult.score;
            if (beta <= alpha) break; // alpha cutoff
        }
    }

    return bestResult;
}
