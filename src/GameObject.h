#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "SpriteSheet.h"

class GameObject {
    protected:
        int x;
        int y;
        SpriteSheet *sprites;
    public:
        GameObject(int x, int y, SpriteSheet *sprites);
        virtual ~GameObject();

        int get_x();
        int get_y();

        virtual bool render();
};

#endif
