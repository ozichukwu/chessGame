#include <cctype>
#include "menuWidgets.h"

Menu::Menu() : screenWidth(0), screenHeight(0), window(nullptr),
	settings(), chessBoardPtr(nullptr), opponentPtr(nullptr), networkPtr(nullptr),
	eventMgrPtr(nullptr) {

	font = sf::Font("./assets/fonts/font.ttf");
};

Menu::Menu(int screenW, int screenH, sf::RenderWindow* windowPtr, EventMgr* eventMgrPtr, NetworkMgr* networkPtr)
	: screenWidth(screenW), screenHeight(screenH), window(windowPtr), settings(),
	chessBoardPtr(nullptr), opponentPtr(nullptr), networkPtr(networkPtr),
	eventMgrPtr(eventMgrPtr)
	{

	// Load UI assets
	font = sf::Font("./assets/fonts/font.ttf");
	bgTexture = sf::Texture("./assets/images/background.png");
	bgTexture.setSmooth(true);

}

Menu::~Menu() {
	delete chessBoardPtr;
	delete opponentPtr;
};

void Menu::textElement::render(Menu& obj, string textStr, int verticalMargin) {
	textString = textStr;

	sf::Text text(obj.font);
	text.setString(textString);
	text.setCharacterSize(size);
	text.setFillColor(color);
	text.setStyle(style);
	sf::FloatRect textRect = text.getGlobalBounds();

	float xF = (float)x;
	float yF = (float)y;
	if (center)
		xF += (width - textRect.size.x) / 2;
	if (vertCenter)
		yF += (height - textRect.size.y) / 2;
	text.setPosition({ xF, yF });
	obj.window->draw(text);

	y += (int)textRect.size.y;
	//add vertical margin
	y += height * verticalMargin / 100;
}

void Menu::renderBackButton(bool isExit) {
	float xF = (float)(screenWidth * 10 / 100);
	float yF = (float)(screenHeight * 10 / 100);
	sf::RectangleShape bgRect({ xF, yF });
	bgRect.setFillColor(sf::Color(105, 105, 105));
	bgRect.setPosition({ 0.f, 0.f });
	window->draw(bgRect);

	//prep textElement
	textElement textE;
	textE.width = (int)xF;
	textE.height = (int)yF;
	textE.x = 0;
	textE.y = 0;
	textE.center = true;
	textE.vertCenter = true;
	textE.size = textE.width * 15 / 100; //5% of screen width for title
	if (isExit)
		textE.render(*this, "[ESC] Exit");
	else textE.render(*this, "[ESC] Back");
}

void Menu::renderMenuBackground(bool isGameScreen) {
	float xF = (float)screenWidth;
	float yF = (float)screenHeight;
	sf::RectangleShape bgRect({ xF, yF });
	bgRect.setTexture(&bgTexture);
	window->draw(bgRect);

	if (!isGameScreen) { // draw menu area
		float xF1 = xF * 0.6;
		float yF1 = yF * 0.6;
		sf::RectangleShape menuArea({ xF1, yF1 });
		menuArea.setFillColor(sf::Color(0, 0, 0, 150));
		menuArea.setPosition({ (xF - xF1) / 2, (yF - yF1) / 2 });
		window->draw(menuArea);
	}
	else {//draw a dark translucent cover
		sf::RectangleShape rect({ xF, yF });
		rect.setFillColor(sf::Color(0, 0, 0, 220));
		window->draw(rect);
	}
}

void Menu::renderMainMenu() {
	//draw background
	renderMenuBackground();

	renderBackButton(true);

	//prep textElement
	int mainAreaWidth = (int)(0.6 * screenWidth);
	int mainAreaHeight = (int)(0.6 * screenHeight);
	textElement textE;
	textE.width = mainAreaWidth;
	textE.height = mainAreaHeight;
	textE.x = (screenWidth - mainAreaWidth) / 2;
	textE.y = (screenHeight - mainAreaHeight) / 2;
	textE.y += textE.height * 3 / 100; //add initial margin
	textE.center = true;
	textE.size = screenWidth * 5 / 100; //5% of screen width for title

	//render title
	textE.render(*this, "Play CHESS", 10); //add 10% title margin

	textE.size = screenWidth * 2 / 100; //2% of screen width for other components
	//Left-justify the texts
	textE.x += 0.2 * textE.width;
	textE.center = false;
	//render cmd1
	textE.render(*this, "[Press 1]\tPlayer vs AI");
	//render cmd2
	textE.render(*this, "[Press 2]\tPlayer vs Player (LAN)");
	//render cmd3
	textE.render(*this, "[Press 3]\tSettings");
}

