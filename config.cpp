#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>


typedef std::map<std::string, std::vector<sf::Texture>> textures_dict;
typedef std::map<std::string, std::vector<std::string>> textureFiles_dict;
typedef std::map<std::string, std::string> fontFiles_dict;
typedef std::map<std::string, sf::Font> fonts_dict;

textures_dict textures;
fonts_dict fonts;
int gStdHeight, gStdWidth;
int chankSize;
int deckSize;
int MapSize;
// std::string path;
std::string fontConf = "configs/fonts";
std::string textureConf = "configs/textures";
std::string generalConf = "configs/general";

fontFiles_dict fontFiles = {};
textureFiles_dict textureFiles = {};


void loadFontNames() {
    std::ifstream fin; fin.open(fontConf);
    std::string line = "";

    while (true) {
        fin >> line;
        if (line == "!!!") break;

        fin >> fontFiles[line];
    }
}

void loadGeneralConfig() {
    std::ifstream fin; fin.open(generalConf);
    std::string line = "";
    std::map<std::string, std::string> tempConf;

    while (true) {
        fin >> line;
        if (line == "!!!") break;

        fin >> tempConf[line];
    }

    // path = tempConf["path"];
    gStdHeight = std::stoi(tempConf["gStdHeight"]);
    gStdWidth = std::stoi(tempConf["gStdWidth"]);
    chankSize = std::stoi(tempConf["chankSize"]);
    deckSize = std::stoi(tempConf["deckSize"]);
    MapSize = std::stoi(tempConf["MapSize"]);

    fin.close();
}

void loadTextureNames() {
    std::ifstream fin; fin.open(textureConf);

    std::string line = "", current_key = "";
    bool addvalue = false, update_key = false;
    int line_number = 1;

    std::getline(fin, line);
    while (line != "!!!") {

        if (line == "<>") {
            addvalue = false;
            update_key = true;
        } else if (line == ">>>>") {
            addvalue = true;
        } else if (addvalue) {
            textureFiles[current_key].push_back(line);
        } else if (update_key) {
            current_key = line;
            textureFiles[current_key] = std::vector<std::string>(0);
            update_key = false;
        } else {
            throw std::runtime_error("Can not read file `" + textureConf + "` correctly. Invalid Syntax in line " + std::to_string(line_number));
        }

        ++line_number;
        std::getline(fin, line);
    }

    fin.close();
}
