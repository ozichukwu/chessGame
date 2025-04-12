#ifndef MENUWIDGETS_H
#define MENUWIDGETS_H
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <string>
#include <ctime>
#include "object.h"
#include "eventManager.h"
#include "gameSettings.h"
#include "network.h"
#include "player.h"
#include "chessboard.h"

using namespace std;

class Menu : public Object {
private:
	enum class Page { MAINMENU, GAME, MULTIPLAYER, HOST_GAME, JOIN_GAME, JOIN_GAME_CONNECT, SETTINGS };

	int screenWidth;
	int screenHeight;

	Page page = Page::MAINMENU;

	sf::Font font;
	sf::RenderWindow* window;
	sf::Texture bgTexture;

	string serverIP = ""; //store the input from the user
	time_t connTimeout = 0; //deadline (UNIX timestamp), in seconds, for the connections to be established (server/client)

	typedef struct textElement {
		int x;
		int y;
		string textString;
		int width;
		int height;
		int size = 40;
		bool center = false;
		bool vertCenter = false;
		int verticalMargin = 3;
		sf::Color color = sf::Color::White;
		sf::Text::Style style = sf::Text::Regular;

		void render(Menu& obj, string textString, int verticalMargin = 3 /* In % */);
	}textElement;

	Settings settings;
	Board* chessBoardPtr;
	Player* opponentPtr;
	NetworkMgr* networkPtr;
	EventMgr* eventMgrPtr;

	void startGame(PlayerType playAs = PlayerType::AI);
	void endGame();

public:
	Menu();
	Menu(int screenW, int screenH, sf::RenderWindow* window, EventMgr* eventMgrPtr, NetworkMgr* networkPtr);
	~Menu();

	//abstract class overriders
	void onClick(int x, int y);
	void onTextEntered(string s);
	void onMouseHover(int x, int y);
	void onKeyPress(sf::Keyboard::Scancode keyCode);
	void onReceivedData(Data data);

	//Rule of thumb in rendering menu;
	//menu is 60% of window dimension and centered
	//title text bottom margin is 10%, other components: 3%
	//back/exit button/component is at top left of the screen

	void renderBackButton(bool isExit = 0); //isExit specifies if the button is for "exit" or "back"
	void renderMenuBackground(bool isGameScreen = false);
	void renderMainMenu();
	void renderMultiplayerMenu();
	void renderSettingsMenu();
	void renderHostGameScreen();
	void renderJoinGameScreen();
	void renderJoiningGameScreen();
	void renderGameUI();
	void renderGameStatusBar();
	void render();
};

#endif
