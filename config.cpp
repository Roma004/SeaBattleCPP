#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <vector>
#include <map>


typedef std::map<std::string, std::vector<sf::Texture>> textures_dict;

textures_dict textures;
int chankSize = 50, deckSize = 40, MapSize = 10;
std::string path = "/home/romaasd/Documents/Projects/test/seaBattleTest/";