
#include "GameCharacter.h"

GameCharacter::GameCharacter(int x, int y, SpriteSheet *sprites) : DynamicObject(x, y, sprites) {
    // Character gets initialized as facing to the right
    this->facing = FACING_RIGHT;
}

GameCharacter::GameCharacter(int x, int y, SpriteSheet *sprites, Direction direction) : DynamicObject(x, y, sprites) {
    // Character gets initialized as facing to the right
    this->facing = direction;
}

GameCharacter::~GameCharacter() {
    //
}
