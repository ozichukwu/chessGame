#include <iostream>
#include "network.h"

NetworkMgr::NetworkMgr() {
    serverStarted = false;
    clientStarted = false;
    connToClientCreated = false;
    connToServerCreated = false;
}

NetworkMgr::~NetworkMgr() {}

bool NetworkMgr::isDataReceived() { return dataReceived; }

Data NetworkMgr::getData(bool reset) {
    if (!reset) return receivedData;
    dataReceived = false;
    Data data = receivedData;
    receivedData = Data();
    return data;
}

void NetworkMgr::startServer() {
    serverStarted = true;

    // bind the listener to a port
    listener = sf::TcpListener();
    listener.setBlocking(false);
    if (listener.listen(PORT) != sf::Socket::Status::Done) {
        cout << "Error in binding TCP listener\n";
    }
    else { cout << "TCP listener binding successful\n"; }

    socket = sf::TcpSocket();
    socket.setBlocking(false);
    isClientFound();
}

bool NetworkMgr::isClientFound() {
    if (connToClientCreated) return true;

    // accept a new connection
    if (listener.accept(socket) != sf::Socket::Status::Done) {}
    else {
        connToClientCreated = true;
        sendGreetings();
    }
    return connToClientCreated;
}

void NetworkMgr::startClient(sf::IpAddress serverIp) {
    clientStarted = true;
    this->serverIp = serverIp;
    // Start socket in non-blocking mode. In blocking mode, the thread halts until
    // a connection or timeout. In non-blocking mode, no response is given. In this case,
    // for a successful connection, the program depends on a "greetings" message from the server.
    // The client send an "acknowledgement" in return
    socket = sf::TcpSocket();
    socket.setBlocking(false);
    sf::Socket::Status status = socket.connect(serverIp, PORT, sf::seconds(30.f));
    isServerFound();
}

bool NetworkMgr::isServerFound() {
    if (connToServerCreated) return true;

    receive();
    if (isDataReceived()) { //must be connected
        Data data = getData(false); //should be greetings from server

        if (data.data[0] == static_cast<int>(NetworkCommands::GREETING) && data.data[1] == 0) {
            //greetings received

            connToServerCreated = true;
            sendGreetings(true); //send acknowledgement            
        }
    }
    return connToServerCreated;
}

void NetworkMgr::reset() {
    //reset parameters
    serverStarted = false;
    clientStarted = false;
    connToClientCreated = false;
    connToServerCreated = false;
    listener = sf::TcpListener();
    socket = sf::TcpSocket();
    sf::IpAddress serverIp = sf::IpAddress::Any;
    dataReceived = false;
    receivedData = Data();
}

bool NetworkMgr::send(char* buffer, size_t size) {
    if (socket.send(buffer, size) != sf::Socket::Status::Done) {
        return false;
    }
    return true;
}

bool NetworkMgr::terminate() {
    //terminate connection
    if (connToClientCreated || connToServerCreated) {
        char buffer[100] = { 0 };
        buffer[0] = static_cast<char>(NetworkCommands::TERMINATE);
        send(buffer, 100);
    }

    //reset parameters
    reset();
}

bool NetworkMgr::sendMove(int from, int to) {
    char buffer[100] = { 0 };
    buffer[0] = static_cast<int>(NetworkCommands::OPPONENT_MOVE);
    buffer[1] = from;
    buffer[2] = to;
    return send(buffer, 100);
}

bool NetworkMgr::sendGreetings(bool acknowledge) {
    char buffer[100] = { 0 };
    buffer[0] = static_cast<char>(NetworkCommands::GREETING);
    buffer[1] = acknowledge;
    return send(buffer, 100);
}

size_t NetworkMgr::receive() {
    char buffer[100];
    size_t sizeReceived = 0;
    memset(buffer, 0, 100);

    sf::Socket::Status status = socket.receive(buffer, 100, sizeReceived);
    if (status != sf::Socket::Status::Done) {}
    else {
        size_t i = 0;
        for (; (receivedData.size < 100 && i < sizeReceived); i++, receivedData.size++) {
            receivedData.data[receivedData.size] = buffer[i];
        }
        if (receivedData.size == 100) {
            //handle data received here
            dataReceived = true;

            if (receivedData.data[0] == static_cast<int>(NetworkCommands::TERMINATE)) {
                Data data = receivedData;
                reset();
                dataReceived = true;
                receivedData = data;
            }
        }
    }
    return sizeReceived;
}

void NetworkMgr::poll() {
    // if server/client is activated and connected, check for incoming data
    if ((serverStarted && isClientFound()) || (clientStarted && isServerFound()))
        receive();

    // do ping/pong every x seconds
}

sf::IpAddress NetworkMgr::getLocalIpAddress() {
    optional<sf::IpAddress> ip = sf::IpAddress::getLocalAddress();
    return *ip;
}
