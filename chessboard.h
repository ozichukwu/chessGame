#ifndef BOARD_H
#define BOARD_H
#include <SFML/Graphics.hpp>
#include "object.h"
#include "gameSettings.h"
#include "boardtile.h"
#include "./chesspiece/chesspiece.h"
#include "player.h"

using namespace std;

enum class GameStatus {INPLAY, VICTORY, DEFEAT, STALEMATE };

class Board : public Object {
private:
	int type;
	int size;
	int tileSize;
	int positionOnScreen[2];
	bool playerIsWhite;
	bool playerTurn; //true when it's player's turn, false when it's opponent's
	GameStatus gameStatus; //in-play, victory or stalemate
	Tile boardTiles[64];
	Piece* boardPieces[33];
	Player opponent;

	//graphics-related properties
	sf::Texture boardTexture;

public:
	Board();
	Board(int screenW, int screenH, Player opponent, Settings settings);
	~Board();

	//getters
	int getSize() const;
	int getTileSize() const;
	Tile getTile(int index);
	bool isPlayerWhite() const;
	bool getPlayerTurn() const;
	GameStatus getGameStatus() const;
	const int* getPositionOnScreen() const;
	const sf::Texture& getBoardTexture();
	void getBoardIn2d(string board2dArr[8][8]);
	int getPlayerCaptures(bool player = true); // Get number of pieces captures by player (or opponent: player=false)

	bool isKingInCheck(string (*board)[8][8] = nullptr, bool myKing = true);
	GameStatus isGameOver(bool player, bool isPlayerTurn);

	//setters
	void setType(int type = 0);
	void setGameStatus(GameStatus status);

	void effectOpponentMove(int from, int to);
	
	void render(sf::RenderWindow& window);

	//event handlers
	void onClick(int x, int y);
	void onTextEntered(string s);
	void onKeyPress(sf::Keyboard::Scancode keyCode);
	void onMouseHover(int x, int y);
	void onReceivedData(Data data);
};

#endif
