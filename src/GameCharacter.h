#pragma once
#ifndef GAME_CHARACTER_H
#define GAME_CHARACTER_H

#include "DynamicObject.h"

enum Direction {
    FACING_RIGHT,
    FACING_LEFT,
    FACING_UP,
    FACING_DOWN
};

class GameCharacter : public DynamicObject {
    protected:
        enum Direction facing;
    public:
        GameCharacter(int x, int y, SpriteSheet *sprites);
        GameCharacter(int x, int y, SpriteSheet *sprites, Direction direction);
        ~GameCharacter();
};

#endif
