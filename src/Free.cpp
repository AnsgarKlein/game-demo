
#include "Free.h"

#include "SpriteHandler.h"

Free::Free(int x, int y) : StaticObject(x, y, false, SPRITE_HANDLER->get("dirt")) {
    //
}

Free::~Free() {
    //
}
