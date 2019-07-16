#pragma once
#ifndef GAME_CHARACTER_H
#define GAME_CHARACTER_H

#include "DynamicObject.h"
#include "Direction.h"

class GameCharacter : public DynamicObject {
    protected:
        enum Direction direction;
    public:
        GameCharacter(int x, int y, SpriteSheet *sprites);
        GameCharacter(int x, int y, SpriteSheet *sprites, Direction direction);
        ~GameCharacter();
};

#endif
