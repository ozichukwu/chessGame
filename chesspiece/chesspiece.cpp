#include <iostream>
#include "chesspiece.h"
#include "../chessboard.h"


Piece::Piece() : name(), pos(), white(0), alive(0), player(0),
	hasMoved(0), type(0), whiteTexturePaths(), blackTexturePaths() {

	initTexturePaths();
}
Piece::Piece(int pos, bool isWhite, bool isAlive, bool player)
	: name(), pos(pos), white(isWhite), alive(isAlive), player(player), hasMoved(0), type() {

	initTexturePaths();
}
Piece::~Piece() {}

void Piece::initTexturePaths() {
	whiteTexturePaths[0] = "./assets/pieces/white/marble/";
	whiteTexturePaths[1] = "./assets/pieces/white/plastic/";
	whiteTexturePaths[2] = "./assets/pieces/white/rust/";
	whiteTexturePaths[3] = "./assets/pieces/white/wood/";

	blackTexturePaths[0] = "./assets/pieces/black/marble/";
	blackTexturePaths[1] = "./assets/pieces/black/plastic/";
	blackTexturePaths[2] = "./assets/pieces/black/rust/";
	blackTexturePaths[3] = "./assets/pieces/black/wood/";
}

//Calculates the new position, where possible, given changes in row, column and position (optional)
int Piece::calcPosChange(int deltaRow, int deltaCol, int position) {
	if (position == -1) position = pos;
	int row = (position / 8) + deltaRow;
	int col = (position % 8) + deltaCol;
	int newPos = row * 8 + col;
	if (row >= 0 && row < 8 && col >= 0 && col < 8) return newPos;
	return -1;
}

//Checks if a seemingly possible move is legal (will defend/not result in check)
bool Piece::moveIsLegal(int move, Board& board) {

	//check if the king will be in check after this move
	string board2dArr[8][8];
	board.getBoardIn2d(board2dArr); //get board state
	int row = pos / 8;
	int col = pos % 8;
	int row1 = move / 8;
	int col1 = move % 8;
	board2dArr[row][col] = "."; //make move
	board2dArr[row1][col1] = name;

	//perform check
	return !board.isKingInCheck(&board2dArr, player);
}

int Piece::getPos() const { return pos; }
bool Piece::isWhite() const { return white; }
bool Piece::isAlive() const { return alive; }
int Piece::getType() const { return type; }
string Piece::getName() const { return name; }
bool Piece::isPlayer() const { return player; }

void Piece::setType(int pieceType) {}

void Piece::loadType(int pieceType, string file) {
	type = pieceType;

	string path;
	if (white) path = whiteTexturePaths[type] + file;
	else path = blackTexturePaths[type] + file;

	//create the texture objects
	texture = sf::Texture(path);
	texture.setSmooth(true);
}

void Piece::move(int newPos) {
	if (!hasMoved) hasMoved = true;
	pos = newPos;
}

void Piece::die() { alive = false; }

void Piece::draw(sf::RenderWindow& window, int x, int y, int tileSize) {
	sf::Sprite sprite(texture);

	//get the size of the texture and compute a suitable scale factor
	sf::Vector2u textureSize = texture.getSize();
	unsigned int maxLen = textureSize.x;
	if (textureSize.y > maxLen) maxLen = textureSize.y; //get longest dimension
	float scale = (float)tileSize * 0.9 / (float)maxLen; //using 90% of the tileSize (safe value)
	sprite.setScale({ scale, scale });

	//center the sprite on the tile
	sf::FloatRect spriteRect = sprite.getGlobalBounds();
	float xF = (float)x + ((float)tileSize - spriteRect.size.x) / 2;
	float yF = (float)y + ((float)tileSize - spriteRect.size.y) / 2;
	sprite.setPosition({ xF, yF });

	//draw sprite
	window.draw(sprite);
}

Moves Piece::calcPossibleMoves(Board& board) { Moves m; return m; }