void Menu::renderMultiplayerMenu() {
	//draw background
	renderMenuBackground();

	renderBackButton();

	//prep textElement
	int mainAreaWidth = (int)(0.6 * screenWidth);
	int mainAreaHeight = (int)(0.6 * screenHeight);
	textElement textE;
	textE.width = mainAreaWidth;
	textE.height = mainAreaHeight;
	textE.x = (screenWidth - mainAreaWidth) / 2;
	textE.y = (screenHeight - mainAreaHeight) / 2;
	textE.y += textE.height * 3 / 100; //add initial margin
	textE.center = true;
	textE.size = screenWidth * 5 / 100; //5% of screen width for title

	//render title
	textE.render(*this, "Player vs Player (LAN)", 10); //add 10% title margin

	textE.size = screenWidth * 2 / 100; //2% of screen width for other components
	//Left-justify the texts
	textE.x += 0.2 * textE.width;
	textE.center = false;
	//render cmd1
	textE.render(*this, "[Press 1]\tHost game");
	//render cmd2
	textE.render(*this, "[Press 2]\tJoin game");
}

void Menu::renderSettingsMenu() {
	//draw background
	renderMenuBackground();

	renderBackButton();

	//prep textElement
	int mainAreaWidth = (int)(0.6 * screenWidth);
	int mainAreaHeight = (int)(0.6 * screenHeight);
	textElement textE;
	textE.width = mainAreaWidth;
	textE.height = mainAreaHeight;
	textE.x = (screenWidth - mainAreaWidth) / 2;
	textE.y = (screenHeight - mainAreaHeight) / 2;
	textE.y += textE.height * 3 / 100; //add initial margin
	textE.center = true;
	textE.size = screenWidth * 5 / 100; //5% of screen width for title

	//render title
	textE.render(*this, "Settings", 10); //add 10% title margin

	textE.size = screenWidth * 2 / 100; //2% of screen width for other components
	//Left-justify the texts
	textE.x += 0.2 * textE.width;
	textE.center = false;
	//render cmd1
	string boardType = settings.readBoardType();
	//textE.render(*this, "[Press 1]\tChange board type [Default]");
	textE.render(*this, "[Press 1]\tChange board type [" + boardType + "]");
	//render cmd2
	string pieceType = settings.readPieceType();
	//textE.render(*this, "[Press 2]\tChange piece type [Marble]");
	textE.render(*this, "[Press 2]\tChange piece type [" + pieceType + "]");
	//render cmd3
	string difficulty = settings.readDifficulty();
	string playerColor = settings.readPlayerColor();
	textE.render(*this, "**Single player mode only**", 1);
	//textE.render(*this, "[Press 3]\tChange difficulty [Easy]");
	textE.render(*this, "[Press 3]\tChange difficulty [" + difficulty + "]");
	//textE.render(*this, "[Press 4]\tChange piece color [White]");
	textE.render(*this, "[Press 4]\tChange piece color [" + playerColor + "]");
}

