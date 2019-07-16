
#include "Player.h"

#include "Level.h"
#include "SpriteHandler.h"
#include "View.h"


Player *PLAYER = NULL;


Player::Player(int x, int y, SpriteSheet *sprites) : GameCharacter(x, y, sprites) {
    // Player gets initialized as facing to the right
    this->player_facing = FACING_RIGHT;
}

Player::Player(int x, int y, std::string sprite_str) : GameCharacter(x, y, SPRITE_HANDLER->get(sprite_str)) {
    // Player gets initialized as facing to the right
    this->player_facing = FACING_RIGHT;
}

Player::~Player() {
    //
}

/**
 * @brief Checks if and how Player can move in a given direction.
 *
 * Gets a direction and checks if Player can move in the given direction.
 * If there is a box in the given direction it is checked whether this
 * box can move.
 *
 * @param dx The value to move Sokoban in x direction
 * @param dy The value to move Sokoban in y direction
 *
 * @return The type of move that Sokoban can (can't) do in the
 *         given direction.
 */
MoveType Player::check_move(int dx, int dy) {
    // If map is solid we cannot move
    if (CURRENT_LEVEL->is_solid(x + dx, y + dy)) {
        //printf("world is solid at target position -> move impossible\n");
        return MOVE_TYPE_IMPOSSIBLE;
    }

    // Map is not solid but maybe there is another object at the position
    std::vector<DynamicObject *> *objects = CURRENT_LEVEL->get_objects(x + dx, y + dy);

    // If there is no other object, we can move alone
    if (objects->size() == 0) {
        //printf("there is no other object -> move alone\n");
        return MOVE_TYPE_ALONE;
    }

    for (size_t i = 0; i < objects->size(); i++) {
        DynamicObject *obj = (*objects)[i];

        //printf("object at target position: '%s'\n", obj->get_str());

        // If the other object can not be pushed we cannot move
        if (!obj->is_pushable(dx, dy)) {
            //printf("other object is not pushable -> move impossible\n");
            return MOVE_TYPE_IMPOSSIBLE;
        }

    }

    // When all objects on target position can be pushed we can move with box
    //printf("other object is pushable -> move with box\n");
    return MOVE_TYPE_WITHBOX;
}

/**
 * @brief Shows Sokoban at the current position.
 *
 * Draws Sokoban at the current position onto the screen.
**/
bool Player::render() {
    //return render_with_direction(this->sprites->get_texture(), x, y, player_facing);
    return this->sprites->render(x, y);
}

/**
 * @brief Moves Sokoban in a given direction if possible.
 *
 * Moves Sokoban in a given direction if this move is possible.
 * If this move requires the move of a box this box is moved too.
 *
 * @param dx The value to Sokoban in x direction
 * @param dy The value to Sokoban in y direction
 */
void Player::move(int dx, int dy) {
    // Set direction Sokoban is facing
    if (dx > 0) {
        player_facing = FACING_RIGHT;
    } else if (dx < 0) {
        player_facing = FACING_LEFT;
    } else if (dy > 0) {
        player_facing = FACING_DOWN;
    } else if (dy < 0) {
        player_facing = FACING_UP;
    }

    // Check which type of move to do
    enum MoveType type = check_move(dx, dy);

    // TODO: Check this enum with switch case instead of ifs

    // Check if move is possible at all
    if (type == MOVE_TYPE_IMPOSSIBLE) {
        return;
    }

    if (type == MOVE_TYPE_ALONE) {
        x += dx;
        y += dy;
        update_camera(x, y);
        return;
    }

    // Move the box first if necessary
    if (type == MOVE_TYPE_WITHBOX) {
        std::vector<DynamicObject *> *objects = CURRENT_LEVEL->get_objects(x + dx, y + dy);
        for (size_t i = 0; i < objects->size(); i++) {
            DynamicObject *obj = (*objects)[i];
            if (!obj->push(dx, dy)) {
                delete objects;
                return;
            }
        }
        delete objects;

        // If pushing has been successful set new sokoban coordinates
        x += dx;
        y += dy;
        update_camera(x, y);
        return;
    }
}

