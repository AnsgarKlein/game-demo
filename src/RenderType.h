#pragma once
#ifndef RENDER_TYPE_H
#define RENDER_TYPE_H

// TODO: Check if some types of rendering types might not be neccessary
// (Maybe only states for animated and not-animated ?)

enum RenderType {
    RENDER_STATIC,
    RENDER_ANIMATED,
    RENDER_PERMUTATED,
    // TODO: Use different types of permutation to specify based on what to permutate
    RENDER_VARIATED,
};

#endif
