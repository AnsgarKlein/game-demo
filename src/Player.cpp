
#include "Player.h"

#include "Level.h"
#include "SpriteHandler.h"
#include "View.h"


Player *PLAYER = NULL;


Player::Player(int x, int y, SpriteSheet *sprites) : GameCharacter(x, y, sprites) {
    //
}

Player::Player(int x, int y, std::string sprite_str) : GameCharacter(x, y, SPRITE_HANDLER->get(sprite_str)) {
    //
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
        return MOVE_TYPE_IMPOSSIBLE;
    }

    // Map is not solid but maybe there is another object at the position
    std::vector<DynamicObject *> *objects = CURRENT_LEVEL->get_objects(x + dx, y + dy);

    // If there is no other object, we can move alone
    if (objects->size() == 0) {
        return MOVE_TYPE_ALONE;
    }

    for (size_t i = 0; i < objects->size(); i++) {
        DynamicObject *obj = (*objects)[i];

        // If the other object can not be pushed we cannot move
        if (!obj->is_pushable(dx, dy)) {
            return MOVE_TYPE_IMPOSSIBLE;
        }

    }

    // When all objects on target position can be pushed we can move with box
    return MOVE_TYPE_WITHBOX;
}

/**
 * @brief Shows Sokoban at the current position.
 *
 * Draws Sokoban at the current position onto the screen.
**/
bool Player::render() {
    switch (facing) {
        case FACING_UP:
            return this->sprites->render(x * GRID_SIZE, y * GRID_SIZE, "facing_north");
            break;
        case FACING_DOWN:
            return this->sprites->render(x * GRID_SIZE, y * GRID_SIZE, "facing_south");
            break;
        case FACING_RIGHT:
            return this->sprites->render(x * GRID_SIZE, y * GRID_SIZE, "facing_east");
            break;
        case FACING_LEFT:
            return this->sprites->render(x * GRID_SIZE, y * GRID_SIZE, "facing_west");
            break;
    }
}

void Player::set_coordinates(int x, int y) {
    this->x = x;
    this->y = y;

    // Update the camera position after player position changed
    update_camera(x * GRID_SIZE, y * GRID_SIZE);
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
        facing = FACING_RIGHT;
    } else if (dx < 0) {
        facing = FACING_LEFT;
    } else if (dy > 0) {
        facing = FACING_DOWN;
    } else if (dy < 0) {
        facing = FACING_UP;
    }

    // Check which type of move to do and move accordingly
    switch(check_move(dx, dy)) {
        case MOVE_TYPE_IMPOSSIBLE:
            break;
        case MOVE_TYPE_ALONE:
            set_coordinates(x + dx, y + dy);
            break;
        case MOVE_TYPE_WITHBOX:
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
            set_coordinates(x + dx, y + dy);
            break;
    }
}

