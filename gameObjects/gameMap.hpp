#ifndef GAME_MAP
#define GAME_MAP

#include "../coreObjects/basicObject.hpp"
#include "chank.hpp"
#include "ship.hpp"
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <vector>
#include <cassert>
#include <iostream>


extern int MapSize, chankSize;


class gameMap : public basicObject {
public:
    std::vector<std::vector<chank>> chanks;
    std::vector<ship> ships;

    gameMap(sf::Vector2i position) {
        this->position = position;
    }

    void initMap() {
        sf::Vector2i newPose(this->position.x+chankSize, this->position.y+chankSize);
        // int count = 0;
        for (int i = 0; i < MapSize; i++) {
            this->chanks.push_back(std::vector<chank>());
            for (int j = 0; j < MapSize; j++) {
                this->chanks[i].push_back(chank(newPose, i, j, chank::water));
                newPose.y += chankSize;

                // std::cout << i << " " << j << " " << count++ << "\n";
            }
            newPose.y = this->position.y + chankSize;
            newPose.x += chankSize;
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

    bool validateShipLocation(int X, int Y, int type, int direction) {
        for (
            int i = X, j = Y;
            i < X + type && j < Y + type; 
            i += (direction == ship::horizontal),
            j += (direction == ship::vertical)
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
        for (int i = 0; i < MapSize; i++)
            for (int j = 0; j < MapSize; j++)
                chanks[i][j].setStatus(chank::water);
        
        for (auto && ship : ships) {
            if (ship.placed)
                assignShip(ship, ship.decks[0].first, ship.decks[0].second);
        }
    }

    void assignShip(ship& newShip, int X, int Y) {

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
                chanks[i][Y].shipID = newShip.ID;
                chanks[i][Y].deckID = i-X;
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
                chanks[X][i].shipID = newShip.ID;
                chanks[X][i].deckID = i-Y;
                newShip.decks[i-Y] = {X, i};
            }
        }

        newShip.placed = true;
    }

    void unassignShip(ship & assignedShip) {
        int X = assignedShip.decks[0].first;
        int Y = assignedShip.decks[0].second;

        int startX, startY, endX, endY;
        if (assignedShip.direction == ship::horizontal) {
            startX = std::max(0, X-1), startY = std::max(0, Y-1);
            endX = std::min(10, X+1+assignedShip.type), endY = std::min(10, Y+2);
        } else {
            startX = std::max(0, X-1), startY = std::max(0, Y-1);
            endX = std::min(10, X+2), endY = std::min(10, Y+1+assignedShip.type);
        }

        for (int i = startX; i < endX; ++i) {
            for (int j = startY; j < endY; ++j) {
                chanks[i][j].setStatus(chank::water);
            }
        }

        assignedShip.placed = false;
    }

    void clear() {
        for (auto && ship : ships) ship.reset();
        FlushChankStatuses();
    }

    void randomiseShips() {
        srand(std::clock());
        int x, y, direction;

        clear();

        for (auto && ship : ships) {
            while (true) {
                x = std::rand() % 10;
                y = std::rand() % 10;
                direction = std::rand() % 2;

                if (validateShipLocation(x, y, ship.type, direction)) {
                    ship.setPosition(chanks[x][y].position);
                    if (ship.direction != direction)
                        ship.rotate();
                    ship.savePosition();
                    assignShip(ship, x, y);

                    break;
                }
            }
        }
    }

    bool checkShip(int ID) {
        int killedCount = 0;
        for (auto&& [x, y] : ships[ID].decks) 
            if (chanks[x][y].status == chank::right_shout)
                killedCount++;

        if (killedCount == ships[ID].type) {
            return false;
        }

        return true;
    }

    void killShip(ship& shipToKill) {
        int X = shipToKill.decks[0].first;
        int Y = shipToKill.decks[0].second;

        int startX, startY, endX, endY;
        if (shipToKill.direction == ship::horizontal) {
            startX = std::max(0, X-1), startY = std::max(0, Y-1);
            endX = std::min(10, X+1+shipToKill.type), endY = std::min(10, Y+2);
        } else {
            startX = std::max(0, X-1), startY = std::max(0, Y-1);
            endX = std::min(10, X+2), endY = std::min(10, Y+1+shipToKill.type);
        }

        for (int i = startX; i < endX; ++i) {
            for (int j = startY; j < endY; ++j) {
                if (chanks[i][j].status != chank::right_shout)
                    chanks[i][j].setStatus(chank::unright_shout);
            }
        }

        shipToKill.alive = false;
    }

    void hadleShout(int X, int Y, bool& first, bool& second) {
        switch (chanks[X][Y].status) {
        case chank::water:
            chanks[X][Y].setStatus(chank::unright_shout);
            std::swap(first, second);
            break;
        case chank::ship:
            chanks[X][Y].setStatus(chank::right_shout);
            if (!checkShip(chanks[X][Y].shipID)) killShip(ships[chanks[X][Y].shipID]);
            break;
        default:
            break;
        }
    }

    bool checkStatus() {
        for (auto && ship : ships) {
            if (ship.alive) return true;
        }

        return false;
    }

    chank& operator()(int X, int Y) {
        assert(X >= 0 && X < MapSize);
        assert(Y >= 0 && Y < MapSize);

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