void Menu::renderHostGameScreen() {
	//draw background
	renderMenuBackground();

	time_t timeLeft = time(nullptr) > connTimeout ? 0 : connTimeout - time(nullptr);
	//only show "Back" button after timeout
	if (timeLeft == 0) renderBackButton();

	string localIp = NetworkMgr::getLocalIpAddress().toString();

	//prep textElement
	int mainAreaWidth = (int)(0.6 * screenWidth);
	int mainAreaHeight = (int)(0.6 * screenHeight);
	textElement textE;
	textE.width = mainAreaWidth;
	textE.height = mainAreaHeight;
	textE.x = (screenWidth - mainAreaWidth) / 2;
	textE.y = (screenHeight - mainAreaHeight) / 2;
	textE.y += textE.height * 3 / 100; //add initial margin
	textE.center = true;
	textE.size = screenWidth * 5 / 100; //5% of screen width for title

	//render title
	textE.render(*this, "Host game (" + to_string(timeLeft) + " s)", 10); //add 10% title margin
	textE.size = screenWidth * 2 / 100; //2% of screen width for other components
	//Left-justify the texts
	textE.x += 0.2 * textE.width;
	textE.center = false;
	//render text
	textE.render(*this, "Share your local IP with your opponent");

	//render text field
	float textFieldWidth = (float)(mainAreaWidth * 0.6);
	float textFieldHeight = (float)(mainAreaHeight * 0.2);
	sf::RectangleShape textField({ textFieldWidth, textFieldHeight });
	textField.setFillColor(sf::Color::Black);
	textField.setPosition({ (float)textE.x, (float)textE.y });
	window->draw(textField);

	//render text in text field
	sf::FloatRect textFieldRect = textField.getGlobalBounds();
	sf::Text ipText(font);
	ipText.setString(localIp);
	ipText.setCharacterSize(textE.size);
	ipText.setFillColor(sf::Color::White);
	ipText.setStyle(sf::Text::Regular);
	sf::FloatRect ipTextRect = ipText.getLocalBounds();
	ipText.setPosition({
		(float)(textFieldRect.position.x + (textFieldWidth - ipTextRect.size.x) / 2),
		(float)(textFieldRect.position.y - (ipTextRect.size.y / 2) + (textFieldHeight - ipTextRect.size.y) / 2),
		});
	window->draw(ipText);
}

void Menu::renderJoinGameScreen() {
	//draw background
	renderMenuBackground();

	renderBackButton();

	//prep textElement
	int mainAreaWidth = (int)(0.6 * screenWidth);
	int mainAreaHeight = (int)(0.6 * screenHeight);
	textElement textE;
	textE.width = mainAreaWidth;
	textE.height = mainAreaHeight;
	textE.x = (screenWidth - mainAreaWidth) / 2;
	textE.y = (screenHeight - mainAreaHeight) / 2;
	textE.y += textE.height * 3 / 100; //add initial margin
	textE.center = true;
	textE.size = screenWidth * 5 / 100; //5% of screen width for title

	//render title
	textE.render(*this, "Join game", 10); //add 10% title margin

	textE.size = screenWidth * 2 / 100; //2% of screen width for other components
	//Left-justify the texts
	textE.x += 0.2 * textE.width;
	textE.center = false;
	//render text
	textE.render(*this, "Enter server's public IPv4");

	//render text field
	float textFieldWidth = (float)(mainAreaWidth * 0.6);
	float textFieldHeight = (float)(mainAreaHeight * 0.2);
	sf::RectangleShape textField({ textFieldWidth, textFieldHeight });
	textField.setFillColor(sf::Color::Black);
	textField.setPosition({ (float)textE.x, (float)textE.y });
	window->draw(textField);

	//render text in text field
	sf::FloatRect textFieldRect = textField.getGlobalBounds();
	sf::Text ipText(font);
	ipText.setString(serverIP == "" ? "000.000.000.000" : serverIP);
	ipText.setCharacterSize(textE.size);
	ipText.setFillColor(serverIP == "" ? sf::Color(169, 169, 169) : sf::Color::White);
	ipText.setStyle(sf::Text::Regular);
	sf::FloatRect ipTextRect = ipText.getLocalBounds();
	ipText.setPosition({
		(float)(textFieldRect.position.x + (textFieldWidth - ipTextRect.size.x) / 2),
		(float)(textFieldRect.position.y - (ipTextRect.size.y / 2) + (textFieldHeight - ipTextRect.size.y) / 2),
		});
	window->draw(ipText);

	//update the text element
	textE.y += textFieldRect.size.y + (textE.height * textE.verticalMargin / 100);
	//render action button
	textE.render(*this, "[ENTER] Connect");
}

