#include "config.cpp"
#include "coreEngine/scene.hpp"
#include "gameObjects/gameMap.hpp"
#include "scenes/playerMap.hpp"
#include "scenes/playerFight.hpp"
#include "scenes/remotePlayerFight.hpp"
#include "scenes/mainMenue.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>


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

void localGame(sf::RenderWindow &window) {
    PlayerMap player1("Player 1");
    PlayerMap player2("Player 2");

    player1.start(window);
    player2.start(window);

    PlayerFight playerFight(player1.player, player2.player);
    playerFight.start(window);
}

int main() {

    loadConfigFromJSON();

    sf::TcpListener server;
    sf::TcpSocket serverSocket;
    sf::TcpSocket clientSocket;

    sf::RenderWindow window;
    window.create(sf::VideoMode(gStdWidth, gStdHeight), "SeaBattle");

    if (!initTextures()) return -1; // try to init textures
    if (!initFonts()) return -1; // try to init textures

    mainMenue lobby;
    lobby.start(window);

    if (lobby.gameMode == mainMenue::local_game) localGame(window);
    else if (lobby.gameMode == mainMenue::server_game) {
        if (server.listen(23457) != sf::Socket::Done) {
            std::cerr << "Can not listen on port 23457\n";
            return -1; // Handle it like that for the time being
        }

        if (server.accept(clientSocket) != sf::Socket::Done) {
            std::cerr << "Can not accept the connection\n";
            return -1;
        }

        PlayerMap player1("Player 1");
        PlayerMap player2("Player 2", PlayerMap::remote);

        player1.start(window);
        
        json data = json::parse(receiveData(clientSocket));
        if (data["status"] == "wait" && data["action"] == "getMap") {
            sendData(clientSocket, player1.getJSONData().dump());
        } else return -1;

        data = json::parse(receiveData(clientSocket));
        if (data["status"] == "ok" && data["action"] == "getMap") {
            player2.getRemoteMap(clientSocket);
        } else return -1;

        remotePlayerFight playerFight(player1.player, player2.player, serverSocket, clientSocket, true);
        playerFight.start(window);
    }
    else if (lobby.gameMode == mainMenue::client_game) {
        if (serverSocket.connect("127.0.0.1", 23457) != sf::Socket::Done) {
            std::cerr << "Can not connect to the remote server!\n";
        }

        PlayerMap player1("Player 1");
        PlayerMap player2("Player 2", PlayerMap::remote);

        player1.start(window);
        player2.getRemoteMap(serverSocket);

        json data = json::parse(receiveData(serverSocket));
        if (data["status"] == "wait" && data["action"] == "getMap") {
            sendData(serverSocket, player1.getJSONData().dump());
        } else return -1;

        data = json::parse(receiveData(serverSocket));
        if (data["status"] == "ok" && data["action"] == "getMap") {
        } else return -1;

        remotePlayerFight playerFight(player1.player, player2.player, serverSocket, clientSocket, false);
        playerFight.start(window); 
    }

    window.close();

    return 0;
}