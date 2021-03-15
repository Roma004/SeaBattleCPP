#ifndef WAIT_UNTIL_READY
#define WAIT_UNTIL_READY

// #include "../headers.hpp"
#include "../coreEngine/scene.hpp"
#include "../coreObjects/shapeObject.hpp"
#include "../coreObjects/spriteObject.hpp"
#include "playerMap.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Window/Event.hpp>
#include <mutex>
#include "../utils/sendData.cpp"

extern textures_dict textures;
// extern std::queue<json> gRecvDataQueue;
// extern std::queue<json> gSentDataQueue;
// extern std::mutex gReceivedDataMutex;
// extern std::mutex gSentDataMutex;

class WaitUntilReady : public scene {
private:
    spriteObject wait;
    bool isHost;
    json myMapdata;
    gameMap &enemyMap;

public:

    WaitUntilReady(gameMap &player2, json myMapdata, bool isHost = true)
    : enemyMap(player2) {
        wait = spriteObject(sf::Vector2i(600, 300), sf::Vector2i(300, 300), sf::Vector2f(1, 1));

        wait.initSprite(textures["wait"][0]);
        wait.sprite.setOrigin(150, 150);

        this->myMapdata = myMapdata;
        this->isHost = isHost;

        // std::cout << "wait_init\n";
    }

    virtual void drawShapes(sf::RenderWindow & window) {
        window.draw(wait.sprite);
    }

    virtual void loopUpdate() {
        wait.sprite.rotate(0.3);

        if (!ableToReceiveData()) return;
        // std::cout << "wait_loop_update\n";

        json data = receiveData();

        if (data.find("chanks") != data.end() && data.find("ships") != data.end()) {
            enemyMap.ChanksFromJSON(data["chanks"]);
            enemyMap.ShipsFromJSON(data["ships"]);

            // std::cout << "map"

            if (!isHost) {
                // std::cout << "Guest::mapReceived\n";
                sendData(json({{"action", "getMap"}, {"status", "ok"}}));
            } else {
                // std::cout << "Host::ableToStart\n";
                sendData(json({{"action", "startGame"}, {"status", "ok"}}));
                stop();
            }
        } else if (data["status"] == "wait" && data["action"] == "getMap") {
            // std::cout << (isHost ? "Host::mapSent\n" : "Guest::mapSent\n");

            sendData(myMapdata);
        } else if (data["status"] == "ok" && data["action"] == "getMap") {
            // std::cout << (isHost ? "Host::getMapRequest\n" : "Guest::getMapRequest\n");

            sendData(json({{"action", "getMap"}, {"status", "wait"}}));
        } else if (data["status"] == "ok" && data["action"] == "startGame") {
            // std::cout << (isHost ? "Host::startGame\n" : "Guest::startGame\n");
            stop();
        } else {
            std::cout << "Unknown data:\n" << data.dump(4) << "\n";
        }
    }

};

#endif