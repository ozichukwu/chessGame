#ifndef EVENTMGR_H
#define EVENTMGR_H
#include <string>
#include <SFML/Graphics.hpp>
#include "object.h"

using namespace std;

//structure of each handler
typedef struct eventHandler {
	Object* objPtr = nullptr;
	string type = "";
	bool active = false;
	eventHandler() {};
	~eventHandler() {};

} eventHandler;

class EventMgr {
private:
	eventHandler handlers[100]; //maximum of 100 handlers
	int numHandlers;
public:
	EventMgr();
	~EventMgr();
	int addEventListener(Object* objPtr, string type);
	void removeEventListener(int id);
	void removeEventListeners(Object* objPtr);
	void handleClick(int x, int y);
	void handleTextEntered(string s);
	void handleKeyPress(sf::Keyboard::Scancode keyCode);
	void handleMoveHover(int x, int y);
	void handleReceivedData(Data data);
};

#endif
