//
//  TextManager.cpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 7/12/26.
//

#include "TextManager.hpp"

TTF_TextEngine* TextManager::TEXT_ENGINE;
bool TextManager::TTF_STARTED = false;
std::unordered_map<std::string, TTF_Font*> TextManager::FONTS;
std::unordered_map<std::string, TTF_Text*> TextManager::LABELS;

bool TextManager::init(SDL_Renderer *renderer)
{
    //TTF_STARTED = false;
    if (!renderer)
    {
        SDL_Log("TextManager::init failed: renderer was null");
        return false;
    }

    if (!TTF_Init())
    {
        SDL_Log("TTF_Init failed: %s", SDL_GetError());
        return false;
    }

    TTF_STARTED = true;

    TEXT_ENGINE = TTF_CreateRendererTextEngine(renderer);
    if (!TEXT_ENGINE)
    {
        SDL_Log("TTF_CreateRendererTextEngine failed: %s", SDL_GetError());
        shutdown();
        return false;
    }
    
    //SDL_Log("Init");

    return true;
}

bool TextManager::loadFont(const std::string &fontId, const char *path, float pointSize)
{
    if (FONTS.find(fontId) != FONTS.end())
    {
        SDL_Log("Font id already exists: %s", fontId.c_str());
        return false;
    }

    TTF_Font* font = TTF_OpenFont(path, pointSize);
    if (!font)
    {
        SDL_Log("TTF_OpenFont failed for '%s': %s", path, SDL_GetError());
        return false;
    }

    FONTS[fontId] = font;
    return true;
}

bool TextManager::createLabel(const std::string &labelId, const std::string &fontId, const std::string &text, SDL_Color color)
{
    if (!TEXT_ENGINE)
    {
        SDL_Log("TextManager has not been initialized");
        return false;
    }

    TTF_Font* font = getFont(fontId);
    if (!font) {
        SDL_Log("Font not found: %s", fontId.c_str());
        return false;
    }

    // Replace old label if it already exists.
    destroyLabel(labelId);

    // length 0 means null-terminated string in SDL3_ttf.
    TTF_Text* label = TTF_CreateText(TEXT_ENGINE, font, text.c_str(), 0);
    if (!label)
    {
        SDL_Log("TTF_CreateText failed: %s", SDL_GetError());
        return false;
    }

    if (!TTF_SetTextColor(label, color.r, color.g, color.b, color.a))
    {
        SDL_Log("TTF_SetTextColor failed: %s", SDL_GetError());
        TTF_DestroyText(label);
        return false;
    }

    LABELS[labelId] = label;
    return true;
}

bool TextManager::setLabelText(const std::string &labelId, const std::string &text)
{
    TTF_Text* label = getLabel(labelId);
    if (!label)
    {
        SDL_Log("Label not found: %s", labelId.c_str());
        return false;
    }
    
    if (!TTF_SetTextString(label, text.c_str(), 0))
    {
        SDL_Log("TTF_SetTextString failed: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool TextManager::setLabelColor(const std::string &labelId, SDL_Color color)
{
    TTF_Text* label = getLabel(labelId);
    if (!label)
    {
        SDL_Log("Label not found: %s", labelId.c_str());
        return false;
    }

    if (!TTF_SetTextColor(label, color.r, color.g, color.b, color.a))
    {
        SDL_Log("TTF_SetTextColor failed: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool TextManager::drawLabel(const std::string &labelId, float x, float y)
{
    TTF_Text* label = getLabel(labelId);
    if (!label)
    {
        SDL_Log("Could not find %s" , labelId.c_str());
        return false;
    }

    return TTF_DrawRendererText(label, x, y);
}

bool TextManager::getLabelSize(const std::string &labelId, int *w, int *h)
{
    TTF_Text* label = getLabel(labelId);
    if (!label)
    {
        return false;
    }

    return TTF_GetTextSize(label, w, h);
}

void TextManager::destroyLabel(const std::string &labelId)
{
    auto it = LABELS.find(labelId);
    if (it == LABELS.end())
    {
        return;
    }

    TTF_DestroyText(it -> second);
    LABELS.erase(it);
}

void TextManager::shutdown()
{
    // Text objects should be destroyed before destroying the text engine.
    for (auto& pair : LABELS)
    {
        TTF_DestroyText(pair.second);
    }
    LABELS.clear();

    if (TEXT_ENGINE)
    {
        TTF_DestroyRendererTextEngine(TEXT_ENGINE);
        TEXT_ENGINE = nullptr;
    }

    for (auto& pair : FONTS)
    {
        TTF_CloseFont(pair.second);
    }
    FONTS.clear();

    if (TTF_STARTED)
    {
        TTF_Quit();
        TTF_STARTED = false;
    }
}

