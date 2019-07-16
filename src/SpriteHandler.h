#pragma once
#ifndef SPRITE_HANDLER_H
#define SPRITE_HANDLER_H

#include <vector>
#include <string>

#include "SpriteSheet.h"

// TODO: Implement associative array like a sane person!

class SpriteHandler {
    private:
        std::vector<std::string> keys;
        std::vector<SpriteSheet *> values;
    public:
        SpriteHandler();
        ~SpriteHandler();

        SpriteSheet *get(std::string key);
        void add(std::string key, SpriteSheet *sprite);

        #ifdef DEBUG_SIZE
        long recursive_size();
        #endif

};

extern SpriteHandler *SPRITE_HANDLER;

void deinitialize_images();

bool initialize_images();

#endif
