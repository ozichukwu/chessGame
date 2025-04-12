#include <iostream>
#include "chesspiece.h"
#include "../chessboard.h"


Queen::Queen() : Piece() { setType(0); }
Queen::Queen(int pos, bool isWhite, bool isAlive, bool player, int type) : Piece(pos, isWhite, isAlive, player) {
	name = "Q";
	if (player) name = "q";
	setType(type);
}
Queen::Queen(Piece piece) : Piece(piece.getPos(), piece.isWhite(), piece.isAlive(), piece.isPlayer()) {
	name = "Q";
	if (player) name = "q";
	setType(piece.getType());
}
Queen::~Queen() {}

void Queen::setType(int pieceType) { loadType(pieceType, "queen.png"); }

Moves Queen::calcPossibleMoves(Board& board) {

	Moves moves;
	int coords[8][2] = { {-1, -1},{-1, 0},{-1, 1},{0, 1},{1, 1},{1, 0},{1, -1},{0, -1} };

	for (int i = 0; i < 8; i++) {
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
