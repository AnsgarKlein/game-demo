#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <stdbool.h>
#include <stdint.h>

#include "vector"

#include <SDL.h>

#include "Player.h"
#include "StaticObject.h"
#include "DynamicObject.h"

class Level {
    private:
        StaticObject **background;
        std::vector<DynamicObject *> *game_objects;
        uint8_t width;
        uint8_t height;
        Player *player;
    public:
        Level(Player *player, uint8_t width, uint8_t height, StaticObject **background, std::vector<DynamicObject *> *game_objects);
        ~Level();

        uint8_t get_width() { return width; };
        uint8_t get_height() { return height; };

        bool is_solid(int x, int y);
        std::vector<DynamicObject *> *get_objects(int x, int y);

        Player get_player();
        bool render();
};

//Level* Level_from_file(const char *filepath);
Level* Level_from_buffer(const char *buf, long len);

extern const char *DEFAULT_LEVEL;

extern const char BOARD_FREE;
extern const char BOARD_BOX;
extern const char BOARD_TARGET;
extern const char BOARD_BOXTARGET;
extern const char BOARD_WALL;

extern Level *CURRENT_LEVEL;

//bool set_level_from_file(const char *filepath);

bool set_level_from_buffer(const char *buf, long len);

Level *Level_from_file(std::string path);

Level *Level_from_json(JsonObject *obj);

#endif
