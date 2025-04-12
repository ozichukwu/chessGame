#include <iostream>
#include <string>
#include "chessboard.h"


Board::Board() : type(0), size(0), tileSize(0), positionOnScreen() {}

Board::Board(int screenW, int screenH, Player opp, Settings settings) {
	//To determine board size;
	//1. Take 75% minimum value btw screen width and height
	size = screenW;
	if (screenH < size) size = screenH;
	size = 75 * size / 100;

	//2. Take the nearest least multiple of 8
	size -= size % 8;

	tileSize = size / 8;

	//The board is centered on screen
	positionOnScreen[0] = (screenW - size) / 2;
	positionOnScreen[1] = (screenH - size) / 2;

	opponent = opp;

	//Specify the board material type (graphics)
	//0 for default, 1 for brown marble, 2 for green marble, 3 for wood
	setType(settings.getBoardType());

	//loading initial game state
	playerIsWhite = settings.getPlayerColor();
	playerTurn = opponent.isAI() || !opponent.isHost(); //you play first if opponent is AI or Client
	int pieceType = settings.getPieceType();

	//create board pieces
	boardPieces[0] = new Rook(0, !playerIsWhite, 1, 0, pieceType);
	boardPieces[1] = new Knight(1, !playerIsWhite, 1, 0, pieceType);
	boardPieces[2] = new Bishop(2, !playerIsWhite, 1, 0, pieceType);
	//enforcing the "Queen on her own color" rule
	if (playerIsWhite) {
		boardPieces[3] = new Queen(3, !playerIsWhite, 1, 0, pieceType);
		boardPieces[4] = new King(4, !playerIsWhite, 1, 0, pieceType);
	}
	else {
		boardPieces[3] = new King(3, !playerIsWhite, 1, 0, pieceType);
		boardPieces[4] = new Queen(4, !playerIsWhite, 1, 0, pieceType);
	}
	boardPieces[5] = new Bishop(5, !playerIsWhite, 1, 0, pieceType);
	boardPieces[6] = new Knight(6, !playerIsWhite, 1, 0, pieceType);
	boardPieces[7] = new Rook(7, !playerIsWhite, 1, 0, pieceType);
	for (int i = 8; i < 16; i++) boardPieces[i] = new Pawn(i, !playerIsWhite, 1, 0, pieceType);

	for (int i = 16; i < 24; i++) boardPieces[i] = new Pawn(i + 32, playerIsWhite, 1, 1, pieceType);
	boardPieces[24] = new Rook(56, playerIsWhite, 1, 1, pieceType);
	boardPieces[25] = new Knight(57, playerIsWhite, 1, 1, pieceType);
	boardPieces[26] = new Bishop(58, playerIsWhite, 1, 1, pieceType);
	//enforcing the "Queen on her own color" rule
	if (playerIsWhite) {
		boardPieces[27] = new Queen(59, playerIsWhite, 1, 1, pieceType);
		boardPieces[28] = new King(60, playerIsWhite, 1, 1, pieceType);
	}
	else {
		boardPieces[27] = new King(59, playerIsWhite, 1, 1, pieceType);
		boardPieces[28] = new Queen(60, playerIsWhite, 1, 1, pieceType);
	}
	boardPieces[29] = new Bishop(61, playerIsWhite, 1, 1, pieceType);
	boardPieces[30] = new Knight(62, playerIsWhite, 1, 1, pieceType);
	boardPieces[31] = new Rook(63, playerIsWhite, 1, 1, pieceType);
	boardPieces[32] = new Piece(); //dummy piece


	//create board tiles
	for (int i = 0; i < 64; i++) {
		if (i < 16)
			boardTiles[i] = Tile(i, tileSize, false, boardPieces[i], boardTexture);
		else if (i < 48)
			boardTiles[i] = Tile(i, tileSize, true, boardPieces[32], boardTexture);
		else
			boardTiles[i] = Tile(i, tileSize, false, boardPieces[i - 32], boardTexture);
	}
}

Board::~Board() {
	for (int i = 0; i < 33; i++) {
		delete boardPieces[i];
		boardPieces[i] = nullptr;
	}
}

int Board::getSize() const { return size; }
int Board::getTileSize() const { return tileSize; }
Tile Board::getTile(int index) { return boardTiles[index]; }
bool Board::isPlayerWhite() const { return playerIsWhite; }
bool Board::getPlayerTurn() const { return playerTurn; }
GameStatus Board::getGameStatus() const { return gameStatus; }
const int* Board::getPositionOnScreen() const { return positionOnScreen; }
const sf::Texture& Board::getBoardTexture() { return boardTexture; }

void Board::getBoardIn2d(string board2dArr[8][8]) {
	for (int i = 0; i < 64; i++) {
		int r = i / 8;
		int c = i % 8;
		if (boardTiles[i].isEmpty()) board2dArr[r][c] = ".";
		else board2dArr[r][c] = boardTiles[i].getPiece()->getName();
	}
}

