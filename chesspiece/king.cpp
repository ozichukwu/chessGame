#include <iostream>
#include "chesspiece.h"
#include "../chessboard.h"


King::King() : Piece() { setType(0); }
King::King(int pos, bool isWhite, bool isAlive, bool player, int type) : Piece(pos, isWhite, isAlive, player) {
	name = "K";
	if (player) name = "k";
	setType(type);
}
King::~King() {}

void King::setType(int pieceType) { loadType(pieceType, "king.png"); }

Moves King::calcPossibleMoves(Board& board) {

	Moves moves;
	int possibleMoves[8] = {
		calcPosChange(-1, -1), calcPosChange(-1, 0), calcPosChange(-1, 1), calcPosChange(0, 1),
		calcPosChange(1, 1), calcPosChange(1, 0), calcPosChange(1, -1), calcPosChange(0, -1)
	};

	for (int i = 0; i < 8; i++) {
		if (possibleMoves[i] < 0) continue;
		Tile tile = board.getTile(possibleMoves[i]);
		//if (tile is empty or contains an opponent) and move won't result in check
		if ((tile.isEmpty() || (tile.getPiece()->isPlayer() != player)) && moveIsLegal(possibleMoves[i], board)) {
			moves.moves[moves.num] = possibleMoves[i];
			moves.num++;
		}
	}

	return moves;
}
