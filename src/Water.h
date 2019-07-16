#pragma once
#ifndef WATER_H
#define WATER_H

#include "StaticObject.h"

class Water : public StaticObject {
    public:
        Water(int x, int y);
        ~Water();
};

#endif
