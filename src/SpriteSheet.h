#pragma once
#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include <SDL.h>

#include <string>

#include "SpriteState.h"
#include "JsonParser.h"

class SpriteSheet {
    private:
        SDL_Texture *texture;
        std::string *id;

        int states_c;
        SpriteState **states_v;

        SpriteState *get_state(std::string *state_str);
    public:
        SpriteSheet(std::string *id, std::string *path);
        SpriteSheet(std::string *id, std::string *path, int states_c, SpriteState *states_v[]);
        virtual ~SpriteSheet();

        std::string *get_id();

        SDL_Texture *get_texture();

        virtual bool render(unsigned int x, unsigned int y);
        virtual bool render(unsigned int x, unsigned int y, const char *state_str);
        virtual bool renderINT(unsigned int x, unsigned int y, SpriteState *state);

};

SpriteSheet *SpriteSheet_from_file(std::string path);
SpriteSheet *SpriteSheet_from_json(JsonObject *obj);

#endif
