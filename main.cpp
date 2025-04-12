#include <iostream>
#include <SFML/Graphics.hpp>
#include "chessboard.h"
#include "eventManager.h"
#include "network.h"
#include "menuWidgets.h"

using namespace std;

int main() {
	sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    int screenWidth = mode.size.x;
    int screenHeight = mode.size.y;
	sf::RenderWindow window(mode, "Play CHESS");
	
    NetworkMgr network;
    EventMgr eventMgr;

    Menu game(screenWidth, screenHeight, &window, &eventMgr, &network);
    eventMgr.addEventListener(&game, "textEntered");
    eventMgr.addEventListener(&game, "keyPress");
    eventMgr.addEventListener(&game, "network");

	while (window.isOpen()) {
        while (const optional event = window.pollEvent())
        {
            bool closed = event->is<sf::Event::Closed>();
            const sf::Event::TextEntered* textEntered = event->getIf<sf::Event::TextEntered>();
            const sf::Event::KeyPressed* keyPressed = event->getIf<sf::Event::KeyPressed>();
            const sf::Event::MouseButtonPressed* mousePressed = event->getIf<sf::Event::MouseButtonPressed>();
            const sf::Event::MouseMoved* mouseMoved = event->getIf<sf::Event::MouseMoved>();

            if (closed)
                window.close();

            if (keyPressed) {
                sf::Keyboard::Scancode keyCode = keyPressed->scancode;
                eventMgr.handleKeyPress(keyCode);
            }
            else if (textEntered) {
                string text = sf::String(textEntered->unicode);
                eventMgr.handleTextEntered(text);
            }
            else if (mousePressed && mousePressed->button == sf::Mouse::Button::Left) {
                int x = mousePressed->position.x;
                int y = mousePressed->position.y;
                eventMgr.handleClick(x, y);
            }
            else if (mouseMoved) {
                int x = mouseMoved->position.x;
                int y = mouseMoved->position.y;
                //eventMgr.handleMoveHover(x, y);
            }
        }

        network.poll();
        if (network.isDataReceived()) {
            eventMgr.handleReceivedData(network.getData());
        }

		window.clear();
        game.render();
		window.display();
	}

	return 0;
}
