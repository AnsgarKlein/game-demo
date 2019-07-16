#pragma once
#ifndef FREE_H
#define FREE_H

#include "StaticObject.h"

class Free : public StaticObject {
    public:
        Free(int x, int y);
        ~Free();
};

#endif
