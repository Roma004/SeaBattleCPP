#include "config.cpp"
#include "coreEngine/scene.hpp"
#include "scenes/playerMap.hpp"
#include "scenes/playerFight.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <string>
#include <vector>


bool initTextures() {
    // textures["chank"] = std::vector<sf::Texture>(4); 
    for (auto && [category, fileNames] : textureFiles) { 
        textures[category] = std::vector<sf::Texture>(fileNames.size());
        for (size_t i = 0; i < fileNames.size(); ++i) {
            if (!textures[category][i].loadFromFile(fileNames[i])) {
                std::cerr << "Unable to load texture from file `" << path << fileNames[i] << "`\n";
                return false;
            }

            textures[category][i].setSmooth(true);
        }
    }

    return true;
}


int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode(1200, 650), "SeaBattle");

    if (!initTextures()) return -1; 

    PlayerMap player1("Player 1");
    PlayerMap player2("Player 2");

    player1.start(window);
    player2.start(window);

    PlayerFight playerFight(player1.player, player2.player, false, false);
    playerFight.start(window);

    window.close();

    return 0;
}