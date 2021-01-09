#ifndef COLLIDERED_OBJECT
#define COLLIDERED_OBJECT
#include "collider.hpp"
#include "basicObject.hpp"
#include <SFML/System/Vector2.hpp>
#include <iostream>

class collideredObject : public basicObject {
protected:
    collider objectCollider;

public:
    bool show;
    bool attachCollider;
    
    collideredObject() {
        this->show = true;
        this->position = sf::Vector2i(0, 0);
        this->attachCollider = true;
    }

    collideredObject(sf::Vector2i position, sf::Vector2i scale, int texturesCount) {
        this->objectCollider = collider(position, position+scale);
        this->show = true;
        this->position = position;
        this->attachCollider = true;
    }

    void setPosition(sf::Vector2i newPosition) {
        this->position = newPosition;
        this->objectCollider.setPosition(newPosition);
    }

    void activate() {
        this->show = true;
        this->objectCollider.activate();
    }

    void deactivate() {
        this->show = false;
        this->objectCollider.deactivate();
    }

    void move(sf::Vector2i vector) {
        this->position += vector;
        if (this->attachCollider) this->objectCollider.move(vector);
    }

    bool doesContein(sf::Vector2i point) {
        return this->objectCollider.doesContein(point);
    }
};

#endif