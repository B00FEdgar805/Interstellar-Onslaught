#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "ECS/Components/Transform.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Components/Velocity.hpp"
#include "ECS/Components/PlayerControl.hpp"
#include "ECS/Components/BoxCollider.hpp"
#include "ECS/CollisionSystem.hpp"
#include "ECS/Systems.hpp"
#include "ECS/Registry.hpp"

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
    
    Entity coin = registry.create();

    registry.add(coin, Transform(500.0f, 300.0f));
    registry.add(coin, Sprite("player"));
    registry.add(coin, BoxCollider(
        Vector2D(32.0f, 32.0f),
        Vector2D(0.0f, 0.0f),
        false,     // isTrigger
        true,     // isStatic
        "coin"
    ));
    
    TextureManager::loadTexture("player", "Assets/Spaceship.png");
    registry.add(player, Sprite("player"));
    registry.add(player, Transform(Vector2D(100.0f, 100.0f)));
    registry.add(player, Velocity());
    registry.add(player, PlayerControl());
    registry.add(player, BoxCollider(Vector2D(32.0f, 32.0f), Vector2D(0.0f, 0.0f), false, false, "player"));

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
                (a->tag == "player" && b->tag == "coin") ||
                (a->tag == "coin" && b->tag == "player")
            )
        )
        {
            SDL_Log("Player touched coin!");
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
    
    map -> drawMap();
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
