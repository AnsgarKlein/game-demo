#pragma once
#ifndef TARGET_H
#define TARGET_H

#include "DynamicObject.h"

class Target : public DynamicObject {
    public:
        Target(int x, int y);
        virtual ~Target();

        virtual bool is_pushable(int dx, int dy);
        virtual bool push(int dx, int dy);

        virtual bool render();
};

#endif
