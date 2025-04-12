#ifndef SETTINGS_H
#define SETTINGS_H
#include <string>

using namespace std;

class Settings {
public:
	Settings();
	~Settings();

	enum class Difficulty { EASY, CHALLENGING, EXPERT };

	int getBoardType() const;
	int getPieceType() const;
	bool getPlayerColor() const;
	Difficulty getDifficulty() const;

	string readBoardType() const;
	string readPieceType() const;
	string readPlayerColor() const;
	string readDifficulty() const;

	void switchBoardType();
	void switchPieceType();
	void switchPlayerColor();
	void setPlayerColor(bool white = true);
	void switchDifficulty();

private:
	int boardType = 0;
	int pieceType = 0;
	bool playerColor = 0;
	Difficulty difficulty = Difficulty::EASY;

};

#endif
