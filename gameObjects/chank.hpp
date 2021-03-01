#ifndef CHANK
#define CHANK

#include "../coreObjects/spriteObject.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>


typedef std::map<std::string, std::vector<sf::Texture>> textures_dict;

extern int MapSize, chankSize;
extern textures_dict textures; 


class chank : public spriteObject {
public:
    int X, Y;
    int shipID, deckID;
    enum statuses {
        null = 0,
        water = 1,
        ship = 2,
        reserved = 3,
        right_shout = 4,
        unright_shout = 5
    } status = water;

    chank(
        sf::Vector2i position,
        int X = 0, int Y = 0,
        statuses status = null,
        int shipID = -1, int deckID = -1
    ) {
        // std::cout << MapSize << "\n";
        sf::Vector2i asd(chankSize, chankSize);
        sf::Vector2i asdf = position + asd;
        // std::cout << asdf.x << " " << asdf.y << "\n";
        this->objectCollider = collider(position, asdf);
        this->attachCollider = true;
        this->setPosition(position);
        this->activate();

        this->status = status;
        this->X = X;
        this->Y = Y;
        this->shipID = shipID;
        this->deckID = deckID; 

        this->texturesScale = sf::Vector2f(0.5f, 0.5f);
        this->initSprite(getTexture());
        this->changeTexture(); 
    }

    sf::Texture& getTexture() {
        return textures["chank"][status];
    }

    sf::Texture& getTexture(statuses status) {
        return textures["chank"][status];
    }

    void changeTexture() {
        switch(status) {
            // case ship:
            //     if (deckID == 0)
            //         this->setTexture(textures["deck"][0])
            //     // else if (deckID)
            default:
                this->setTexture(textures["chank"][status]);
                break;
        }
    }

    void setStatus(statuses newStatus) {
        this->status = newStatus;
        this->changeTexture();
    }
};

#endif