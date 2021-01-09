#ifndef PLAYER_FIGHT
#define PLAYER_FIGHT

#include "../coreEngine/scene.hpp"
#include "../gameObjects/gameMap.hpp"
#include "playerMap.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>


extern int MapSize;


class PlayerFight : public scene {
public:
    gameMap& player1, player2;

    PlayerFight(
        gameMap& player1,
        gameMap& player2
    )
    : player1(player1), player2(player2) {

        this->player2.move(sf::Vector2i(600, 0));
    }

    virtual void drawShapes(sf::RenderWindow& window) {
        for (int i = 0; i < MapSize; i++) {
            for (int j = 0; j < MapSize; j++) {
                if (player1(i, j).show) {
                    window.draw(player1(i, j).sprite);
                }
                if (player2(i, j).show) {
                    window.draw(player2(i, j).sprite);
                }
            }
        }
    }
};

#endif