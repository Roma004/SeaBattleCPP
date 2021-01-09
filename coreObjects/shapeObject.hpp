#ifndef SHAPE_OBJECT
#define SHAPE_OBJECT
#include "collideredObject.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>

class shapeObject : public collideredObject {

public:
    sf::RectangleShape shape;
    
    shapeObject() {
        this->show = true;
        this->position = sf::Vector2i(0, 0);
        this->attachCollider = true;
    }

    shapeObject(sf::Vector2i position, sf::Vector2i scale) {
        this->objectCollider = collider(position, position+scale);
        this->show = true;
        this->position = position;
        this->attachCollider = true;
    }

    void setPosition(sf::Vector2i newPosition) {
        this->position = newPosition;
        this->shape.setPosition(sf::Vector2f(newPosition));
        this->objectCollider.setPosition(newPosition);
    }

    void move(sf::Vector2i vector) {
        this->position += vector;
        if (this->attachCollider) this->objectCollider.move(vector);
        this->shape.setPosition(sf::Vector2f(vector));
    }

    void initShape(
        sf::Vector2i scale,
        sf::Color color,
        sf::Color outlineColor,
        double outlineThickness = 5.f
    ) {
        this->shape = sf::RectangleShape(sf::Vector2f(scale));
        this->shape.setFillColor(color);
        this->shape.setPosition(sf::Vector2f(position));
        this->shape.setOutlineThickness(outlineThickness);
        this->shape.setOutlineColor(outlineColor);
    }
};

#endif