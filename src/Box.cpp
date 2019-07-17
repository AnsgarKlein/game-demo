
#include "Box.h"

#include "SpriteHandler.h"
#include "Target.h"
#include "Level.h"
#include "View.h"

using namespace std;

Box::Box(int x, int y) : DynamicObject(x, y, SPRITE_HANDLER->get("box")) {
    this->z = 2;
}

Box::~Box() {
    //
}

bool Box::is_on_target() {
    // Check if there is a target under this box

    // Check every object at this position
    std::vector<DynamicObject *> *objects = CURRENT_LEVEL->get_objects(x, y);
    for (size_t i = 0; i < objects->size(); i++) {
        DynamicObject *obj = (*objects)[i];

        // Check if object is a target
        Target* t = dynamic_cast<Target *> (obj);
        if (t != NULL) {
            delete objects;
            return true;
        }
    }

    delete objects;
    return false;
}

bool Box::is_pushable(int dx, int dy) {
    int new_x = x + dx;
    int new_y = y + dy;

    // Box can be pushed if new position is not solid
    return !CURRENT_LEVEL->is_solid(new_x, new_y);
}

bool Box::push(int dx, int dy) {
    if (!is_pushable(dx, dy)) {
        return false;
    }

    // Box is pushed and changes position
    x += dx;
    y += dy;

    // Returning true means position of box is now free and
    // pushing object (player?) can now take this position.
    return true;
}

bool Box::render() {
    // Box gets only animated if it is on top of a target
    if (is_on_target()) {
        return sprites->render(x * GRID_SIZE, y * GRID_SIZE, "shining");
    } else {
        return sprites->render(x * GRID_SIZE, y * GRID_SIZE);
    }
}
