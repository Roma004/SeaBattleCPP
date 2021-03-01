#ifndef MAIN_MENUE
#define MAIN_MENUE

#include "../coreEngine/scene.hpp"
#include "../coreObjects/shapeObject.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <cstdio>

class mainMenue : public scene {
public:
    shapeObject twoPlayers;
    shapeObject byIP;

    enum gameModes {singlePlay = 0, oneToOne = 1, remote = 2} gameMode; 

    mainMenue() {
        twoPlayers = shapeObject(sf::Vector2i(200, 255), sf::Vector2i(200, 150));
        byIP = shapeObject(sf::Vector2i(800, 255), sf::Vector2i(200, 150));

        twoPlayers.initShape(sf::Vector2i(200, 150), sf::Color::Red, sf::Color::Red);
        byIP.initShape(sf::Vector2i(200, 150), sf::Color::Blue, sf::Color::Red);
    }

    virtual void drawShapes(sf::RenderWindow & window) {
        window.draw(twoPlayers.shape);
        window.draw(byIP.shape);
    }

    virtual void mouseButtonPressedEvent(sf::Event e) {
        int x = e.mouseButton.x, y = e.mouseButton.y;
        sf::Vector2i mPos(x, y);


        if (twoPlayers.doesContein(sf::Vector2i(x, y))) {
            gameMode = oneToOne;
            this->stop();
        } else if (byIP.doesContein(mPos)) {
            gameMode = remote;
            this->stop();
        }
    }
};

#endif