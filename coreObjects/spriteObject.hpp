#ifndef SPRITE_OBJECT
#define SPRITE_OBJECT

#include "collideredObject.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
class spriteObject : public collideredObject {
protected:
    sf::Vector2f texturesScale ;

public:
    sf::Sprite sprite;

    spriteObject() {
        this->show = true;
        this->position = sf::Vector2i(0, 0);
        this->attachCollider = true;
        this->texturesScale = sf::Vector2f(1, 1);
    }

    spriteObject(sf::Vector2i position, sf::Vector2i scale, sf::Vector2f texturesScale) {
        this->show = true;
        this->objectCollider = collider(position, position+scale);
        this->position = position;
        this->attachCollider = true;
        this->texturesScale = texturesScale;
    }


    void setPosition(sf::Vector2i newPosition) {
        this->position = newPosition;
        this->sprite.setPosition(sf::Vector2f(newPosition));
        this->objectCollider.setPosition(newPosition);
    }

    void move(sf::Vector2i vector) {
        this->position += vector;
        if (this->attachCollider) this->objectCollider.move(vector);
        this->sprite.move(sf::Vector2f(vector));
    }

    void setTexture(sf::Texture& texture) {
        this->sprite.setTexture(texture);
    }

    void initSprite(sf::Texture& texture) {
        this->setTexture(texture);
        this->sprite.setPosition(sf::Vector2f(position));
        this->sprite.scale(sf::Vector2f(texturesScale));
    }
};

#endif