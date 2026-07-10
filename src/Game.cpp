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
#include "ECS/Camera.hpp"
#include "ECS/ProjectileSystem.hpp"
#include "ECS/EnemyAI.hpp"
#include "ECS/Components/Health.hpp"
#include "ECS/MenuSystem.hpp"
#include "ECS/PlayerSystems.hpp"
#include "Globals.hpp"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

// Init ECS system

Registry registry;
Entity PLAYER = registry.create();
Entity level = registry.create();
Systems systems;
ProjectileSystem projectiles;
EnemyAi enemies(PLAYER);
PlayerSystems playerSystem(PLAYER);
Camera2D camera(GLOBALS::SCREEN_WIDTH, GLOBALS::SCREEN_HEIGHT);
Menu UI;


Game::Game()
{
    init("SDL Game", GLOBALS::SCREEN_WIDTH, GLOBALS::SCREEN_HEIGHT, false);
}

Game::~Game()
{
    clean();
}

void Game::init(const char* title, int width, int height, bool fullscreen)  // Init screen and creates enitites
{
    int flags = 0;
    //SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;

    if(fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    else
    {
        flags = SDL_WINDOW_RESIZABLE; // | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    }
    
    SDL_SetHint(SDL_HINT_VIDEO_DOUBLE_BUFFER, "1");
    //SDL_SetHint(SDL_WINDOW_HIGH_PIXEL_DENSITY, "1"); // or proper DPI hints depending on your rendering backend
    
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
    SDL_SetRenderVSync(RENDERER, 1);
    SDL_SetRenderLogicalPresentation(RENDERER, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    START_TIME = SDL_GetTicks();
    LAST_TIME = START_TIME;
    
    
    RUNNING = true;
    
    // Inits player and map
        
    Entity blackhole = registry.create();
    
    TextureManager::loadTexture("player", "Assets/SpaceshipAnimation.png");
    TextureManager::loadTexture("Blackhole", "Assets/blackhole2.png");
    TextureManager::loadTexture("Level1", "Assets/TileMap1.png");
    TextureManager::loadTexture("Projectile", "Assets/Projectile.png");
    TextureManager::loadTexture("Enemy", "Assets/Enemy1.png");
    TextureManager::loadTexture("Buttons", "Assets/Buttons.png");
    registry.add(blackhole, Transform(camera.worldToScreen(Vector2D(600.0f, 600.0f))));
    registry.add(blackhole, Sprite("Blackhole", Vector2D(320.0f, 180.0f).scale(2)));
    registry.add(blackhole, BoxCollider(
        Vector2D(320.0f, 180.0f).scale(1),
        Vector2D(0.0f, 0.0f),
        false,     // isTrigger
        true,     // isStatic
        "blackhole"
    ));
    
    registry.add(PLAYER, Sprite("player"));
    registry.add(PLAYER, Transform(Vector2D(100.0f, 100.0f)));
    registry.add(PLAYER, Velocity(150.0f));
    registry.add(PLAYER, PlayerControl());
    registry.add(PLAYER, BoxCollider(Vector2D(32.0f, 32.0f), Vector2D(0.0f, 0.0f), false, false, "player"));
    registry.add(PLAYER, Animation(DELTA_TIME, 3, 150));
    registry.add(PLAYER, Health(100.0f));

    registry.add(level, Transform(0.0f, 0.0f));

    enemies.createEnemy(registry, Vector2D(10.0f, 10.0f), 100.0f, DELTA_TIME);
    
    TileMap levelMap = Map::loadFromFile(
        "Assets/Maps/Level1.txt",
        "Level1",
        32,     // tile size in the tileset image
        2,      // how many columns the tileset has
        2.0f,   // scale: 32px tiles become 64px on screen
        0      // empty tile value
    );

    registry.add(level, levelMap);
    
    if (true)
    {
        initIMGUI();
    }

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
        
        
        
        switch (GLOBALS::CURRENT_STATE)
        {
            case GLOBALS::STATE_MAIN_MENU:
                UI.buttonSystem(e);
                UI.renderSystemMain(RENDERER);
                handleEvents(); // Handles user inputes
                break;
            case GLOBALS::STATE_GAMEPLAY:
                //SDL_Log("Gameplay");
                handleEvents(); // Handles user inputes
                update();   // Handlers movemnts systems
                render();   // Handles any rendering
                //UpdateFPSCounter(DELTA_TIME);
                //SDL_Delay(16);
                break;
            case GLOBALS::STATE_PAUSED:
                //DELTA_TIME = 0.0f;
                UI.buttonSystem(e);
                handleEvents(); // Handles user inputes
                render();   // Handles any rendering
                //handlePauseMenuInput(event);
                break;
            case GLOBALS::STATE_EXIT:
                //SDL_Log("Exited");
                RUNNING = false;
                break;
        }
    
    }
    
    clean();    // Called when program ends to close safley
    return 0;
}


void Game::handleEvents()
{
    SDL_PollEvent(&e);
    ImGui_ImplSDL3_ProcessEvent(&e);
    switch (e.type)     // Handles ending the program
    {

        case SDL_EVENT_QUIT:
            GLOBALS::CURRENT_STATE = GLOBALS::STATE_EXIT;
            //RUNNING = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (e.key.key == SDLK_ESCAPE)
            {
                GLOBALS::CURRENT_STATE = GLOBALS::STATE_EXIT;
                //RUNNING = false;
            }
            else if (e.key.key == SDLK_TAB)
            {
                if (GLOBALS::CURRENT_STATE == GLOBALS::STATE_PAUSED)
                {
                    GLOBALS::CURRENT_STATE = GLOBALS::STATE_GAMEPLAY;
                    //std::cout << GLOBALS::CURRENT_STATE << std::endl;

                }
                else
                {
                    GLOBALS::CURRENT_STATE = GLOBALS::STATE_PAUSED;
                    //std::cout << GLOBALS::CURRENT_STATE << std::endl;
                }
                //SDL_Log("Paued");
            }
            else if (e.key.key == SDLK_1)
            {
                if (IMGUI)
                {
                    IMGUI = false;
                }
                else
                {
                    IMGUI = true;
                }
            }
            break;
        
        default:
            break;
            
    }
    if (GLOBALS::CURRENT_STATE == GLOBALS::STATE_GAMEPLAY)
    {
        systems.playerInputSystem(registry);
    }
}

void Game::update()
{
   // need to make function for game logic
    std::vector<CollisionEvent> collisions = collisionSystem(registry);
    // Collisions events
    /*
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
    */
    
    
    systems.movementSystem(registry, DELTA_TIME);
    enemies.enemySpawnSystem(registry, DELTA_TIME);
    enemies.enemyAISystem(registry);
    enemies.enemyCollisions(registry, collisions);
    projectiles.projectilesCollisons(registry, collisions);
    projectiles.projectileSystem(registry, DELTA_TIME);
    playerSystem.fireSystem(projectiles, registry);
    
   
    
    // camera systems
    
    Transform* player_transform = registry.get<Transform>(PLAYER);

    if (player_transform)
    {
        camera.follow(player_transform -> position, 8.0f, DELTA_TIME);
    }
    
    
    
    TileMap* tilemap = registry.get<TileMap>(level);
    if(tilemap)
    {
        camera.clampToWorld(tilemap -> mapWidth * tilemap -> worldTileSize(), tilemap -> mapHeight * tilemap -> worldTileSize());
    }
}

void Game::render()
{
    if (!SDL_RenderClear(RENDERER))
    {
        SDL_Log("SDL_RenderClear failed: %s\n", SDL_GetError());
    }
    
    RenderTileMap(registry, camera);
    systems.renderSystem(registry, RENDERER, camera);
    
    if (GLOBALS::CURRENT_STATE == GLOBALS::STATE_PAUSED)
    {
        UI.renderSystemPause(RENDERER);
    }
    
    if (!SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 0))
    {
        SDL_Log("SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
    }
    
    
    //map -> drawMap();
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    //ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

//    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport() -> ID);
   //ImGui::ShowDemoWindow();
    if (IMGUI)
    {
        ImGui::Begin("Debug Functions");
        
        if (ImGui::Button("Rate of Fire"))
        {
            playerSystem.upgradeROF(1.1f);
        }
        
        if (ImGui::Button("Speed"))
        {
            playerSystem.upgradeSpeed(1.1f, registry);
        }
        
        if (ImGui::Button("close"))
        {
            IMGUI = false;
        }
        ImGui::End();
        
    }
    
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), RENDERER);
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

void Game::initIMGUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    
    // Setup scaling
    //float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    ImGuiStyle &style = ImGui::GetStyle();
    //style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a
                       // solution for dynamic style scaling, changing this
                       // requires resetting Style + calling this again)
                       // makes this unnecessary. We leave both here for
                       // documentation purpose)
    //style.FontScaleDpi = main_scale;

    style.FontSizeBase = 15.0f;
    style.Colors[ImGuiCol_Button] = ImVec4(235.0f/255.0f, 82.0f/255.0f, 30.0f/255.0f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(200.0f/255.0f, 82.0f/255.0f, 30.0f/255.0f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(235.0f/255.0f, 82.0f/255.0f, 30.0f/255.0f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(235.0f/255.0f, 82.0f/255.0f, 30.0f/255.0f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(235.0f/255.0f, 82.0f/255.0f, 30.0f/255.0f, 1.0f);
    style.Colors[ImGuiCol_Tab] = ImVec4(235.0f/255.0f, 82.0f/255.0f, 30.0f/255.0f, 1.0f);
    style.Colors[ImGuiCol_TabSelected] = ImVec4(235.0f/255.0f, 82.0f/255.0f, 30.0f/255.0f, 1.0f);
    style.Colors[ImGuiCol_TabDimmed] = ImVec4(200.0f/255.0f, 82.0f/255.0f, 30.0f/255.0f, 1.0f);

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);

    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    //io.Fonts->AddFontDefault();
//235, 82, 30
    ImGui_ImplSDL3_InitForSDLRenderer(WINDOW, RENDERER);
    ImGui_ImplSDLRenderer3_Init(RENDERER);
}

