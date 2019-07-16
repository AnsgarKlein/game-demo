#pragma once
#ifndef SAND_H
#define SAND_H

#include "StaticObject.h"

class Sand : public StaticObject {
    public:
        Sand(int x, int y);
        ~Sand();
};

#endif
