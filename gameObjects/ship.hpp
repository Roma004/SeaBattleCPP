#ifndef SHIP
#define SHIP

#include "../coreObjects/shapeObject.hpp"
#include "../coreObjects/collider.hpp"
#include "chank.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

extern int chankSize, deckSize;
extern textures_dict textures;

class ship : public spriteObject {
private:
    sf::Vector2i lastPosition;

public:
    int type; // 1, 2, 3, 4 decks
    std::vector<std::pair<int, int>> decks;
    enum directions{horizontal = 0, vertical = 1} direction;
    sf::Vector2i scale;
    bool inMove, selected, placed;
    int ID;

    ship (
        int ID, int type = 1, 
        sf::Vector2i position = sf::Vector2i(0, 0),
        directions direction = horizontal
    ) {
        this->ID = ID;
        this->type = type;
        this->direction = direction;
        decks = std::vector<std::pair<int, int>>(type);

        if (direction == horizontal) {
            scale = sf::Vector2i(type*chankSize - (chankSize-deckSize), deckSize);
        } else {
            scale = sf::Vector2i(deckSize, type*chankSize - (chankSize-deckSize));
        }

        setPosition(position);
        objectCollider = collider(position, position+scale);
        attachCollider = true;
        savePosition();
        activate();

        inMove = false;
        selected = false;
        placed = false;
        texturesScale = sf::Vector2f(0.5f, 0.5f);
        initSprite(getTexture());
    }

    void select() {
        selected = true;
        // sprite.setColor(sf::Color::Black);
    }

    void unselect() {
        selected = false;
        // sprite.setColor(sf::Color::White); 
    }

    void savePosition() {
        lastPosition = position;
    }

    void rotate() {
        if (direction == vertical) {
            direction = horizontal;
            scale = sf::Vector2i(type*chankSize - (chankSize-deckSize), deckSize);
        } else {
            direction = vertical;
            scale = sf::Vector2i(deckSize, type*chankSize - (chankSize-deckSize));
        }

        objectCollider = collider(position, position+scale);
        // this->initShape(scale, sf::Color(150, 150, 150), sf::Color(100, 100, 100));
        // changeTexture();
        initSprite(getTexture());
    }

    void moveBack() { 
        setPosition(lastPosition);
    }

    void changeTexture() {
        setTexture(textures["ship"][type + 4*direction - 1]);
    }

    sf::Texture& getTexture() {
        return textures["ship"][type + 4*direction - 1];
    }
};

#endif
