#ifndef REMOTE_PLAYER_FIGHT
#define REMOTE_PLAYER_FIGHT

#include "../coreEngine/scene.hpp"
#include "../gameObjects/gameMap.hpp"
#include "playerMap.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <vector>
#include "../json.hpp"

using json = nlohmann::json;


extern int MapSize;


class remotePlayerFight : public scene {
public:
    gameMap& player1;
    gameMap& player2;
    bool hidePlayer1Ships, hidePlayer2Ships;
    sf::TcpSocket& serverSocket;
    sf::TcpSocket& clientSocket;

    bool myStep;
    bool isHost;

    remotePlayerFight(
        gameMap& player1,
        gameMap& player2,
        sf::TcpSocket& socket1,
        sf::TcpSocket& socket2,
        bool isHost = true,
        bool hidePlayer1Ships = false,
        bool hidePlayer2Ships = true
    )
    : player1(player1), player2(player2), serverSocket(socket1), clientSocket(socket2) {

        this->player2.move(sf::Vector2i(600, 0));
        this->hidePlayer1Ships = hidePlayer1Ships;
        this->hidePlayer2Ships = hidePlayer2Ships;

        updateChanksStatuses();
        myStep = isHost;
        this->isHost = isHost;
    }

    void updateChanksStatuses() {
        for (int i = 0; i < MapSize; i++) {
            for (int j = 0; j < MapSize; j++) {
                if (player1(i, j).status == chank::reserved) player1(i, j).setStatus(chank::water);
                if (player1(i, j).status == chank::ship && hidePlayer1Ships) 
                    player1(i, j).setTexture(player1(i, j).getTexture(chank::water));

                if (player2(i, j).status == chank::reserved) player2(i, j).setStatus(chank::water);
                if (player2(i, j).status == chank::ship && hidePlayer2Ships) 
                    player2(i, j).setTexture(player2(i, j).getTexture(chank::water));
            }
        }
    }

    virtual void drawShapes(sf::RenderWindow& window) {
        for (int i = 0; i < MapSize; i++) {
            for (int j = 0; j < MapSize; j++) {
                if (player1(i, j).show) {
                    window.draw(player1(i, j).sprite);
                }
                if (player2(i, j).show) {
                    window.draw(player2(i, j).sprite);
                }
            }
        }
    }

    virtual void mouseButtonPressedEvent(sf::Event e) {
        int x = e.mouseButton.x, y = e.mouseButton.y;
        sf::Vector2i mouse(x, y);

        if (!myStep) return; 

        for (int i = 0; i < MapSize; i++) { for (int j = 0; j < MapSize; j++) {
            if (!player2(i, j).doesContein(mouse))
                continue;

            json data;
            // std::cout << "MouseButtonPressed\n";
            if (isHost) {
                data = procesShoutResults(i, j, player2);
                // std::cout << "MouseButtonPressed::Host::dataProcessed\n";
                std::cout << data.dump(4) << "\n";
                sendData(clientSocket, data.dump());
                // std::cout << "MouseButtonPressed::Host::dataSent\n";
            } else {
                sendData(serverSocket, json({{"coordX", i}, {"coordY", j}}).dump());
                data = json::parse(receiveData(serverSocket));
                // std::cout << "MouseButtonPressed::Guest::dataReceivednadParsed\n";
                std::cout << data.dump(4) << "\n";
            }
            shootHandler(data, player2);
        }}
    }

    virtual void loopUpdate() {
        if (myStep) return;

        // std::cout << "loopUpdate\n";

        json data;
        if (isHost) {
            data = json::parse(receiveData(clientSocket));
            data = procesShoutResults(
                data["coordX"].get<int>(),
                data["coordY"].get<int>(),
                player1
            );
            sendData(clientSocket, data.dump());
            // std::cout << "loopUpdate::dataSent\n";
        } else {
            data = json::parse(receiveData(serverSocket));
            // std::cout << "loopUpdate::dataReseived\n";
        }

        shootHandler(data, player1);
    }

    json procesShoutResults(int x, int y, gameMap &player) {
        json data = {
            {"coordX", x},
            {"coordY", y},
            {"shouldStop", false},
            {"hostStep", true}
        };

        switch (player(x, y).status) {
        case chank::water:
            data["chankStatus"] = "water";
            data["hostStep"] = !myStep;
            break;
        case chank::ship:
            data["chankStatus"] = "ship";
            data["hostStep"] = myStep;
            data["shouldBeKilled"] = !player.checkShip(player(x, y).shipID, true);
            break;
        default:
            data["chankStatus"] = "nothing";
            data["hostStep"] = myStep;
            break;
        }

        data["shouldStop"] = !player.checkStatus();

        return data;
    }

    void shootHandler(json data, gameMap &player) {
        int x = data["coordX"].get<int>();
        int y = data["coordY"].get<int>();

        // std::cout << "shootHandler\n";

        std::string chankStatus = data["chankStatus"].get<std::string>();

        if (chankStatus == "water") {
            // std::cout << "shootHandler::Water\n";
            player(x, y).setStatus(chank::unright_shoot);
        } else if (chankStatus == "ship") {
            // std::cout << "shootHandler::Ship\n";
            player(x, y).setStatus(chank::right_shoot);
            if (data["shouldBeKilled"].get<bool>()) {
                player.killShip(player.ships[player(x, y).shipID]);
            }
        }

        // std::cout << "shootHandler::main_if_handled\n";

        if (isHost) myStep = data["hostStep"].get<bool>();
        else myStep = !data["hostStep"].get<bool>();

        if (data["shouldStop"].get<bool>()) stop();

        // std::cout << "shootHandler::actions_completed\n";
    }

};

#endif