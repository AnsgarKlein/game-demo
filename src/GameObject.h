#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "SpriteSheet.h"
#include "Direction.h"

class GameObject {
    protected:
        int x;
        int y;
        SpriteSheet *sprites;

        static bool render_animated(const SDL_Texture *text, int x, int y, uint16_t frame_time, uint8_t frames, bool overlay);
        static bool render_with_direction(const SDL_Texture *text, int x, int y, enum Direction direction);
        static bool render_with_variation(const SDL_Texture *text, int x, int y, int variations, int commonness);
        static bool render_permutated(const SDL_Texture *text, int x, int y);
        static bool render_static(const SDL_Texture *text, int x, int y);
    public:
        GameObject(int x, int y, SpriteSheet *sprites);
        virtual ~GameObject();

        int get_x();
        int get_y();

        virtual char *get_str();

        virtual bool render();
};

#endif
