#include "gameSettings.h"

Settings::Settings() {}
Settings::~Settings() {}

int Settings::getBoardType() const { return boardType; }
int Settings::getPieceType() const { return pieceType; }
bool Settings::getPlayerColor() const { return playerColor; }
Settings::Difficulty Settings::getDifficulty() const { return difficulty; }

string Settings::readBoardType() const {
	if (boardType == 0) return "Default";
	if (boardType == 1) return "Brown Marble";
	if (boardType == 2) return "Green Marble";
	if (boardType == 3) return "Wood";
}
string Settings::readPieceType() const {
	if (pieceType == 0) return "Marble";
	if (pieceType == 1) return "Plastic";
	if (pieceType == 2) return "Rust";
	if (pieceType == 3) return "Wood";
}
string Settings::readPlayerColor() const {
	if (playerColor) return "White";
	return "Black";
}
string Settings::readDifficulty() const {
	if (difficulty == Difficulty::EASY) return "Easy";
	if (difficulty == Difficulty::CHALLENGING) return "Challenging";
	if (difficulty == Difficulty::EXPERT) return "Expert";
}

void Settings::switchBoardType() { boardType = (boardType + 1) % 4; }
void Settings::switchPieceType() { pieceType = (pieceType + 1) % 4; }
void Settings::switchPlayerColor() { playerColor = !playerColor; }
void Settings::setPlayerColor(bool white) { playerColor = white; }
void Settings::switchDifficulty() {
	if (difficulty == Difficulty::EASY)
		difficulty = Difficulty::CHALLENGING;
	else if (difficulty == Difficulty::CHALLENGING)
		difficulty = Difficulty::EXPERT;
	else if (difficulty == Difficulty::EXPERT)
		difficulty = Difficulty::EASY;
}
