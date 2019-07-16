#pragma once
#ifndef DYNAMIC_OBJECT_H
#define DYNAMIC_OBJECT_H

#include "GameObject.h"

class DynamicObject : public GameObject {
    protected:
        int z;
    public:
        DynamicObject(int x, int y, SpriteSheet *sprites);
        virtual ~DynamicObject();

        virtual int get_z();

        void move_to(int x, int y);
        void move(int dx, int dy);

        virtual void interact(DynamicObject *other);

        virtual bool is_pushable(int dx, int dy);
        virtual bool push(int dx, int dy);
};

#endif