void Menu::renderJoiningGameScreen() {
	//draw background
	renderMenuBackground();

	time_t timeLeft = time(nullptr) > connTimeout ? 0 : connTimeout - time(nullptr);
	//only show "Back" button after timeout
	if (timeLeft == 0) renderBackButton();

	//prep textElement
	int mainAreaWidth = (int)(0.6 * screenWidth);
	int mainAreaHeight = (int)(0.6 * screenHeight);
	textElement textE;
	textE.width = mainAreaWidth;
	textE.height = mainAreaHeight;
	textE.x = (screenWidth - mainAreaWidth) / 2;
	textE.y = (screenHeight - mainAreaHeight) / 2;
	textE.y += textE.height * 3 / 100; //add initial margin
	textE.center = true;
	textE.size = screenWidth * 5 / 100; //5% of screen width for title

	//render title
	textE.render(*this, "Connecting... (" + to_string(timeLeft) + " s)", 10); //add 10% title margin

	textE.size = screenWidth * 2 / 100; //2% of screen width for other components
	//Left-justify the texts
	textE.x += 0.2 * textE.width;
	textE.center = false;
	//render text
	textE.render(*this, "Please wait while we connect your opponent...");
}

void Menu::renderGameUI() {
	//draw background
	renderMenuBackground(true);
	renderBackButton(true);
	renderGameStatusBar();
	chessBoardPtr->render(*window);
}

void Menu::renderGameStatusBar() {
	float barWidth = (float)(screenWidth * 0.6);
	float barHeight = (float)(screenHeight * 0.1);
	float barX = ((float)screenWidth - barWidth) / 2;
	float barY = 0.f;
	sf::RectangleShape bgRect({ barWidth, barHeight });
	bgRect.setFillColor(sf::Color(128, 128, 128));
	bgRect.setPosition({ barX, barY });
	window->draw(bgRect);
	sf::FloatRect barRect = bgRect.getGlobalBounds();

	//render "piece captures" in status bar
	sf::Text capturesText(font);
	capturesText.setString("Captures\nYou: " + to_string(chessBoardPtr->getPlayerCaptures())
		+ "\nOpponent: " + to_string(chessBoardPtr->getPlayerCaptures(false))
	);
	capturesText.setCharacterSize((barWidth < barHeight ? barWidth : barHeight) * 20 / 100);
	capturesText.setFillColor(sf::Color::White);
	capturesText.setStyle(sf::Text::Regular);
	sf::FloatRect capturesTextRect = capturesText.getLocalBounds();
	capturesText.setPosition({
		(float)(barRect.position.x + barWidth * 2 / 100), // 2% padding
		(float)(barRect.position.y + (barHeight - capturesTextRect.size.y) / 2),
		});
	window->draw(capturesText);

	//render text in status bar
	sf::Text text(font);

	//craft text string
	string textStr = "";
	if (chessBoardPtr->getGameStatus() != GameStatus::INPLAY) {
		if (chessBoardPtr->getGameStatus() == GameStatus::VICTORY) textStr = "Victory!";
		else if (chessBoardPtr->getGameStatus() == GameStatus::DEFEAT) textStr = "Defeat!";
		else if (chessBoardPtr->getGameStatus() == GameStatus::STALEMATE) textStr = "Draw!";
	}
	else {
		textStr += chessBoardPtr->getPlayerTurn() ? "Your turn" : "Opponent's turn";
		if (chessBoardPtr->isKingInCheck()) textStr += " (CHECK)";
	}
	text.setString(textStr);
	text.setCharacterSize((barWidth < barHeight ? barWidth : barHeight) * 30 / 100);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Regular);
	sf::FloatRect textRect = text.getLocalBounds();
	text.setPosition({
		(float)(barRect.position.x + (barWidth - textRect.size.x) / 2),
		(float)(barRect.position.y - (textRect.size.y / 2) + (barHeight - textRect.size.y) / 2),
		});
	window->draw(text);
}


//event handlers
void Menu::onClick(int x, int y) {};
void Menu::onTextEntered(string s) {
	if (page == Page::MAINMENU) {
		if (s == "1") {  // Start the game
			page = Page::GAME;
			startGame();
		}
		else if (s == "2") page = Page::MULTIPLAYER;
		else if (s == "3") page = Page::SETTINGS;
	}
	else if (page == Page::MULTIPLAYER) {
		if (s == "1") { //Host game
			page = Page::HOST_GAME;
			connTimeout = time(nullptr) + 60; //60s timeout
			startGame(PlayerType::HOST);
		}
		else if (s == "2") page = Page::JOIN_GAME;
	}
	else if (page == Page::SETTINGS) {
		if (s == "1") settings.switchBoardType();
		else if (s == "2") settings.switchPieceType();
		else if (s == "3") settings.switchDifficulty();
		else if (s == "4") settings.switchPlayerColor();
	}
	else if (page == Page::HOST_GAME) { //Host game
	}
	else if (page == Page::JOIN_GAME) { //Join game
		if (s == "." || isdigit(s[0])) serverIP += s;
	}
};

