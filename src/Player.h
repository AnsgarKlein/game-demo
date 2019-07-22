#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "GameCharacter.h"
#include "SpriteSheet.h"

/**
 * @brief Types for moves of Sokoban
 */
enum MoveType {
    /**
     * Move is not possible.
     */
    MOVE_TYPE_IMPOSSIBLE,

    /**
     * Move is a normal possible move.
     */
    MOVE_TYPE_ALONE,

    /**
     * Move is possible and is a move which also moves a Box.
     */
    MOVE_TYPE_WITHBOX
};

class Player : public GameCharacter {
    private:
        enum MoveType check_move(int dx, int dy);
        void move_box(int dx, int dy);
    public:
        Player(int x, int y, SpriteSheet *sprites);
        Player(int x, int y, std::string sprite_str);
        virtual ~Player();

        void set_coordinates(int x, int y);
        void move(int dx, int dy);
        bool render();
};

extern Player *PLAYER;

#endif
