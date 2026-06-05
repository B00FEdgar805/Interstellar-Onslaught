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
            mapPosition = transform->position;
        }

        float destinationTileSize = tileMap.worldTileSize();

        // Calculate the visible tile boundaries based on the camera viewport
        int startX = std::max(0, static_cast<int>((camera.POSITION.x - mapPosition.x) / destinationTileSize));
        int startY = std::max(0, static_cast<int>((camera.POSITION.y - mapPosition.y) / destinationTileSize));
        int endX = std::min(tileMap.mapWidth, static_cast<int>(((camera.POSITION.x + camera.VIEWPORT_WIDTH) - mapPosition.x) / destinationTileSize) + 1);
        int endY = std::min(tileMap.mapHeight, static_cast<int>(((camera.POSITION.y + camera.VIEWPORT_HEIGHT) - mapPosition.y) / destinationTileSize) + 1);

        for (int y = startY; y < endY; ++y)
        {
            for (int x = startX; x < endX; ++x)
            {
                int tileId = tileMap.getTile(x, y);
                if (tileId == tileMap.emptyTile || tileId < 0)
                {
                    continue;
                }

                SDL_FRect source;
                source.x = static_cast<float>(((tileId - 1) % tileMap.tilesetColumns) * tileMap.tileSize);
                source.y = static_cast<float>(((tileId - 1) / tileMap.tilesetColumns) * tileMap.tileSize);
                source.w = static_cast<float>(tileMap.tileSize);
                source.h = static_cast<float>(tileMap.tileSize);

                SDL_FRect worldDestination;
                worldDestination.x = mapPosition.x + static_cast<float>(x) * destinationTileSize;
                worldDestination.y = mapPosition.y + static_cast<float>(y) * destinationTileSize;
                worldDestination.w = destinationTileSize;
                worldDestination.h = destinationTileSize;

                SDL_FRect screenDestination = camera.worldToScreenRect(worldDestination);

                TextureManager::draw(tileMap.textureId, source, screenDestination);
            }
        }
    }
}

