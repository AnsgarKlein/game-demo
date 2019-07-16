#pragma once
#ifndef STATIC_OBJECT_H
#define STATIC_OBJECT_H

#include "GameObject.h"
#include "SpriteSheet.h"

class StaticObject : public GameObject {
    protected:
        bool solid;
    public:
        StaticObject(int x, int y, bool solid, SpriteSheet *sprites);
        virtual ~StaticObject();

        virtual bool is_solid();
};

#endif
