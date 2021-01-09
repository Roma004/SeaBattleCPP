#ifndef GAME_MAP
#define GAME_MAP

#include "../coreObjects/basicObject.hpp"
#include "chank.hpp"
#include "ship.hpp"
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <utility>
#include <vector>
#include <cassert>
#include <iostream>


class gameMap : public basicObject {
public:
    int size;
    int chankSize;
    std::vector<std::vector<chank>> chanks;
    std::vector<ship> ships;

    gameMap(int size = 10, int chankSize = 50, sf::Vector2i position = sf::Vector2i(0, 0)) {
        this->size = size;
        this->position = position;
        this->chankSize = chankSize;
    }

    void initMap() {
        sf::Vector2i newPose(this->position.x+this->chankSize, this->position.y+this->chankSize);
        // int count = 0;
        for (int i = 0; i < this->size; i++) {
            this->chanks.push_back(std::vector<chank>());
            for (int j = 0; j < this->size; j++) {
                this->chanks[i].push_back(chank(newPose, i, j, this->chankSize, chank::water));
                newPose.y += this->chankSize;

                // std::cout << i << " " << j << " " << count++ << "\n";
            }
            newPose.y = this->position.y + this->chankSize;
            newPose.x += this->chankSize;
        }
    }

    void initShips(sf::Vector2i shipsPosinton) {
        int ID = 0;
        for (int type = 1; type < 5; type++) {
            for (int count = 0; count < 5-type; count++) {
                this->ships.push_back(ship(
                    ID++, type,
                    sf::Vector2i(
                        shipsPosinton.x + count*(type*50 + 50),
                        shipsPosinton.y + 100*(type - 1)
                    ),
                    ship::horizontal
                ));
            }
        }
    }

    bool validateShipLocation(int ID, int X, int Y) {
        ship& newShip = ships[ID];

        for (
            int i = X, j = Y;
            i < X + newShip.type && j < Y + newShip.type; 
            i += (newShip.direction == ship::horizontal),
            j += (newShip.direction == ship::vertical)
        ) {
            if (i == 10 || j == 10) {
                return false;
            }

            if (chanks[i][j].status != chank::water) {
                return false;
            }
        }

        return true;
    }

    void FlushChankStatuses() {
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                chanks[i][j].setStatus(chank::water);
        
        for (auto && ship : ships) {
            if (ship.placed)
                assignShip(ship.ID, ship.decks[0].first, ship.decks[0].second);
        }
    }

    void assignShip(int ID, int X, int Y) {
        ship& newShip = ships[ID];

        if (newShip.direction == ship::horizontal) {
            int startX = std::max(0, X-1), startY = std::max(0, Y-1);
            int endX = std::min(10, X+1+newShip.type), endY = std::min(10, Y+2);

            for (int i = startX; i < endX; ++i) {
                for (int j = startY; j < endY; ++j) {
                    chanks[i][j].setStatus(chank::reserved);
                }
            }

            for (int i = X; i < X+newShip.type; ++i) {
                chanks[i][Y].setStatus(chank::ship);
                newShip.decks[i-X] = {i, Y};
            }
        } else {
            int startX = std::max(0, X-1), startY = std::max(0, Y-1);
            int endX = std::min(10, X+2), endY = std::min(10, Y+1+newShip.type);

            for (int i = startX; i < endX; ++i) {
                for (int j = startY; j < endY; ++j) {
                    chanks[i][j].setStatus(chank::reserved);
                }
            }

            for (int i = Y; i < Y+newShip.type; ++i) {
                chanks[X][i].setStatus(chank::ship);
                newShip.decks[i-Y] = {X, i};
            }
        }

        newShip.placed = true;
    }

    void unassignShip(ship & assignedShip) {
        int X = assignedShip.decks[0].first;
        int Y = assignedShip.decks[0].second;
        if (assignedShip.direction == ship::horizontal) {
            int startX = std::max(0, X-1), startY = std::max(0, Y-1);
            int endX = std::min(10, X+1+assignedShip.type), endY = std::min(10, Y+2);

            for (int i = startX; i < endX; ++i) {
                for (int j = startY; j < endY; ++j) {
                    chanks[i][j].setStatus(chank::water);
                }
            }
        } else {
            int startX = std::max(0, X-1), startY = std::max(0, Y-1);
            int endX = std::min(10, X+1), endY = std::min(10, Y+1+assignedShip.type);

            for (int i = startX; i < endX; ++i) {
                for (int j = startY; j < endY; ++j) {
                    chanks[i][j].setStatus(chank::water);
                }
            }
        }

        assignedShip.placed = false;
    }

    chank& operator()(int X, int Y) {
        assert(X >= 0 && X < this->size);
        assert(Y >= 0 && Y < this->size);

        return this->chanks[X][Y];
    }

    void move(sf::Vector2i vector) {
        this->position += vector;

        for (auto && line : chanks) {
            for (auto && chank : line) {
                chank.move(vector);
            }
        }
    }
};

#endif