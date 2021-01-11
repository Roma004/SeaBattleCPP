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
        "src/reserved.png",
        // "src/test_ship.png",
        "src/reserved.png"
    }},
    {"ship", {
        "src/ship1_rotated.png",
        "src/ship2_rotated.png",
        "src/ship3_rotated.png",
        "src/ship4_rotated.png",
        "src/ship1.png",
        "src/ship2.png",
        "src/ship3.png",
        "src/ship4.png"
    }},
    {"rotateButton", {"src/rotate.png"}},
    {"enterButton", {"src/enter.png"}}
};
