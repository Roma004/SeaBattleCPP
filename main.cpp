#include "coreConfigs/textureConfig.hpp"
#include "coreEngine/scene.hpp"
#include "scenes/playerMap.hpp"
#include "scenes/playerFight.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <map>
#include <string>
#include <string>

const int chankSize = 50, deckSize = 40, MapSize = 10;
std::string path = "/home/romaasd/Documents/Projects/test/seaBattleTest/";

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode(1050, 650), "SeaBattle");

    // textureConfig textures({
    //     path+"src/enter.png",
    //     path+"src/rotate.png",
    //     path+"src/null.png",
    //     path+"src/water.png",
    //     path+"src/test_ship.png",
    //     path+"src/reserved.png",
    //     path+"src/ship1.png",
    //     path+"src/ship2.png",
    //     path+"src/ship3.png",
    //     path+"src/ship4.png",
    //     path+"src/ship1_rotated.png",
    //     path+"src/ship2_rotated.png",
    //     path+"src/ship3_rotated.png",
    //     path+"src/ship4_rotated.png",
    // }, {
    //     {"chank", {2, 3, 4, 5}},
    //     {"ship", {6, 7, 8, 9, 10, 11, 12, 13}},
    //     {"rotateButton", {1}},
    //     {"enterButton", {0}}
    // });

    PlayerMap player1(50, 40, "Player 1", 10);
    PlayerMap player2(50, 40, "Player 2", 10);
    PlayerFight playerFight(player1.player, player2.player, 10);

    player1.start(window);
    player2.start(window);
    playerFight.start(window);

    window.close();

    return 0;
}