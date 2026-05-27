//
//  TileSystem.cpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/27/26.
//

#include "TileSystem.hpp"
#include "../TextureManager.hpp"
#include "Components/TileMap.hpp"
#include "Components/Transform.hpp"

#include "../Math.hpp"

#include <SDL3/SDL.h>

void RenderTileMap(Registry& registry)
{
    for (auto& [entity, tileMap] : registry.all<TileMap>())
    {
        if (!tileMap.isValid())
        {
            continue;
        }

        Vector2D mapPosition(0.0f, 0.0f);

        Transform* transform = registry.get<Transform>(entity);

        if (transform != nullptr)
        {
            mapPosition = transform->position;
        }

        float destinationTileSize = tileMap.worldTileSize();

        for (int y = 0; y < tileMap.mapHeight; y++)
        {
            for (int x = 0; x < tileMap.mapWidth; x++)
            {
                int tileId = tileMap.getTile(x, y);

                if (tileId == tileMap.emptyTile)
                {
                    continue;
                }
                
                if (tileId < 0)
                {
                    continue;
                }

                SDL_FRect source;
                
                // Gets the tile from tile set

                source.x = static_cast<float>(((tileId - 1) % tileMap.tilesetColumns) * tileMap.tileSize);

                source.y = static_cast<float>(((tileId - 1) / tileMap.tilesetColumns) * tileMap.tileSize);

                source.w = static_cast<float>(tileMap.tileSize);
                source.h = static_cast<float>(tileMap.tileSize);

                SDL_FRect destination;
                
                // Draws the tile location based on tile size and array order

                destination.x = mapPosition.x + static_cast<float>(x) * destinationTileSize;
                destination.y = mapPosition.y + static_cast<float>(y) * destinationTileSize;
                destination.w = destinationTileSize;
                destination.h = destinationTileSize;

                TextureManager::draw(tileMap.textureId,source,destination);
            }
        }
    }
}
