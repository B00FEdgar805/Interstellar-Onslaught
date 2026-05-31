#include "Map.hpp"


TileMap Map::loadFromFile(const std::string& filepath, const std::string& textureId, int tileSize, int tilesetColumns, float scale, int emptyTile)
{
    std::ifstream file(filepath);

    if (!file.is_open())
    {
        SDL_Log("Failed to open tile map file: %s", filepath.c_str());
        return TileMap();
    }

    std::vector<int> tiles;

    int mapWidth = 0;
    int mapHeight = 0;

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line))
    {
        lineNumber++;

        // Allow both "1 2 3" and "1,2,3".
        std::replace(line.begin(), line.end(), ',', ' ');

        std::istringstream stream(line);

        std::vector<int> row;
        int tileId = 0;

        while (stream >> tileId)
        {
            row.push_back(tileId);
        }

        // Skip empty lines.
        if (row.empty())
        {
            continue;
        }

        if (mapWidth == 0)
        {
            mapWidth = static_cast<int>(row.size());
        }
        else if (static_cast<int>(row.size()) != mapWidth)
        {
            SDL_Log(
                "Tile map error in %s at line %d: expected %d tiles, got %d.",
                filepath.c_str(),
                lineNumber,
                mapWidth,
                static_cast<int>(row.size())
            );

            return TileMap();
        }

        tiles.insert(tiles.end(), row.begin(), row.end());
        mapHeight++;
    }

    TileMap tileMap(textureId, mapWidth, mapHeight, tileSize, tilesetColumns, scale, emptyTile);

    tileMap.tiles = std::move(tiles);

    if (!tileMap.isValid())
    {
        SDL_Log("Tile map failed validation: %s", filepath.c_str());
        return TileMap();
    }

    return tileMap;
}
