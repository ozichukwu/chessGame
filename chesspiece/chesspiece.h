#ifndef CHESSPIECE_H
#define CHESSPIECE_H
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class Board; //forward declaration of the board class

typedef struct _ {
	int num = 0; //number of moves
	int moves[64]; //moves as in the tile positions
} Moves;

class Piece {
private:
	void initTexturePaths();
protected:
	string name;
	int pos;
	bool white;
	bool alive;
	bool hasMoved;
	int type;
	bool player;

	void loadType(int type, string file);
	int calcPosChange(int deltaRow, int deltaCol, int position = -1);
	bool moveIsLegal(int move, Board& board);

	//graphics-related
	string whiteTexturePaths[4];
	string blackTexturePaths[4];
	sf::Texture texture;
	
public:
	Piece();
	Piece(int pos, bool isWhite, bool alive, bool player);
	~Piece();

	int getPos() const;
	bool isWhite() const;
	bool isAlive() const;
	bool isPlayer() const;
	int getType() const;
	string getName() const;

	virtual void setType(int type);
	virtual Moves calcPossibleMoves(Board& board);
	void move(int newPos);
	void die();
	void draw(sf::RenderWindow& window, int x, int y, int tileSize);
};


class King : public Piece {
private:

public:
	King();
	King(int pos, bool isWhite, bool alive, bool player, int type = 0);
	~King();

	void setType(int type);
	virtual Moves calcPossibleMoves(Board& board) override;
};


class Queen : public Piece {
private:

public:
	Queen();
	Queen(int pos, bool isWhite, bool alive, bool player, int type = 0);
	Queen(Piece piece); //mainly for pawn promotion
	~Queen();

	void setType(int type);
	virtual Moves calcPossibleMoves(Board& board) override;
};


class Rook : public Piece {
private:

public:
	Rook();
	Rook(int pos, bool isWhite, bool alive, bool player, int type = 0);
	~Rook();

	void setType(int type);
	virtual Moves calcPossibleMoves(Board& board) override;
};


class Bishop : public Piece {
private:

public:
	Bishop();
	Bishop(int pos, bool isWhite, bool alive, bool player, int type = 0);
	~Bishop();

	void setType(int type);
	virtual Moves calcPossibleMoves(Board& board) override;
};


class Knight : public Piece {
private:

public:
	Knight();
	Knight(int pos, bool isWhite, bool alive, bool player, int type = 0);
	~Knight();

	void setType(int type);
	virtual Moves calcPossibleMoves(Board& board) override;
};


class Pawn : public Piece {
private:

public:
	Pawn();
	Pawn(int pos, bool isWhite, bool alive, bool player, int type = 0);
	~Pawn();

	void setType(int type);
	virtual Moves calcPossibleMoves(Board& board) override;
};


#endif
