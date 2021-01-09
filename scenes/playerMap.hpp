#ifndef PLAUER1
#define PLAUER1

#include "../coreEngine/scene.hpp"
#include "../coreObjects/spriteObject.hpp"
#include "../gameObjects/gameMap.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>

class PlayerMap : public scene {
private:
    int chankSize, deckSize, MapSize;
    sf::Font font;
    sf::Text playerName;
    spriteObject rotateButton, enterButton;

public:
    gameMap player;

    PlayerMap (int chankSize = 50, int deckSize = 40, std::string name = "Player 1", int MapSize = 10) {
        this->chankSize = chankSize;
        this->deckSize = deckSize;
        this->MapSize = MapSize;

        this->player = gameMap(MapSize, chankSize, sf::Vector2i(0, 50));
        this->rotateButton = spriteObject(
            sf::Vector2i(600, 500),
            sf::Vector2i(100, 100),
            1, sf::Vector2f(0.25f, 0.25f)
        );
        this->enterButton = spriteObject(
            sf::Vector2i(800, 500),
            sf::Vector2i(100, 150),
            1, sf::Vector2f(0.25f, 0.25f)
        );

        player.initMap();
        player.initShips(sf::Vector2i(600, 100));

        // rotateButton.initShape(sf::Vector2i(200, 50), sf::Color(255, 0, 0), sf::Color(255, 20, 20), 0);
        rotateButton.initTextures({"/home/romaasd/Documents/Projects/test/seaBattleTest/src/rotate.png"});
        rotateButton.initSprite(0);
        enterButton.initTextures({"/home/romaasd/Documents/Projects/test/seaBattleTest/src/enter.png"});
        enterButton.initSprite(0);

        if (!font.loadFromFile("/home/romaasd/Documents/Projects/test/seaBattleTest/src/PermanentMarker-Regular.ttf")) {
            std::cerr << "Unable to load font";
        }
        playerName.setFont(font);
        playerName.setString(name);
        playerName.setCharacterSize(70);
        playerName.setPosition(sf::Vector2f(50, 15));
    } 

    virtual void drawShapes(sf::RenderWindow& window) {
        for (int i = 0; i < MapSize; i++) {
            for (int j = 0; j < MapSize; j++) {
                if (player(i, j).show) {
                    window.draw(player(i, j).sprite);
                }
            }
        }

        for (auto & ship : player.ships) {
            window.draw(ship.shape);
        }

        window.draw(rotateButton.sprite);
        window.draw(enterButton.sprite);
        window.draw(playerName);
    }

    virtual void mouseButtonPressedEvent(sf::Event e) {
        int x = e.mouseButton.x, y = e.mouseButton.y;

        if (enterButton.doesContein(sf::Vector2i(x, y))) {
            bool shouldStop = true;
            for (auto && ship : player.ships) {
                shouldStop = shouldStop && ship.placed; 
            }
            
            if (shouldStop) this->stop();
        }

        for (auto && ship : player.ships) {
            if (ship.doesContein(sf::Vector2i(x, y))) {
                ship.inMove = true;
                ship.select();
                ship.setPosition(sf::Vector2i(x, y));
                player.unassignShip(ship);
                player.FlushChankStatuses();
            } else if (ship.selected && !ship.inMove && ship.placed) {
                if (rotateButton.doesContein(sf::Vector2i(x, y))) {
                    player.unassignShip(ship);
                    player.FlushChankStatuses();
                    ship.rotate();
                    if (!player.validateShipLocation(ship.ID, ship.decks[0].first, ship.decks[0].second)) {
                        ship.rotate();
                    }
                    player.assignShip(ship.ID, ship.decks[0].first, ship.decks[0].second);
                } else {
                    ship.unselect();
                }
            }
        }
    }

    virtual void mouseMovedEvent(sf::Event e) {
        int x = e.mouseMove.x, y = e.mouseMove.y;

        for (auto && ship : player.ships) {
            if (ship.inMove) {
                ship.setPosition(sf::Vector2i(x, y));
            }
        }
    }

    virtual void mouseButtonRealisedEvent(sf::Event e) {
        int x = e.mouseButton.x, y = e.mouseButton.y;

        for (auto && ship : player.ships) {
            if (ship.inMove) {
                ship.inMove = false;
                player.FlushChankStatuses();

                for (int i = 0; i < MapSize; ++i) {
                    for (int j = 0; j < MapSize; ++j) {
                        if (player(i, j).doesContein(sf::Vector2i(x, y))) {
                            if (player.validateShipLocation(ship.ID, i, j)) {
                                ship.setPosition(
                                    player(i, j).position
                                    + sf::Vector2i((chankSize-deckSize)/2, (chankSize-deckSize)/2)
                                );
                                ship.savePosition();
                                player.assignShip(ship.ID, i, j);
                            } else ship.moveBack();
                        } else ship.moveBack();
                    }
                }
            }
        }
    }
};

#endif