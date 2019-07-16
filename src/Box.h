#pragma once
#ifndef BOX_H
#define BOX_H

#include "DynamicObject.h"

class Box : public DynamicObject {
    private:
        bool is_on_target();
    public:
        Box(int x, int y);
        virtual ~Box();

        virtual bool is_pushable(int dx, int dy);
        virtual bool push(int dx, int dy);

        virtual bool render();
};

#endif
