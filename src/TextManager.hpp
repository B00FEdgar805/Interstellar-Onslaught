//
//  TextManager.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 7/12/26.
//

#ifndef TextManager_hpp
#define TextManager_hpp

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <string>
#include <unordered_map>

class TextManager
{
private:
    static TTF_TextEngine* TEXT_ENGINE;
    static bool TTF_STARTED;
    static std::unordered_map<std::string, TTF_Font*> FONTS;
    static std::unordered_map<std::string, TTF_Text*> LABELS;
    //SDL_Color WHITE = SDL_Color{255, 255, 255, 255};
    
public:    
    static bool init(SDL_Renderer* renderer);
    static bool loadFont(const std::string& fontId, const char* path, float pointSize);
    static bool createLabel(const std::string& labelId, const std::string& fontId, const std::string& text, SDL_Color color);
    static bool setLabelText(const std::string& labelId, const std::string& text);
    static bool setLabelColor(const std::string& labelId, SDL_Color color);
    static bool drawLabel(const std::string& labelId, float x, float y);
    static bool getLabelSize(const std::string& labelId, int* w, int* h);
    static void destroyLabel(const std::string& labelId);
    static void shutdown();
    
private:
    static TTF_Font* getFont(const std::string& fontId)
    {
        auto it = FONTS.find(fontId);
        if (it == FONTS.end())
        {
            return nullptr;
        }

        return it -> second;
    }

    static TTF_Text* getLabel(const std::string& labelId)
    {
        auto it = LABELS.find(labelId);
        if (it == LABELS.end())
        {
            return nullptr;
        }

        return it -> second;
    }

};

#endif /* TextManager_hpp */
