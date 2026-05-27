//
//  TileMap.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/27/26.
//

#ifndef TileMap_hpp
#define TileMap_hpp

#include "Component.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

class TileMap final : public BaseComponent
{
public:
    std::string textureId;

    int mapWidth = 0;
    int mapHeight = 0;

    int tileSize = 32;
    int tilesetColumns = 1;

    float scale = 1.0f;

    int emptyTile = 0;

    std::vector<int> tiles;

public:
    TileMap() = default;

    TileMap(
        std::string textureId, int mapWidth, int mapHeight, int tileSize, int tilesetColumns, float scale = 1.0f, int emptyTile = 0)
        : textureId(std::move(textureId)),
          mapWidth(mapWidth),
          mapHeight(mapHeight),
          tileSize(tileSize),
          tilesetColumns(tilesetColumns),
          scale(scale),
          emptyTile(emptyTile)
    {
    }

    bool isValid() const
    {
        return mapWidth > 0 &&
               mapHeight > 0 &&
               tileSize > 0 &&
               tilesetColumns > 0 &&
               tiles.size() == static_cast<std::size_t>(mapWidth * mapHeight);
    }

    float worldTileSize() const
    {
        return static_cast<float>(tileSize) * scale;
    }

    int getTile(int x, int y) const
    {
        if (x < 0 || y < 0 || x >= mapWidth || y >= mapHeight)
        {
            return emptyTile;
        }

        return tiles[static_cast<std::size_t>(y * mapWidth + x)];
    }
};
#endif /* TileMap_hpp */
