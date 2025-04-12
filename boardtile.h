#ifndef TILE_H
#define TILE_H
#include <SFML/Graphics.hpp>
#include "./chesspiece/chesspiece.h"

using namespace std;

class Tile {
private:
	int position;
	int size;
	bool highlighted;
	bool moveMarkerActive;
	int moveMarkerOwner;
	bool empty;
	Piece* piecePtr;

	//graphics-related properties
	sf::Texture* boardTexturePtr;
	sf::RectangleShape tileRect;

public:
	Tile();
	Tile(int pos, int size, bool empty, Piece* tilePiecePtr, sf::Texture& texture);
	~Tile();

	int getPosition() const;
	int getSize() const;
	bool isHighlighted() const; //feature to be implemented later
	bool hasActiveMoveMarker() const;
	int getMoveMarkerOwner() const;
	bool isEmpty() const;
	Piece* getPiece();

	void setHighlighting(bool value = true);
	void setMoveMarker(bool value, int owner = -1);
	void setPiece(Piece* piecePtr);
	void movePiece(Tile& moveTo);

	void draw(sf::RenderWindow& window, int x, int y);
};

#endif
