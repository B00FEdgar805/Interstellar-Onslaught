//
//  Map.cpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/11/26.
//

#include "Map.hpp"
#include "TextureManager.hpp"


//int level[500];

int level1[20][25] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

};

Map::Map()
{
    STARS = TextureManager::loadTexture("Assets/stars.png");
    STAR = TextureManager::loadTexture("Assets/star.png");
    PLANET = TextureManager::loadTexture("Assets/planet.png");
    BLACKHOLE = TextureManager::loadTexture("Assets/blackhole2.png");
    SPACE = TextureManager::loadTexture("Assets/space.png");
    
    loadMap(level1);
    SRC_RECT.x = 0;
    SRC_RECT.y = 0;
    SRC_RECT.w = 32;
    SRC_RECT.h = 32;
    DEST_RECT.w = 32;
    DEST_RECT.h = 32;
    DEST_RECT.x = 0;
    DEST_RECT.y = 0;
    SRC_BLACKHOLE.x = 0;
    SRC_BLACKHOLE.y = 0;
    SRC_BLACKHOLE.w = 320;
    SRC_BLACKHOLE.h = 180;
    DEST_BLACKHOLE.x = 240;
    DEST_BLACKHOLE.y = 0;
    DEST_BLACKHOLE.w = 320;
    DEST_BLACKHOLE.h = 180;
}

void Map::loadMap(int arr[20][25])
{
    for (int r = 0; r < 20; r++)
    {
        for (int c = 0; c < 25; c++)
        {
            //MAP[r][c] = arr[r][c];
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(0, 50);
            MAP[r][c] = distr(gen);
            
        }
    }
}

void Map::drawMap()
{
    int type = 0;
    
    for (int r = 0; r < 20; r++)
    {
        for (int c = 0; c < 25; c++)
        {
            type = MAP[r][c];
            DEST_RECT.x = c * 32;
            DEST_RECT.y = r * 32;
            
            switch (type)
            {
                case 0:
                    TextureManager::draw(STARS, SRC_RECT, DEST_RECT);
                    break;
                case 1:
                    TextureManager::drawRotated(STARS, SRC_RECT, DEST_RECT, 90.f);
                    break;
                case 2:
                    TextureManager::drawRotated(STARS, SRC_RECT, DEST_RECT, 180.f);
                    break;
                case 3:
                    TextureManager::drawRotated(STARS, SRC_RECT, DEST_RECT, 270.f);
                    break;
                case 4:
                    TextureManager::draw(STAR, SRC_RECT, DEST_RECT);
                    break;
                case 5:
                    TextureManager::drawRotated(STAR, SRC_RECT, DEST_RECT, 90.f);
                    break;
                case 6:
                    TextureManager::drawRotated(STAR, SRC_RECT, DEST_RECT, 180.f);
                    break;
                case 7:
                    TextureManager::drawRotated(STAR, SRC_RECT, DEST_RECT, 270.f);
                    break;
                case 8:
                    TextureManager::draw(PLANET, SRC_RECT, DEST_RECT);
                    break;
                default:
                    TextureManager::draw(SPACE, SRC_RECT, DEST_RECT);
                    //TextureManager::drawRotated(PLANET, SRC_RECT, DEST_RECT, 90.f);


                    break;
            }
             
        }
    }
    
    TextureManager::draw(BLACKHOLE, SRC_BLACKHOLE, DEST_BLACKHOLE);
}
