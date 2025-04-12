#include "player.h"
#include "chessboard.h"

Player::Player() : aiDepth(0), type(), networkPtr(nullptr) {}

Player::Player(Settings::Difficulty difficulty, NetworkMgr* netPtr, PlayerType type) : type(type) {
	//set AI difficulty
	if (difficulty == Settings::Difficulty::EASY) aiDepth = 2;
	else if (difficulty == Settings::Difficulty::CHALLENGING) aiDepth = 3;
	else if (difficulty == Settings::Difficulty::EXPERT) aiDepth = 4;

	networkPtr = netPtr;
}

Player::~Player() {}

bool Player::isAI() const { return type == PlayerType::AI; }
bool Player::isHost() const { return type == PlayerType::HOST; }

void Player::getAIMove(Board& board, const char chessBoard2d[8][8], int depth) {
	
	//get AI's response and update board.. will run in a separate thread
	MinimaxResult best = AI().compute(chessBoard2d, depth);

	//get movements
	int from = best.bestMoveFromR * 8 + best.bestMoveFromC;
	int to = best.bestMoveToR * 8 + best.bestMoveToC;

	board.effectOpponentMove(from, to);
	delete[] chessBoard2d;
}

void Player::sendMove(int from, int to) {
	//transform board to opponent's pov
	from = 63 - from;
	to = 63 - to;
	
	//send to opponent
	networkPtr->sendMove(from, to);
}

void Player::sendMove(string boardArr[8][8], Board& board) {
	// prepare the arguments
	char (*newBoard)[8] = new char[8][8];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			char c = boardArr[i][j][0];
			newBoard[i][j] = c;
		}
	}

	//get AI's move
	if (isAI()) {
		// Start a new thread and pass arguments
		thread workerThread(&Player::getAIMove, this, ref(board), newBoard, aiDepth);
		//allow thread run independently
		workerThread.detach();		
	}
}
