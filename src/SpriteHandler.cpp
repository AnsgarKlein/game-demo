
#include "SpriteHandler.h"

SpriteHandler *SPRITE_HANDLER;

SpriteHandler::SpriteHandler() {
    const std::string all_sprites[] = {
        "sprites/box.json",
        "sprites/dirt.json",
        "sprites/grass.json",
        "sprites/sand.json",
        "sprites/water.json",
        "sprites/player.json",
        "sprites/target.json",
        "sprites/wall.json",
        "sprites/woodplanks.json",
        "sprites/tree.json",
    };

    // Read all sprite sheets
    for (std::string str : all_sprites) {
        SpriteSheet *sheet = SpriteSheet_from_file(str);
        if (sheet != NULL) {
            add(sheet->get_id(), sheet);
        }
    }
}

SpriteHandler::~SpriteHandler() {
    // Destroy all sprite sheets
    for (auto pair : sprites) {
        SpriteSheet *sprite = pair.second;
        delete sprite;

        // Dont delete the string because it is a pointer
        // to the id in the SpriteSheet and will get freed
        // with it.
    }
}

SpriteSheet *SpriteHandler::get(std::string key) {
    // TODO: Implement associative array like a sane person!

    // Search key
    for (auto pair : sprites) {
        std::string *str = pair.first;
        SpriteSheet *sprite = pair.second;

        if (key == *str) {
            return sprite;
        }
    }

    // Key has not been found
    return NULL;
}

void SpriteHandler::add(std::string *key, SpriteSheet *sprite) {
    std::pair<std::string *, SpriteSheet *> pair(key, sprite);
    sprites.insert(pair);
}

void deinitialize_images() {
    delete SPRITE_HANDLER;
    SPRITE_HANDLER = NULL;
}

bool initialize_images() {
    SPRITE_HANDLER = new SpriteHandler();
    return (SPRITE_HANDLER == NULL) ? false : true;
}

