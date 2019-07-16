
#include "StaticObject.h"
#include "View.h"

StaticObject::StaticObject(int x, int y, bool solid, SpriteSheet *sprites) : GameObject(x, y, sprites) {
    this->solid = solid;
}

StaticObject::~StaticObject() {
    //
}

bool StaticObject::is_solid() {
    return this->solid;
}

