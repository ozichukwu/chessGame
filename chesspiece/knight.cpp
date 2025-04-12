#include <iostream>
#include "chesspiece.h"
#include "../chessboard.h"


Knight::Knight() : Piece() { setType(0); }
Knight::Knight(int pos, bool isWhite, bool isAlive, bool player, int type) : Piece(pos, isWhite, isAlive, player) {
	name = "N";
	if (player) name = "n";
	setType(type);
}
Knight::~Knight() {}

void Knight::setType(int pieceType) { loadType(pieceType, "knight.png"); }

Moves Knight::calcPossibleMoves(Board& board) {
	Moves moves;
	int possibleMoves[8] = {
		calcPosChange(-2, -1),
		calcPosChange(-2, 1),
		calcPosChange(-1, -2),
		calcPosChange(-1, 2),
		calcPosChange(1, -2),
		calcPosChange(1, 2),
		calcPosChange(2, -1),
		calcPosChange(2, 1),
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
