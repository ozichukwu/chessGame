#include "eventManager.h"

EventMgr::EventMgr() : numHandlers(0) {}
EventMgr::~EventMgr() {}

int EventMgr::addEventListener(Object* objPtr, string type) {
	eventHandler evHandler; //create struct
	evHandler.objPtr = objPtr;
	evHandler.type = type;
	evHandler.active = 1;

	int id = numHandlers;
	handlers[id] = evHandler;
	if (numHandlers < 100) return numHandlers++;
	return id;
}

void EventMgr::removeEventListener(int id) {
	const int TOTAL = 100;
	handlers[id] = handlers[TOTAL - 1];
	handlers[TOTAL - 1] = eventHandler();
}

void EventMgr::removeEventListeners(Object* objPtr) {
	for (int i = 0; i < 100; i++) {
		if (handlers[i].objPtr == objPtr) removeEventListener(i);
	}
}

void EventMgr::handleClick(int x, int y) {
	for (int i = 0; i < 100; i++)
		if (handlers[i].type == "click")
			handlers[i].objPtr->onClick(x, y);
}

void EventMgr::handleTextEntered(string s) {
	for (int i = 0; i < 100; i++)
		if (handlers[i].type == "textEntered")
			handlers[i].objPtr->onTextEntered(s);
}

void EventMgr::handleMoveHover(int x, int y) {
	for (int i = 0; i < 100; i++)
		if (handlers[i].type == "mouseHover")
			handlers[i].objPtr->onMouseHover(x, y);
}

void EventMgr::handleKeyPress(sf::Keyboard::Scancode keyCode) {
	for (int i = 0; i < 100; i++)
		if (handlers[i].type == "keyPress")
			handlers[i].objPtr->onKeyPress(keyCode);
}

void EventMgr::handleReceivedData(Data data) {
	for (int i = 0; i < 100; i++)
		if (handlers[i].type == "network")
			handlers[i].objPtr->onReceivedData(data);
}
