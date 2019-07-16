
#include "Grass.h"

#include "SpriteHandler.h"

Grass::Grass(int x, int y) : StaticObject(x, y, false, SPRITE_HANDLER->get("grass")) {
    //
}

Grass::~Grass() {
    //
}
