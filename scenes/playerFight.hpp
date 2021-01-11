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
    gameMap& player1;
    gameMap& player2;
    bool hidePlayer1Ships, hidePlayer2Ships;

    PlayerFight(
        gameMap& player1,
        gameMap& player2,
        bool hidePlayer1Ships = true,
        bool hidePlayer2Ships = true
    )
    : player1(player1), player2(player2) {

        this->player2.move(sf::Vector2i(600, 0));
        this->hidePlayer1Ships = hidePlayer1Ships;
        this->hidePlayer2Ships = hidePlayer2Ships;

        updateChanksStatuses();
    }

    void updateChanksStatuses() {
        for (int i = 0; i < MapSize; i++) {
            for (int j = 0; j < MapSize; j++) {
                if (player1(i, j).status == chank::reserved) player1(i, j).setStatus(chank::water);
                if (player1(i, j).status == chank::ship && hidePlayer1Ships) 
                    player1(i, j).setTexture(player1(i, j).getTexture(chank::water));

                if (player2(i, j).status == chank::reserved) player2(i, j).setStatus(chank::water);
                if (player2(i, j).status == chank::ship && hidePlayer2Ships) 
                    player2(i, j).setTexture(player2(i, j).getTexture(chank::water));
            }
        }
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