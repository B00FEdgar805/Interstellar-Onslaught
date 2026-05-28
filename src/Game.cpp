#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "ECS/Components/Transform.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Components/Velocity.hpp"
#include "ECS/Components/PlayerControl.hpp"
#include "ECS/Components/BoxCollider.hpp"
#include "ECS/Components/TileMap.hpp"
#include "ECS/Components/Animation.hpp"
#include "ECS/CollisionSystem.hpp"
#include "ECS/Systems.hpp"
#include "ECS/Registry.hpp"
#include "ECS/TileSystem.hpp"

// Init ECS system

SDL_Renderer* Game::RENDERER = nullptr;
Map* map;
Registry registry;
Entity player = registry.create();;
Systems systems;


Game::Game()
{
    init("SDL Game", 800, 640, false);
}

Game::~Game()
{
    clean();
}

void Game::init(const char* title, int width, int height, bool fullscreen)  // Init screen and creates enitites
{
    int flags = 0;
    
    if(fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    else
    {
        flags = SDL_WINDOW_RESIZABLE;
    }
    
    // Initialize SDL (video + events)
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s\n", SDL_GetError());
        SDL_Quit();
    }
    
    // Create a window
    WINDOW = SDL_CreateWindow(title, width, height, flags);
    if (!WINDOW)
    {
        SDL_Log("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
    }
    
    // Create a renderer
    RENDERER = SDL_CreateRenderer(WINDOW, nullptr);
    if (!RENDERER)
    {
        SDL_Log("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(WINDOW);
        SDL_Quit();
    }
    
    SDL_SetRenderLogicalPresentation(RENDERER, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    
    RUNNING = true;
    
    // Inits player and map
    
    map = new Map();
    
    Entity blackhole = registry.create();
    
    TextureManager::loadTexture("player", "Assets/SpaceshipAnimation.png");
    TextureManager::loadTexture("Blackhole", "Assets/blackhole2.png");
    TextureManager::loadTexture("Level1", "Assets/TileMap1.png");

    registry.add(blackhole, Transform(260.0f, 230.0f));
    registry.add(blackhole, Sprite("Blackhole", Vector2D(320.0f, 180.0f)));
    registry.add(blackhole, BoxCollider(
        Vector2D(320.0f, 180.0f),
        Vector2D(0.0f, 0.0f),
        false,     // isTrigger
        true,     // isStatic
        "blackhole"
    ));
    
    
    registry.add(player, Sprite("player"));
    registry.add(player, Transform(Vector2D(100.0f, 100.0f)));
    registry.add(player, Velocity());
    registry.add(player, PlayerControl());
    registry.add(player, BoxCollider(Vector2D(32.0f, 32.0f), Vector2D(0.0f, 0.0f), false, false, "player"));
    registry.add(player, Animation(DELTA_TIME, 3, 150));
    Entity level = registry.create();

    registry.add(level, Transform(0.0f, 0.0f));

    TileMap levelMap = Map::loadFromFile(
        "Assets/Maps/Level1.txt",
        "Level1",
        32,     // tile size in the tileset image
        2,      // how many columns the tileset has
        1.0f,   // scale: 32px tiles become 64px on screen
        0      // empty tile value
    );

    registry.add(level, levelMap);

}

int Game::run()
{
    auto lastTime = std::chrono::steady_clock::now();

    while (isRunning()) // Main game loop
    {
        auto currentTime = std::chrono::steady_clock::now();

        DELTA_TIME = std::chrono::duration<float>(currentTime - lastTime).count();

        lastTime = currentTime;

        if (DELTA_TIME > 0.05f)
        {
            DELTA_TIME = 0.05f;
        }

        handleEvents(); // Handles user inputes
        update();   // Handlers movemnts systems
        render();   // Handles any rendering
        //UpdateFPSCounter(DELTA_TIME);
    }
    
    clean();    // Called when program ends to close safley
    return 0;
}


void Game::handleEvents()
{
    SDL_Event e;
    SDL_PollEvent(&e);
    switch (e.type)     // Handles ending the program
    {
        case SDL_EVENT_QUIT:
            RUNNING = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (e.key.key == SDLK_ESCAPE)
            {
                RUNNING = false;
            }
            break;
        default:
            break;
            
    }
    systems.playerInputSystem(registry);    // player inputs
}

void Game::update()
{
   // collisionSystem(registry);
    std::vector<CollisionEvent> collisions = collisionSystem(registry);

    for (const CollisionEvent& collision : collisions)
    {
        BoxCollider* a = registry.get<BoxCollider>(collision.a);
        BoxCollider* b = registry.get<BoxCollider>(collision.b);

        if (a == nullptr || b == nullptr)
        {
            continue;
        }

        if (
            collision.isTrigger &&
            (
                (a->tag == "player" && b->tag == "blackhole") ||
                (a->tag == "blackhole" && b->tag == "player")
            )
        )
        {
            SDL_Log("Player touched blackhole!");
        }
    }
    
    systems.movementSystem(registry, DELTA_TIME);
}

void Game::render()
{
    if (!SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 0))
    {
        SDL_Log("SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
    }
    if (!SDL_RenderClear(RENDERER))
    {
        SDL_Log("SDL_RenderClear failed: %s\n", SDL_GetError());
    }
    
    //map -> drawMap();
    RenderTileMap(registry);
    systems.renderSystem(registry, RENDERER);

    SDL_RenderPresent(RENDERER);
}

void Game::clean()
{
    // Cleanup
    TextureManager::clear();
    SDL_DestroyRenderer(RENDERER);
    SDL_DestroyWindow(WINDOW);
    SDL_Quit();
}

bool Game::isRunning()
{
    return RUNNING;
}

void Game::UpdateFPSCounter(float deltaTime)   // Quick FPS counter for testing
{
    static int frames = 0;
    static float accumulator = 0.0f;

    frames++;
    accumulator += deltaTime;

    if (accumulator >= 1.0f)
    {
        float fps = frames / accumulator;
        SDL_Log("FPS: %.1f", fps);
        frames = 0;
        accumulator = 0.0f;
    }
}
