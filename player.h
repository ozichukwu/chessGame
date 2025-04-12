#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <cctype>
#include "chessAI.h"
#include "network.h"
#include "gameSettings.h"


class Board; // forward declaration

using namespace std;

enum class PlayerType {AI, HOST, CLIENT};

class Player {
private:
	PlayerType type;
	int aiDepth = 1;

	void getAIMove(Board& board, const char chessBoard2d[8][8], int depth);

	NetworkMgr* networkPtr;

public:
	Player();
	Player(Settings::Difficulty difficulty, NetworkMgr* networkPtr, PlayerType type = PlayerType::AI);
	~Player();

	bool isAI() const;
	bool isHost() const;

	void sendMove(int from, int to); //is called when user makes a move
	void sendMove(string boardArr[8][8], Board& board);
};

#endif