void Menu::onKeyPress(sf::Keyboard::Scancode keyCode) {
	bool isEscape = keyCode == sf::Keyboard::Scancode::Escape;
	bool isEnter = keyCode == sf::Keyboard::Scancode::Enter;
	bool isBackspace = keyCode == sf::Keyboard::Scancode::Backspace;

	if (page == Page::MAINMENU && isEscape) window->close();

	else if (page == Page::GAME && isEscape) {
		if (!opponentPtr->isAI()) networkPtr->terminate();
		endGame();
		page = Page::MAINMENU;
	}

	else if ((page == Page::GAME || page == Page::MULTIPLAYER || page == Page::SETTINGS) && isEscape)
		page = Page::MAINMENU;

	else if (page == Page::HOST_GAME && isEscape && time(nullptr) > connTimeout)
		page = Page::MULTIPLAYER;

	else if (page == Page::JOIN_GAME) {
		if (isEscape) page = Page::MULTIPLAYER;

		if (isBackspace)
			if (!serverIP.empty()) serverIP.pop_back();

		if (isEnter) { //resolve ip and start network game
			optional<sf::IpAddress> ip = sf::IpAddress::resolve(serverIP);
			if (!ip.has_value()) return; //ip is malformed

			page = Page::JOIN_GAME_CONNECT;
			connTimeout = time(nullptr) + 30; //30s timeout

			startGame(PlayerType::CLIENT);
		}
	}

	else if (page == Page::JOIN_GAME_CONNECT && isEscape && time(nullptr) > connTimeout)
		page = Page::JOIN_GAME;
}

void Menu::onMouseHover(int x, int y) {};
void Menu::onReceivedData(Data data) {
	if (data.data[0] == static_cast<int>(NetworkCommands::TERMINATE)) {
		page = Page::MAINMENU;
	}
	if (data.data[0] == static_cast<int>(NetworkCommands::GREETING)) {
		//New connection
		connTimeout = 0;
		page = Page::GAME;
	}
};

void Menu::render() {
	if (page == Page::MAINMENU) renderMainMenu();
	else if (page == Page::GAME) renderGameUI();

	else if (page == Page::MULTIPLAYER) renderMultiplayerMenu();
	else if (page == Page::HOST_GAME) renderHostGameScreen();
	else if (page == Page::JOIN_GAME) renderJoinGameScreen();
	else if (page == Page::JOIN_GAME_CONNECT) renderJoiningGameScreen();

	else if (page == Page::SETTINGS) renderSettingsMenu();
}

void Menu::startGame(PlayerType playAs) {
	endGame();

	//start game
	if (playAs != PlayerType::AI) {//host is black, client is white in multiplayer games
		settings.setPlayerColor(playAs == PlayerType::CLIENT);
	}
	opponentPtr = new Player(settings.getDifficulty(), networkPtr, playAs);
	chessBoardPtr = new Board(screenWidth, screenHeight, *opponentPtr, settings);
	//add event listeners
	eventMgrPtr->addEventListener(chessBoardPtr, "click");
	eventMgrPtr->addEventListener(chessBoardPtr, "network");

	if (playAs == PlayerType::CLIENT) {
		optional<sf::IpAddress> ip = sf::IpAddress::resolve(serverIP);
		networkPtr->startClient(ip.value());
	}
	else if (playAs == PlayerType::HOST) {
		networkPtr->startServer();
	}
}

void Menu::endGame() {
	//delete relevant pointers
	eventMgrPtr->removeEventListeners(chessBoardPtr); //remove events listeners from chessboard
	delete chessBoardPtr;
	delete opponentPtr;

	chessBoardPtr = nullptr;
	opponentPtr = nullptr;
}