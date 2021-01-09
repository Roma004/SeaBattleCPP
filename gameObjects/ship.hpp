#ifndef SHIP
#define SHIP

#include "../coreObjects/shapeObject.hpp"
#include "../coreObjects/collider.hpp"
#include "chank.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

class ship : public shapeObject {
private:
    sf::Vector2i lastPosition;
    int chankSize, deckSize;

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
        directions direction = horizontal,
        int deckSize = 40, int chankSize = 50
    ) {
        this->ID = ID;
        this->type = type;
        this->direction = direction;
        this->decks = std::vector<std::pair<int, int>>(type);
        this->chankSize = chankSize;
        this->deckSize = deckSize;

        if (direction == horizontal) {
            this->scale = sf::Vector2i(type*chankSize - (chankSize-deckSize), deckSize);
        } else {
            this->scale = sf::Vector2i(deckSize, type*chankSize - (chankSize-deckSize));
        }

        this->objectCollider = collider(position, position+scale);
        this->attachCollider = true;
        this->setPosition(position);
        this->savePosition();
        this->activate();

        this->inMove = false;
        this->selected = false;
        this->placed = false;
        this->initShape(scale, sf::Color(150, 150, 150), sf::Color(100, 100, 100));
    }

    void select() {
        selected = true;
        shape.setFillColor(sf::Color(50, 50, 50));
    }

    void unselect() {
        selected = false;
        shape.setFillColor(sf::Color(150, 150, 150));
    }

    void savePosition() {
        this->lastPosition = this->position;
    }

    void rotate() {
        if (direction == vertical) {
            this->direction = horizontal;
            this->scale = sf::Vector2i(type*chankSize - (chankSize-deckSize), deckSize);
        } else {
            this->direction = vertical;
            this->scale = sf::Vector2i(deckSize, type*chankSize - (chankSize-deckSize));
        }

        this->objectCollider = collider(position, position+scale);
        this->initShape(scale, sf::Color(150, 150, 150), sf::Color(100, 100, 100));
    }

    void moveBack() {
        this->setPosition(this->lastPosition);
    }
};

#endif
