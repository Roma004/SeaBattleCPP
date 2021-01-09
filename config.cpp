#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <vector>
#include <map>


typedef std::map<std::string, std::vector<sf::Texture>> textures_dict;
typedef std::map<std::string, std::vector<std::string>> textureFiles_dict;

textures_dict textures;
int chankSize = 50;
int deckSize = 40;
int MapSize = 10;
std::string path = "/home/romaasd/Documents/Projects/test/seaBattleTest/";

textureFiles_dict textureFiles = {
    {"chank", {
        "src/null.png",
        "src/water.png",
        "src/test_ship.png",
        "src/reserved.png"
    }},
    {"rotateButton", {"src/rotate.png"}},
    {"enterButton", {"src/enter.png"}}
};
