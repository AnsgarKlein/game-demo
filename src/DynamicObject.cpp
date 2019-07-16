
#include "DynamicObject.h"

DynamicObject::DynamicObject(int x, int y, SpriteSheet *sprites) : GameObject(x, y, sprites) {
    this->z = 0;
}

DynamicObject::~DynamicObject() {
    //
}

int DynamicObject::get_z() {
    return z;
}

void DynamicObject::interact(DynamicObject *other) {
    if (other == NULL) {
        return;
    }
}

bool DynamicObject::is_pushable(int dx, int dy) {
    if (dx == dy) { }
    return false;
}

bool DynamicObject::push(int dx, int dy) {
    if (dx == dy) { }
    return false;
}
