#include <iostream>
#include "boardtile.h"


Tile::Tile() : position(0), size(0), highlighted(0),
	moveMarkerActive(0), moveMarkerOwner(-1), empty(0), boardTexturePtr(), tileRect() {}

Tile::Tile(int pos, int tileSize, bool emptyTile, Piece* tilePiecePtr, sf::Texture& texture) {
	position = pos;
	size = tileSize;
	highlighted = false;
	moveMarkerActive = false;
	moveMarkerOwner = -1;
	empty = emptyTile;
	piecePtr = tilePiecePtr;

	boardTexturePtr = &texture;
	float tileSizeF = (float)size; //just because implicit type conversion was required
	tileRect = sf::RectangleShape({tileSizeF, tileSizeF});
	tileRect.setTexture(boardTexturePtr);

	if ( ((pos / 8 & 1) == 0) ^ (pos & 1) )
		tileRect.setTextureRect(sf::IntRect({ 64, 0 }, { 64, 64 })); //the lighter tile
	else
		tileRect.setTextureRect(sf::IntRect({ 0, 0 }, { 64, 64 })); //the darker tile
}

Tile::~Tile() {}

int Tile::getPosition() const { return position; }
int Tile::getSize() const { return size; }
bool Tile::isHighlighted() const { return highlighted; }
bool Tile::hasActiveMoveMarker() const { return moveMarkerActive; }
int Tile::getMoveMarkerOwner() const { return moveMarkerOwner; }
bool Tile::isEmpty() const { return empty; }
Piece* Tile::getPiece() { return piecePtr; }

void Tile::setHighlighting(bool value) { highlighted = value; }

void Tile::setMoveMarker(bool value, int owner) {
	moveMarkerActive = value;
	moveMarkerOwner = owner;
}
void Tile::setPiece(Piece* tilePiecePtr) {
	empty = false;
	piecePtr = tilePiecePtr;
}

void Tile::movePiece(Tile& moveTo) {
	moveTo.setPiece(piecePtr);
	piecePtr->move(moveTo.position);
	moveTo.highlighted = true;
	empty = true;
	piecePtr = nullptr;
	highlighted = true;
}

void Tile::draw(sf::RenderWindow& window, int x, int y) {
	//draw tile
	float xF = (float)x; //just because implicit type conversion was required
	float yF = (float)y;

	//draw tile background
	tileRect.setPosition({ xF, yF });
	window.draw(tileRect);

	if (highlighted) { //draw highlighted background
		float tileSizeF = (float)size; //just because implicit type conversion was required
		sf::RectangleShape highlightRect({ tileSizeF, tileSizeF });
		highlightRect.setFillColor(sf::Color(247, 215, 5, 100)); //translucent color
		highlightRect.setPosition({ xF, yF });
		window.draw(highlightRect);
	}

	//draw piece if any
	if (!empty) piecePtr->draw(window, x, y, size);

	//draw move markers if any
	if (moveMarkerActive) {
		float radius = (float)size * 0.125; //using 25% of the tile size for the diameter
		float xF = (float)(x + size / 2) - radius; //just because implicit type conversion was required
		float yF = (float)(y + size / 2) - radius;
		sf::CircleShape marker(radius);
		marker.setFillColor(sf::Color(100, 250, 50));
		marker.setPosition({xF, yF});
		window.draw(marker);
	}
}