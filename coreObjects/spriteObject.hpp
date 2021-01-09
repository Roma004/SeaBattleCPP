#ifndef SPRITE_OBJECT
#define SPRITE_OBJECT

#include "collideredObject.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
class spriteObject : public collideredObject {
private:
    std::vector<sf::Texture> textures;

protected:
    int texturesCount;
    sf::Vector2f texturesScale ;

public:
    sf::Sprite sprite;

    spriteObject() {
        this->show = true;
        this->position = sf::Vector2i(0, 0);
        this->attachCollider = true;
        this->texturesCount = 0;
        this->texturesScale = sf::Vector2f(1, 1);
    }

    spriteObject(sf::Vector2i position, sf::Vector2i scale, int texturesCount, sf::Vector2f texturesScale) {
        this->show = true;
        this->objectCollider = collider(position, position+scale);
        this->position = position;
        this->attachCollider = true;
        this->texturesCount = texturesCount;
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

    void initTextures(std::vector<std::string> textureFiles) {
        this->textures = std::vector<sf::Texture>(this->texturesCount);
        for (int i = 0; i < this->texturesCount; i++) {
            if (!this->textures[i].loadFromFile(textureFiles[i])) {
                std::cerr << "Unable to load texture from file `" << texturesCount << "`.\n";
            }
            this->textures[i].setSmooth(true);
        }
    }

    void setTexture(int id) {
        this->sprite.setTexture(this->textures[id]);
    }

    void initSprite(int textureID) {
        this->setTexture(textureID);
        this->sprite.setPosition(sf::Vector2f(position));
        this->sprite.scale(sf::Vector2f(texturesScale));
    }
};

#endif