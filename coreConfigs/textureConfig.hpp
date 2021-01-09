#ifndef TEXTURE_CONFIG
#define TEXTURE_CONFIG

#include <SFML/Graphics/Texture.hpp>
#include <map>
#include <string>
#include <vector>
#include <iostream>

typedef std::map<std::string, std::vector<int>> dict;

class textureConfig {
public:
    std::vector<sf::Texture> textures;
    dict textureIDs;

    textureConfig(std::vector<std::string> textureFiles, dict textureIDs) {
        this->textureIDs = textureIDs;
        this->loadtextures(textureFiles);
    }

    void loadtextures(std::vector<std::string> &textureFiles) {
        textures = std::vector<sf::Texture>(textureFiles.size());
        for (long unsigned int i = 0; i < textureFiles.size(); i++) {
            if (!textures[i].loadFromFile(textureFiles[i])) {
                std::cerr << "Unable to load texture from file `" << textureFiles[i] << "`\n";
            }
            textures[i].setSmooth(true);
        }
    }
};

#endif