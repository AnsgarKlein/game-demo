
#include "Wall.h"

#include "SpriteHandler.h"

Wall::Wall(int x, int y) : StaticObject(x, y, true, SPRITE_HANDLER->get("wall")) {
    //
}

Wall::~Wall() {
    //
}
