#pragma once
#ifndef GRASS_H
#define GRASS_H

#include "StaticObject.h"

class Grass : public StaticObject {
    public:
        Grass(int x, int y);
        ~Grass();
};

#endif
