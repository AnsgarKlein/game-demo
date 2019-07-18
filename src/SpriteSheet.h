#pragma once
#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include <SDL.h>

#include <string>
#include "SpriteState.h"

class SpriteSheet {
    private:
        SDL_Texture *texture;
        const char *str;

        int states_c;
        SpriteState **states_v;

        SpriteState *get_state(const char *state_str);
    public:
        SpriteSheet(std::string *path);
        SpriteSheet(const char *path);
        SpriteSheet(std::string *path, int states_c, SpriteState **states_v);
        SpriteSheet(const char *path, int states_c, SpriteState **states_v);
        virtual ~SpriteSheet();

        char *get_str();

        SDL_Texture *get_texture();

        virtual bool render(int x, int y);
        virtual bool render(int x, int y, const char *state_str);
        virtual bool renderINT(unsigned int x, unsigned int y, SpriteState *state);

};

#endif
