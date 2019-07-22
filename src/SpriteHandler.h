#pragma once
#ifndef SPRITE_HANDLER_H
#define SPRITE_HANDLER_H

#include <vector>
#include <string>

#include "SpriteSheet.h"

class SpriteHandler {
    private:
        std::unordered_map<std::string *, SpriteSheet *> sprites;
    public:
        SpriteHandler();
        ~SpriteHandler();

        SpriteSheet *get(std::string key);
        void add(std::string *key, SpriteSheet *sprite);
};

extern SpriteHandler *SPRITE_HANDLER;

void deinitialize_images();

bool initialize_images();

#endif
