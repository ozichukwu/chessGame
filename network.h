#ifndef NETWORK_H
#define NETWORK_H
#include <SFML/Network.hpp>

using namespace std;

const int PORT = 30000;

enum class NetworkCommands { TERMINATE, GREETING, OPPONENT_MOVE };

struct Data {
	char data[100] = { 0 };
	size_t size = 0;
};

/*
* ********* Network protocol specification **************
* A complete message sent/received is 100 bytes. If this message is stored in an array "data" then,
* Command = data[0], Message content = data[1..99]
* 
* *** Terminate ***
* data[0] = 0
* 
* *** New connection ***
* data[0] = 1
* 
* *** Opponent's move ***
* data[0] = 2, data[1] = move from(int 0 - 63), data[2] = move to(int 0 - 63)
* 
*/


class NetworkMgr {
private:
	bool serverStarted;
	bool clientStarted;
	bool connToClientCreated;
	bool connToServerCreated;

	sf::TcpListener listener;
	sf::TcpSocket socket; //to communicate with server/client
	sf::IpAddress serverIp = sf::IpAddress::Any; //ip of the server that user is connected to

	bool dataReceived = false;
	Data receivedData;

	void reset(); //reset parameters

public:
	NetworkMgr();
	~NetworkMgr();

	bool isDataReceived();
	Data getData(bool reset = true);

	void startServer();
	bool isClientFound();

	void startClient(sf::IpAddress serverIp);
	bool isServerFound();

	void poll();

	//end existing connections
	bool terminate();

	//buffer is assumed to be 100 bytes max
	bool send(char* buffer, size_t size);

	//send "greetings" message
	bool sendGreetings(bool acknowledge = false);

	//send player's move
	bool sendMove(int from, int to);

	size_t receive();

	static sf::IpAddress getLocalIpAddress();

};

#endif
