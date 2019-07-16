
#include "Water.h"

#include "SpriteHandler.h"

Water::Water(int x, int y) : StaticObject(x, y, true, SPRITE_HANDLER->get("water")) {
    //
}

Water::~Water() {
    //
}
