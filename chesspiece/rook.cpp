#include <iostream>
#include "chesspiece.h"
#include "../chessboard.h"


Rook::Rook() : Piece() { setType(0); }
Rook::Rook(int pos, bool isWhite, bool isAlive, bool player, int type) : Piece(pos, isWhite, isAlive, player) {
	name = "R";
	if (player) name = "r";
	setType(type);
}
Rook::~Rook() {}

void Rook::setType(int pieceType) { loadType(pieceType, "rook.png"); }

Moves Rook::calcPossibleMoves(Board& board) {

	Moves moves;
	int coords[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

	for (int i = 0; i < 4; i++) {
		int possibleMove = pos;

		while (possibleMove >= 0) {
			possibleMove = calcPosChange(coords[i][0], coords[i][1], possibleMove);
			if (possibleMove < 0) continue;

			Tile tile = board.getTile(possibleMove);

			//if (tile is empty or contains an opponent) and move won't result in check
			if ( (tile.isEmpty() || (tile.getPiece()->isPlayer() != player)) && moveIsLegal(possibleMove, board) ) {
				moves.moves[moves.num] = possibleMove;
				moves.num++;
			}
			if (!tile.isEmpty()) break;
		}
	}

	return moves;
}
