#include "TileSystem.hpp"
#include "../TextureManager.hpp"
#include "Components/TileMap.hpp"
#include "Components/Transform.hpp"

#include "../Math.hpp"

#include <SDL3/SDL.h>

void RenderTileMap(Registry& registry, const Camera2D& camera)
{
    for (auto& [entity, tileMap] : registry.all<TileMap>())
    {
        if (!tileMap.isValid())
        {
            continue;
        }

        Vector2D mapPosition(0.0f, 0.0f);

        Transform* transform = registry.get<Transform>(entity);

        if (transform)
        {
            mapPosition = transform -> position;
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

                SDL_FRect worldDestination;
                
                // Draws the tile location based on tile size and array order
                

                worldDestination.x = mapPosition.x + static_cast<float>(x) * destinationTileSize;
                worldDestination.y = mapPosition.y + static_cast<float>(y) * destinationTileSize;
                worldDestination.w = destinationTileSize;
                worldDestination.h = destinationTileSize;

                SDL_FRect screenDestination = camera.worldToScreenRect(worldDestination);
                
                // Draws only tiles that are on camera
                
                int startX = static_cast<int>(camera.POSITION.x / destinationTileSize);
                int startY = static_cast<int>(camera.POSITION.y / destinationTileSize);
                int endX = static_cast<int>((camera.POSITION.x + camera.VIEWPORT_WIDTH) / destinationTileSize) + 1;
                int endY = static_cast<int>((camera.POSITION.y + camera.VIEWPORT_HEIGHT) / destinationTileSize) + 1;
                
                if (startX < 0)
                {
                    startX = 0;
                }
                
                if (startY < 0)
                {
                    startY = 0;
                }
                
                if(endX > tileMap.mapWidth)
                {
                    endX = tileMap.mapWidth;
                }
                
                if(endY > tileMap.mapHeight)
                {
                    endY = tileMap.mapHeight;
                }
                
                for(int y = startY; y < endY; y++)
                {
                    for (int x = startX; x < endX; x++)
                    {
                        TextureManager::draw(tileMap.textureId, source, screenDestination);
                    }
                }
            }
        }
    }
}
