#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cstddef>
#include "json.hpp"

using json = nlohmann::json;


typedef std::map<std::string, std::vector<sf::Texture>> textures_dict;
typedef std::map<std::string, std::vector<std::string>> textureFiles_dict;
typedef std::map<std::string, std::string> fontFiles_dict;
typedef std::map<std::string, sf::Font> fonts_dict;