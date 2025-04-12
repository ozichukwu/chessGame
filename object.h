#ifndef OBJECT_H
#define OBJECT_H
#include <string>
#include <SFML/Graphics.hpp>
#include "network.h"

using namespace std;

class Object {
public:
	virtual void onClick(int x, int y) = 0;
	virtual void onTextEntered(string s) = 0;
	virtual void onKeyPress(sf::Keyboard::Scancode keyCode) = 0;
	virtual void onMouseHover(int x, int y) = 0;
	virtual void onReceivedData(Data data) = 0;
};

#endif
