#include <iostream>
#include "chesspiece.h"
#include "../chessboard.h"


Pawn::Pawn() : Piece() { setType(0); }
Pawn::Pawn(int pos, bool isWhite, bool isAlive, bool player, int type) : Piece(pos, isWhite, isAlive, player) {
	name = "P";
	if (player) name = "p";
	setType(type);
}
Pawn::~Pawn() {}

void Pawn::setType(int pieceType) { loadType(pieceType, "pawn.png"); }

Moves Pawn::calcPossibleMoves(Board& board) {
	Moves moves;
	int initialJump = player ? calcPosChange(-2, 0) : calcPosChange(2, 0);
	if (!hasMoved && board.getTile(initialJump).isEmpty() && moveIsLegal(initialJump, board)) {
		moves.moves[moves.num] = initialJump;
		moves.num++;
	}

	int move1 = player ? calcPosChange(-1, 0) : calcPosChange(1, 0);
	if (move1 >= 0 && board.getTile(move1).isEmpty() && moveIsLegal(move1, board)) {
		moves.moves[moves.num] = move1;
		moves.num++;
	}

	//possible kill moves
	int killMoves[2] = {
		player ? calcPosChange(-1, -1) : calcPosChange(1, -1),
		player ? calcPosChange(-1, 1) : calcPosChange(1, 1)
	};
	for (int i = 0; i < 2; i++) {
		if (killMoves[i] >= 0) {
			Tile tile = board.getTile(killMoves[i]);
			//if tile is not empty and contains an opponent and move is legal
			if (!tile.isEmpty() && (tile.getPiece()->isPlayer() != player) && moveIsLegal(killMoves[i], board)) {
				moves.moves[moves.num] = killMoves[i];
				moves.num++;
			}
		}
	}

	return moves;
}
