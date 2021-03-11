#include "headers.hpp"
#include <fstream>


textures_dict textures;
fonts_dict fonts;
int gStdHeight, gStdWidth;
int chankSize;
int deckSize;
int MapSize;
std::string fontConf = "configs/fonts.json";
std::string textureConf = "configs/textures.json";
std::string generalConf = "configs/general.json";

fontFiles_dict fontFiles = {};
textureFiles_dict textureFiles = {};

void loadConfigFromJSON() {
    std::ifstream confStream(fontConf);
    std::string conf(
        (std::istreambuf_iterator<char>(confStream)),
        (std::istreambuf_iterator<char>())
    );
    fontFiles = json::parse(conf).get<fontFiles_dict>();
    confStream.close();
    confStream.open(textureConf);
    conf.assign(
        (std::istreambuf_iterator<char>(confStream)),
        (std::istreambuf_iterator<char>())
    );
    textureFiles = json::parse(conf).get<textureFiles_dict>();
    confStream.close();
    confStream.open(generalConf);
    conf.assign(
        (std::istreambuf_iterator<char>(confStream)),
        (std::istreambuf_iterator<char>())
    );

    std::map<std::string, int> generalVars = json::parse(conf).get<std::map<std::string, int>>();

    gStdHeight = generalVars["gStdHeight"];
    gStdWidth = generalVars["gStdWidth"];
    chankSize = generalVars["chankSize"];
    deckSize = generalVars["deckSize"];
    MapSize = generalVars["MapSize"];
}
