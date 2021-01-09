#ifndef BASIC_OBJECT
#define BASIC_OBJECT
#include "collider.hpp"
#include <SFML/System/Vector2.hpp>
#include <iostream>

class basicObject {
public:
    bool show;
    sf::Vector2i position;
    
    basicObject() {
        this->show = false;
        this->position = sf::Vector2i(0, 0);
    }

    basicObject(sf::Vector2i position, int texturesCount) {
        this->show = true;
        this->position = position;
    }

    void setPosition(sf::Vector2i newPosition) {
        this->position = newPosition;
    }

    void activate() {
        this->show = true;
    }

    void deactivate() {
        this->show = false;
    }

    void move(sf::Vector2i vector) {
        this->position += vector;
    }
};

#endif