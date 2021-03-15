#ifndef PLAUER1
#define PLAUER1

#include "../coreEngine/scene.hpp"
#include "../coreObjects/spriteObject.hpp"
#include "../gameObjects/gameMap.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <exception>
#include <iostream>
// #include "../utils/sendSocketData.cpp"
#include "../json.hpp"

using json = nlohmann::json;

extern int chankSize, deckSize, MapSize;
extern std::map<std::string, sf::Font> fonts;
extern textures_dict textures;

class PlayerMap : public scene {
private:
    sf::Font font;
    sf::Text playerName;
    spriteObject rotateButton, enterButton;
    shapeObject randomButton;

public:
    gameMap player;
    enum mode {normal = 0, remote = 1} playerMode;

    PlayerMap (std::string name, mode playerMode = normal)
    : player(*(new gameMap(sf::Vector2i(0, 50)))), playerMode(playerMode) {

        player.initMap();
        player.initShips(sf::Vector2i(600, 100));

        if (playerMode == normal) {
            this->rotateButton = spriteObject(
                sf::Vector2i(600, 500),
                sf::Vector2i(100, 100),
                sf::Vector2f(0.25f, 0.25f)
            );
            this->enterButton = spriteObject(
                sf::Vector2i(800, 500),
                sf::Vector2i(100, 150),
                sf::Vector2f(0.25f, 0.25f)
            );
            this->randomButton = shapeObject(
                sf::Vector2i(1100, 500),
                sf::Vector2i(50, 50)
            );

            rotateButton.initSprite(textures["rotateButton"][0]);
            enterButton.initSprite(textures["enterButton"][0]);
            randomButton.initShape(sf::Vector2i(50, 50), sf::Color::Red, sf::Color::Red);

            playerName.setFont(fonts["PermanentMarker"]);
            playerName.setString(name);
            playerName.setCharacterSize(70);
            playerName.setPosition(sf::Vector2f(50, 15));
        }
    }

    json getJSONData() {
        json data = {
            {"chanks", std::vector<chankData>()},
            {"ships", std::vector<json>()}
        };

        auto &&chanks = player.chanks;
        auto &&ships = player.ships; 

        for (int x = 0; x < MapSize; ++x) {
            for (int y = 0; y < MapSize; ++y) {
                data["chanks"].insert(data["chanks"].end(), json({
                    {"x", x},
                    {"y", y},
                    {"deckID", chanks[x][y].deckID},
                    {"shipID", chanks[x][y].shipID},
                    {"status", chanks[x][y].status},
                }));
            }
        }

        std::vector<std::map<std::string, int>> temp;

        for (auto && ship : ships) {
            for (int i = 0; i < (int)ship.decks.size(); ++i) {
                temp.push_back({
                    {"ID", i},
                    {"first", ship.decks[i].first},
                    {"second", ship.decks[i].second}
                });
            }

            data["ships"].insert(data["ships"].end(), json({
                {"ID", ship.ID},
                {"direction", ship.direction},
                {"type", ship.type},
                {"scale_x", ship.scale.x},
                {"scale_y", ship.scale.y},
                {"decks", temp}
            }));

            temp.clear();
        }

        return data;
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
            window.draw(ship.sprite);
        }

        window.draw(rotateButton.sprite);
        window.draw(enterButton.sprite);
        window.draw(randomButton.shape);
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

        if (randomButton.doesContein(sf::Vector2i(x, y))) {
            player.randomiseShips();
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
                    if (!player.validateShipLocation(ship.decks[0].first, ship.decks[0].second, ship.type, ship.direction)) {
                        ship.rotate();
                    }
                    player.assignShip(ship, ship.decks[0].first, ship.decks[0].second);
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
                            if (player.validateShipLocation(i, j, ship.type, ship.direction)) {
                                ship.setPosition(player(i, j).position);
                                ship.savePosition();
                                player.assignShip(ship, i, j);
                            } else ship.moveBack();
                        } else ship.moveBack();
                    }
                }
            }
        }
    }
};

#endif