
#include "SpriteHandler.h"


SpriteHandler *SPRITE_HANDLER;


// TODO: Implement associative array like a sane person!
SpriteHandler::SpriteHandler() {
    // Read all sprite sheets

    // Box
    {
        SpriteState **states = (SpriteState **)malloc(sizeof(SpriteState *) * 2);
        {
            // Default state
            const char *str = "default";
            Point *frame = new Point { 0, 0 };
            SpriteState *state = new SpriteState(str, frame);
            states[0] = state;
        }
        {
            // Shining (animated) state
            const char *str = "shining";
            int frame_time = 200;
            int frames_c = 5;
            Point **frames_v = (Point **)malloc(sizeof(Point *) * frames_c);
            frames_v[0] = new Point { 0, 1 };
            frames_v[1] = new Point { 1, 1 };
            frames_v[2] = new Point { 2, 1 };
            frames_v[3] = new Point { 3, 1 };
            frames_v[4] = new Point { 4, 1 };
            SpriteState *state = new SpriteState(str, frames_c, frames_v, frame_time);
            states[1] = state;
        }
        add("box", new SpriteSheet("sprites/box.png", 2, states));
    }

    // Dirt
    add("dirt", new SpriteSheet("sprites/dirt.png"));

    // Grass
    add("grass", new SpriteSheet("sprites/grass.png"));

    // Sand
    add("sand", new SpriteSheet("sprites/sand.png"));

    // Water
    add("water", new SpriteSheet("sprites/water.png"));

    // Player
    {
        SpriteState **states = (SpriteState **)malloc(sizeof(SpriteState *) * 4);
        {
            const char *str = "facing_east";
            Point *frame = new Point { 0, 0 };
            SpriteState *state = new SpriteState(str, frame);
            states[0] = state;
        }
        {
            const char *str = "facing_west";
            Point *frame = new Point { 0, 0 };
            int *rotate = (int *)malloc(sizeof(int) * 1);
            bool *mirror_h = (bool *)malloc(sizeof(bool) * 1);
            bool *mirror_v = (bool *)malloc(sizeof(bool) * 1);
            rotate[0] = 0;
            mirror_h[0] = true;
            mirror_v[0] = false;

            SpriteState *state = new SpriteState(str, frame, rotate, mirror_h, mirror_v);
            states[1] = state;
        }
        {
            const char *str = "facing_north";
            Point *frame = new Point { 0, 2 };
            SpriteState *state = new SpriteState(str, frame);
            states[2] = state;
        }
        {
            const char *str = "facing_south";
            Point *frame = new Point { 0, 1 };
            SpriteState *state = new SpriteState(str, frame);
            states[3] = state;
        }
        add("player", new SpriteSheet("sprites/player.png", 4, states));
    }

    // Target
    {
        SpriteState **states = (SpriteState **)malloc(sizeof(SpriteState *) * 1);
        {
            const char *str = "default";
            int frame_time = 200;
            int frames_c = 5;
            Point **frames_v = (Point **)malloc(sizeof(Point *) * frames_c);
            frames_v[0] = new Point { 0, 0 };
            frames_v[1] = new Point { 1, 0 };
            frames_v[2] = new Point { 2, 0 };
            frames_v[3] = new Point { 3, 0 };
            frames_v[4] = new Point { 4, 0 };
            SpriteState *state = new SpriteState(str, frames_c, frames_v, frame_time);
            states[0] = state;
        }
        add("target", new SpriteSheet("sprites/target.png", 1, states));
    }

    // Wall
    add("wall", new SpriteSheet("sprites/wall.png"));

    // Woodplanks
    add("woodplanks", new SpriteSheet("sprites/woodplanks.png"));

    // Tree
    add("tree", new SpriteSheet("sprites/tree.png"));
}

SpriteHandler::~SpriteHandler() {
    // Destroy all sprite sheets
    for (unsigned long i = 0; i < values.size(); i++) {
        delete values.at(i);
    }
}

SpriteSheet *SpriteHandler::get(std::string key) {
    // Search key
    for (unsigned long i = 0; i < keys.size(); i++) {
        if (keys.at(i) == key) {
            return values.at(i);
        }
    }

    // Key has not been found
    fprintf(stderr, "Could not find sprite for key '%s'!\n", key.c_str());
    return NULL;
}

void SpriteHandler::add(std::string key, SpriteSheet *sprite) {
    keys.push_back(key);
    values.push_back(sprite);
}

void deinitialize_images() {
    delete SPRITE_HANDLER;
    SPRITE_HANDLER = NULL;
}

bool initialize_images() {
    SPRITE_HANDLER = new SpriteHandler();
    return (SPRITE_HANDLER == NULL) ? false : true;
}