int Board::getPlayerCaptures(bool player) {
	int total = 16;
	for (int i = 0; i < 64; i++) {
		if (!boardTiles[i].isEmpty()) {
			Piece* piece = boardTiles[i].getPiece();
			if (piece->isAlive() && piece->isPlayer() != player) // if piece is alive and belongs to opponent
				total--;
		}
	}
	return total;
}

bool Board::isKingInCheck(string (*boardPtr)[8][8], bool myKing) {
	string board[8][8];
	if (boardPtr == nullptr) //use this board
		getBoardIn2d(board);
	else { // Copy the argument
		for (int i = 0; i < 8; ++i)
			for (int j = 0; j < 8; ++j)
				board[i][j] = (*boardPtr)[i][j];
	};

	string king = "k"; //symbol of player's (or opponent's, if myKing == false) king
	string oppKing = "K"; //symbol of player's opponent piece (depends on myKing)
	string queen = "Q"; //symbol of player's opponent piece (depends on myKing)
	string knight = "N";
	string bishop = "B";
	string rook = "R";
	string pawn = "P";
	if (!myKing) { king = "K"; oppKing = "k", queen = "q"; knight = "n"; bishop = "b"; rook = "r"; pawn = "p"; }

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
	//check for king threats
	int kingDir[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1} };
	for (int i = 0; i < 8; i++) {
		int dr = r + kingDir[i][0];
		int dc = c + kingDir[i][1];
		if (dr < 0 || dr >= 8 || dc < 0 || dc >= 8) continue;
		if (board[dr][dc] == oppKing) return true;
	}
	//check for knight threats
	int knightDir[8][2] = { {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {2, -1}, {2, 1}, {1, -2}, {1, 2} };
	for (int i = 0; i < 8; i++) {
		int dr = r + knightDir[i][0];
		int dc = c + knightDir[i][1];
		if (dr < 0 || dr >= 8 || dc < 0 || dc >= 8) continue;
		if (board[dr][dc] == knight) return true;
	}
	//check for pawn threats
	int pawnDir[2][2] = { {-1, -1}, {-1, 1} };
	if (!myKing) pawnDir[0][0] = pawnDir[1][0] = 1;
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
			string piece = board[dr][dc];
			if (piece != ".") {
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
			string piece = board[dr][dc];
			if (piece != ".") {
				if (board[dr][dc] == rook || board[dr][dc] == queen) return true;
				break;
			}
		}
	}
	return false;
}


GameStatus Board::isGameOver(bool player, bool isPlayerTurn) {
	// Checks for checkmate, stalemate or neither, from the player's POV. If player = true, it
	// checks from the player's POV. If player = false, it checks from the opponent's POV.
	// If isPlayerTurn = true, it assumes it's the player's (or opponent's: player = false) turn

	bool playerKingInCheck = isKingInCheck(nullptr, player);
	bool opponentKingInCheck = isKingInCheck(nullptr, !player);
	bool playerHasValidMoves = false;
	bool opponentHasValidMoves = false;

	//check if player opponent has valid moves
	for (int i = 0; i < 64; i++) {
		if (boardTiles[i].isEmpty()) continue;

		Piece* piecePtr = boardTiles[i].getPiece(); //tile has a piece
		bool pieceHasValidMoves = piecePtr->calcPossibleMoves(*this).num > 0;
		if (!playerHasValidMoves && piecePtr->isPlayer() == player && pieceHasValidMoves)
			playerHasValidMoves = true;

		else if (!opponentHasValidMoves && piecePtr->isPlayer() != player && pieceHasValidMoves)
			opponentHasValidMoves = true;

		if (playerHasValidMoves && opponentHasValidMoves) break;
	}

	if (isPlayerTurn) {
		if (playerHasValidMoves) return GameStatus::INPLAY;
		if (playerKingInCheck) return GameStatus::DEFEAT;
	}
	else {
		if (opponentHasValidMoves) return GameStatus::INPLAY;
		if (opponentKingInCheck) return GameStatus::VICTORY;
	}
	return GameStatus::STALEMATE;
}

void Board::setGameStatus(GameStatus status) { gameStatus = status; }

void Board::setType(int boardType) {
	type = boardType;

	const string texturePaths[4] = {
		"./assets/boards/default128x64.png",
		"./assets/boards/brownMarble128x64.png",
		"./assets/boards/greenMarble128x64.png",
		"./assets/boards/wood128x64.png",
	};

	//create the texture objects
	boardTexture = sf::Texture(texturePaths[type]);
	boardTexture.setSmooth(true);
}

void Board::render(sf::RenderWindow& window) {

	for (int pos = 0; pos < 64; pos++) {
		//compute the location of the tile on-screen
		int x = positionOnScreen[0] + (pos % 8) * tileSize;
		int y = positionOnScreen[1] + (pos / 8) * tileSize;
		//draw tile
		boardTiles[pos].draw(window, x, y);
	}
}

