#ifndef COLLIDER
#define COLLIDER

#include <SFML/System/Vector2.hpp>
#include <iostream>

class collider {
private:
    sf::Vector2i lu, rd;

public:
    bool isActive;

    collider() {
        this->lu = sf::Vector2i(0, 0);
        this->rd = sf::Vector2i(0, 0);
        this->isActive = false;
    }

    collider(std::pair<int, int> left_up, std::pair<int, int> right_down) {
        this->lu = sf::Vector2i(left_up.first, left_up.second);
        this->rd = sf::Vector2i(right_down.first, right_down.second);
        this->isActive = true;
    }

    collider(sf::Vector2i left_up, sf::Vector2i right_down) {
        this->lu = left_up;
        this->rd = right_down;
        this->isActive = true;
    }

    void activate() { this->isActive = true; }
    void deactivate() { this->isActive = false; }

    bool doesContein(sf::Vector2i point) {
        if (this->isActive)
            return this->lu.x <= point.x && point.x <= this->rd.x
                && this->lu.y <= point.y && point.y <= this->rd.y;
        
        return false;
    }

    void setPosition(sf::Vector2i newPosition) {
        this->move(newPosition-this->lu);
    }

    void move(sf::Vector2i vector) {
        this->lu += vector;
        this->rd += vector;
    }
};

#endif