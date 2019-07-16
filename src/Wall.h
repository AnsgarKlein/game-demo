#pragma once
#ifndef WALL_H
#define WALL_H

#include "StaticObject.h"

class Wall : public StaticObject {
    public:
        Wall(int x, int y);
        ~Wall();
};

#endif