void Board::effectOpponentMove(int tileFrom, int tileTo) {

	//disable all tile highlights
	for (int i = 0; i < 64; i++) {
		boardTiles[i].setHighlighting(false);
	}

	if (!boardTiles[tileTo].isEmpty()) { // it has to be player's piece
		//set the piece as killed
		boardTiles[tileTo].getPiece()->die();
	}
	boardTiles[tileFrom].movePiece(boardTiles[tileTo]);
	//piece has now been moved successfully

	//handle pawn promotion, where neccessary
	Piece* piecePtr = boardTiles[tileTo].getPiece();
	if (piecePtr->getName() == "P" && tileTo / 8 == 7) {

		for (int i = 0; i < 33; i++) {
			if (piecePtr == boardPieces[i]) {
				Piece* promotedPiece = new Queen(*piecePtr); //promoted to queen
				boardPieces[i] = promotedPiece;
				boardTiles[tileTo].setPiece(promotedPiece);
				delete piecePtr;
				piecePtr = nullptr;
				break;
			}
		}
	}

	if (gameStatus == GameStatus::INPLAY) { //check if you lost
		GameStatus gameStatus = isGameOver(true, true);
		if (gameStatus == GameStatus::DEFEAT) setGameStatus(GameStatus::DEFEAT); // you lost
		else if (gameStatus == GameStatus::STALEMATE) setGameStatus(GameStatus::STALEMATE); // stalemate
	}

	playerTurn ^= 1; //change turn to yours
}


//event handler
void Board::onTextEntered(string s) {}
void Board::onKeyPress(sf::Keyboard::Scancode keyCode) {}
void Board::onClick(int x, int y) {
	
	//we only care if it's the player's turn and game's not over
	if (!playerTurn && gameStatus != GameStatus::INPLAY) return;

	//we only care about clicks inside the board
	if (x < positionOnScreen[0] || x > positionOnScreen[0] + size
		|| y < positionOnScreen[1] && y > positionOnScreen[1] + size)
		return;

	int columnPosition = (x - positionOnScreen[0]) / tileSize;
	int rowPosition = (y - positionOnScreen[1]) / tileSize;
	int tile = columnPosition + rowPosition * 8;

	//if move marker is active
	//move piece (move marker owner) to this tile..
	//sort out any possible kill operation
	//highlights after moving will be handled here also
	bool pieceMoved = false; //no piece has moved in this click on the board

	if (boardTiles[tile].hasActiveMoveMarker()) { //player wants to move piece
		//disable all tile highlights
		for (int i = 0; i < 64; i++) {
			boardTiles[i].setHighlighting(false);
		}

		pieceMoved = true;
		if (!boardTiles[tile].isEmpty()) { // it has to be an opponent
			//set the piece as killed
			boardTiles[tile].getPiece()->die();
		}
		int moveMarkerOwner = boardTiles[tile].getMoveMarkerOwner();
		boardTiles[moveMarkerOwner].movePiece(boardTiles[tile]);
		//piece has now been moved successfully

		//handle pawn promotion, where neccessary
		Piece* piecePtr = boardTiles[tile].getPiece();
		if ((piecePtr->getName() == "p" && tile / 8 == 0)
			|| (piecePtr->getName() == "P" && tile / 8 == 7)) {

			for (int i = 0; i < 33; i++) {
				if (piecePtr == boardPieces[i]) {
					Piece* promotedPiece = new Queen(*piecePtr); //promoted to queen
					boardPieces[i] = promotedPiece;
					boardTiles[tile].setPiece(promotedPiece);
					delete piecePtr;
					piecePtr = nullptr;
					break;
				}
			}
		}
		//check if you won
		GameStatus gameStatus = isGameOver(true, false);
		if (gameStatus == GameStatus::VICTORY) setGameStatus(GameStatus::VICTORY); // you won
		else if (gameStatus == GameStatus::STALEMATE) setGameStatus(GameStatus::STALEMATE); // stalemate

		playerTurn ^= 1; //change turn to opponent's
		 
		//send move to opponent
		if (opponent.isAI() && gameStatus == GameStatus::INPLAY) {
			string board2d[8][8];
			getBoardIn2d(board2d);
			opponent.sendMove(board2d, *this);
		}
		else {
			opponent.sendMove(moveMarkerOwner, tile);
		}
	}

	//put off all move markers
	for (int i = 0; i < 64; i++) {
		boardTiles[i].setMoveMarker(false);
	}

	//if no piece moved, tile has a piece and it's the piece's turn
	if (!pieceMoved
		&& !boardTiles[tile].isEmpty()
		&& boardTiles[tile].getPiece()->isPlayer() == playerTurn
		) {

		//highlight corresponding move markers
		Moves possibleMoves = boardTiles[tile].getPiece()->calcPossibleMoves(*this);
		
		//put on the move markers
		for (int i = 0; i < possibleMoves.num; i++) {
			boardTiles[possibleMoves.moves[i]].setMoveMarker(true, tile);
		}
	}
}

void Board::onMouseHover(int x, int y) {}

void Board::onReceivedData(Data data) {
	if (data.data[0] == static_cast<int>(NetworkCommands::OPPONENT_MOVE)) { //opponent's move
		effectOpponentMove(data.data[1], data.data[2]);
	}
}
