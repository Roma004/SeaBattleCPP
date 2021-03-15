#include "config.cpp"
#include "utils/sendData.cpp"
#include "utils/sendSocketData.cpp"
#include "coreEngine/scene.hpp"
#include "gameObjects/gameMap.hpp"
#include "scenes/playerMap.hpp"
#include "scenes/playerFight.hpp"
#include "scenes/remotePlayerFight.hpp"
#include "scenes/mainMenue.hpp"
#include "scenes/waitUntilReady.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <chrono>
#include <iostream>
#include <thread>


sf::TcpListener server;
sf::TcpSocket socket;

extern std::queue<json> gRecvDataQueue;
extern std::queue<json> gSentDataQueue;
extern std::mutex gReceivedDataMutex;
extern std::mutex gSentDataMutex;

bool initTextures() { // init textures, size is not a matter
    // for (auto && [category, fileNames] : textureFiles) { 
    for (auto && textureFile : textureFiles) { 
        auto category = textureFile.first;
        auto fileNames = textureFile.second;
        textures[category] = std::vector<sf::Texture>(fileNames.size());
        for (size_t i = 0; i < fileNames.size(); ++i) {
            if (!textures[category][i].loadFromFile(fileNames[i])) {
                std::cerr << "Unable to load texture from file `" << fileNames[i] << "`\n";
                return false;
            }

            textures[category][i].setSmooth(true);
        }
    }

    return true;
}

bool initFonts() {

    for (auto && fontFile : fontFiles) {
        fonts[fontFile.first] = sf::Font();
        if (!fonts[fontFile.first].loadFromFile(fontFile.second)) {
            std::cerr << "Unable to load texture from file `" << fontFile.second << "`\n";
            return false;
        }
    }

    return true;
}

void dataReceiverFunc() {
    json recvData;
    std::string data;

    while (true) {
        data = receiveData(socket);
        // std::cout << "received: " << data << "\n";

        if (data == "") {
            // std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        try {
            recvData = json::parse(data);
        } catch (json::parse_error &e) {
            std::cout << e.what() << "\n";
            // std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }

        // if (data.length() < 500) std::cout << "new data received\n" << data << "\n";

        gReceivedDataMutex.lock();
        gRecvDataQueue.push(recvData);
        // std::cout << "in receive size: " << gRecvDataQueue.size() << "\n";
        gReceivedDataMutex.unlock();
    }
}

void dataSenderFunc() {
    json data;

    while (true) {
        if (gSentDataQueue.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }

        gSentDataMutex.lock();
        data = gSentDataQueue.front();
        gSentDataQueue.pop();
        gSentDataMutex.unlock();

        sendData(socket, data.dump());

        // if (data.dump().length() < 500)
        //     std::cout << "New Data sent\n" << data.dump() << "\n";
    }
}

void localGame(sf::RenderWindow &window) {
    PlayerMap player1("Player 1");
    PlayerMap player2("Player 2");

    player1.start(window);
    player2.start(window);

    PlayerFight playerFight(player1.player, player2.player);
    playerFight.start(window);
}

void serverGame(sf::RenderWindow &window) {

    if (server.listen(23456) != sf::Socket::Done) {
        std::cerr << "Can not listen on port 23457\n";
    }

    if (server.accept(socket) != sf::Socket::Done) {
        std::cerr << "Can not accept the connection\n";
    }

    PlayerMap player1("Player 1");
    PlayerMap player2("Player 2", PlayerMap::remote);

    std::thread dataReceiver(dataReceiverFunc);
    dataReceiver.detach();
    std::thread dataSender(dataSenderFunc);
    dataSender.detach();

    player1.start(window);
    std::cout << "asd\n";

    WaitUntilReady wait(player2.player, player1.getJSONData());
    wait.start(window);

    remotePlayerFight playerFight(player1.player, player2.player, true);
    playerFight.start(window);
}


void clientGame(sf::RenderWindow &window) {
    // sf::TcpSocket socket;

    if (socket.connect("127.0.0.1", 23456) != sf::Socket::Done) {
        std::cerr << "Can not connect to the remote server!\n";
    }

    std::thread dataReceiver(dataReceiverFunc);
    dataReceiver.detach();
    std::thread dataSender(dataSenderFunc);
    dataSender.detach();

    PlayerMap player1("Player 1");
    PlayerMap player2("Player 2", PlayerMap::remote);

    player1.start(window);
    std::cout << "asd\n";

    sendData(json({{"action", "getMap"}, {"status", "wait"}}));
    WaitUntilReady wait(player2.player, player1.getJSONData(), false);
    wait.start(window);

    remotePlayerFight playerFight(player1.player, player2.player, false);
    playerFight.start(window);
}


int main() {

    loadConfigFromJSON();

    sf::RenderWindow window;
    window.create(sf::VideoMode(gStdWidth, gStdHeight), "SeaBattle");

    if (!initTextures()) return -1; // try to init textures
    if (!initFonts()) return -1; // try to init textures

    mainMenue lobby;
    lobby.start(window);

    if (lobby.gameMode == mainMenue::local_game) localGame(window);
    else if (lobby.gameMode == mainMenue::server_game) serverGame(window);
    else if (lobby.gameMode == mainMenue::client_game) clientGame(window);

    window.close();

    return 0;
}