#pragma once
#ifndef TREE_H
#define TREE_H

#include "DynamicObject.h"

class Tree : public DynamicObject {
    public:
        Tree(int x, int y);
        ~Tree();
};

#endif
