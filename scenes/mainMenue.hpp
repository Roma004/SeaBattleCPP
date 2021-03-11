#ifndef MAIN_MENUE
#define MAIN_MENUE

#include "../headers.hpp"
#include "../coreEngine/scene.hpp"
#include "../coreObjects/shapeObject.hpp"
#include "../coreObjects/spriteObject.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

extern textures_dict textures;

class mainMenue : public scene {
private:
    spriteObject localGame;
    spriteObject serverGame;
    spriteObject clientGame;

public:
    enum gameModes {local_game = 0, server_game = 1, client_game = 2} gameMode; 

    mainMenue() {
        localGame = spriteObject(sf::Vector2i(300, 25), sf::Vector2i(600, 150), sf::Vector2f(1, 1));
        serverGame = spriteObject(sf::Vector2i(300, 200), sf::Vector2i(600, 150), sf::Vector2f(1, 1));
        clientGame = spriteObject(sf::Vector2i(300, 375), sf::Vector2i(600, 150), sf::Vector2f(1, 1));

        localGame.initSprite(textures["mainMenue"][0]);
        serverGame.initSprite(textures["mainMenue"][1]);
        clientGame.initSprite(textures["mainMenue"][2]);
    }

    virtual void drawShapes(sf::RenderWindow & window) {
        window.draw(localGame.sprite);
        window.draw(serverGame.sprite);
        window.draw(clientGame.sprite);
    }

    virtual void mouseButtonPressedEvent(sf::Event e) {
        int x = e.mouseButton.x, y = e.mouseButton.y;
        sf::Vector2i mPos(x, y);

        if (localGame.doesContein(mPos)) {
            gameMode = local_game;
            this->stop();
        } else if (serverGame.doesContein(mPos)) {
            gameMode = server_game;
            this->stop();
        } else if (clientGame.doesContein(mPos)) {
            gameMode = client_game;
            this->stop();
        }
    }
};

#endif