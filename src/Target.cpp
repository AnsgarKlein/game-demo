
#include "Target.h"

#include "SpriteHandler.h"
#include "View.h"

Target::Target(int x, int y) : DynamicObject(x, y, SPRITE_HANDLER->get("target")) {
    //
}

Target::~Target() {
    //
}

bool Target::is_pushable(int dx, int dy) {
    if (dx == dy) { }

    // Can always walk against target
    return true;
}

bool Target::push(int dx, int dy) {
    if (dx == dy) { }

    // Target position is always free after pushing
    return true;
}

