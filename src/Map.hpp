#ifndef Map_hpp
#define Map_hpp

#include "ECS/Components/TileMap.hpp"

#include <SDL3/SDL.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

struct Map
{
    static TileMap loadFromFile(const std::string& filepath, const std::string& textureId,int tileSize, int tilesetColumns, float scale = 1.0f, int emptyTile = 0);
};


#endif /* Map_hpp */
