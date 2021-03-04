#include "config.cpp"
#include "coreEngine/scene.hpp"
#include "scenes/playerMap.hpp"
#include "scenes/playerFight.hpp"
#include "scenes/mainMenue.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <string>
#include <vector>


bool initTextures() { // init textures, size is not a matter
    // for (auto && [category, fileNames] : textureFiles) { 
    for (auto && textureFile : textureFiles) { 
        auto category = textureFile.first;
        auto fileNames = textureFile.second;
        textures[category] = std::vector<sf::Texture>(fileNames.size());
        for (size_t i = 0; i < fileNames.size(); ++i) {
            if (!textures[category][i].loadFromFile(path + fileNames[i])) {
                std::cerr << "Unable to load texture from file `" << path << fileNames[i] << "`\n";
                return false;
            }

            textures[category][i].setSmooth(true);
        }
    }

    return true;
}

bool initFonts() {

    for (auto && fontFile : fontFiles) {
        fonts[fontFile.first] = sf::Font();
        if (!fonts[fontFile.first].loadFromFile(path + fontFile.second)) {
            std::cerr << "Unable to load texture from file `" << path << fontFile.second << "`\n";
            return false;
        }
    }

    return true;
}

int main() {
    loadGeneralConfig();
    loadTextureNames();
    loadFontNames();

    sf::RenderWindow window;
    window.create(sf::VideoMode(gStdWidth, gStdHeight), "SeaBattle");

    if (!initTextures()) return -1; // try to init textures
    if (!initFonts()) return -1; // try to init textures

    // mainMenue lobby;
    // lobby.start(window);

    // if (lobby.gameMode == mainMenue::oneToOne) {
    PlayerMap player1("Player 1");
    PlayerMap player2("Player 2");

    player1.start(window);
    player2.start(window);

    PlayerFight playerFight(player1.player, player2.player);
    playerFight.start(window);
    // }

    window.close();

    return 0;
}