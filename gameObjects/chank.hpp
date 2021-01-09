#ifndef CHANK
#define CHANK

#include "../coreObjects/spriteObject.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>


extern int MapSize;


class chank : public spriteObject {
public:
    int X, Y;
    int shipID, deckID;
    enum statuses {null = 0, water = 1, ship = 2, reserved = 3} status = water;

    chank(
        sf::Vector2i position,
        int X = 0, int Y = 0,
        statuses status = null,
        int shipID = -1, int deckID = -1
    ) {
        this->objectCollider = collider(position, position + sf::Vector2i(MapSize, MapSize));
        this->attachCollider = true;
        this->setPosition(position);
        this->activate();

        this->status = status;
        this->X = X;
        this->Y = Y;
        this->shipID = shipID;
        this->deckID = deckID;

        this->texturesCount = 4;
        this->texturesScale = sf::Vector2f(0.5f, 0.5f);
        this->initTextures({
            "/home/romaasd/Documents/Projects/test/seaBattleTest/src/null.png",
            "/home/romaasd/Documents/Projects/test/seaBattleTest/src/water.png",
            "/home/romaasd/Documents/Projects/test/seaBattleTest/src/test_ship.png",
            "/home/romaasd/Documents/Projects/test/seaBattleTest/src/reserved.png"
        });
        this->initSprite(0);
        this->setTexture(status);
    }

    void setStatus(statuses newStatus) {
        this->status = newStatus;
        setTexture(newStatus);
    }
};

#endif