
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
            Point **frames_v = (Point **)malloc(sizeof(Point *) * 5);
            frames_v[0] = new Point { 0, 1 };
            frames_v[1] = new Point { 1, 1 };
            frames_v[2] = new Point { 2, 1 };
            frames_v[3] = new Point { 3, 1 };
            frames_v[4] = new Point { 4, 1 };
            SpriteState *state = new SpriteState(str, frame_time, frames_c, frames_v);
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
        SpriteState **states = (SpriteState **)malloc(sizeof(SpriteState *) * 3);
        {
            const char *str = "default";
            Point *frame = new Point { 0, 0 };
            SpriteState *state = new SpriteState(str, frame);
            states[0] = state;
        }
        {
            const char *str = "facing_down";
            Point *frame = new Point { 0, 1 };
            SpriteState *state = new SpriteState(str, frame);
            states[1] = state;
        }
        {
            const char *str = "facing_north";
            Point *frame = new Point { 0, 2 };
            SpriteState *state = new SpriteState(str, frame);
            states[2] = state;
        }

        add("player", new SpriteSheet("sprites/player.png", 3, states));
    }

    // Target
    {
        //Animation *animation = new Animation(200, 5, true);
        //add("target", new SpriteSheet("sprites/target.png", animation));
        add("target", new SpriteSheet("sprites/target.png"));
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

#ifdef DEBUG_SIZE
long SpriteHandler::recursive_size() {
    long size = 0;

    // Keys
    for (size_t i = 0; i < keys.size(); i++) {
        size += sizeof(keys[i]);
    }

    // Values
    for (size_t i = 0; i < values.size(); i++) {
        long value_size = 0;

        value_size += sizeof(values[i]);
        value_size += values[i]->recursive_size();

        size += value_size;
    }

    printf("Size of SpriteHandler is %ld\n", size);
    return size;
}
#endif


void deinitialize_images() {
    delete SPRITE_HANDLER;
    SPRITE_HANDLER = NULL;
}

bool initialize_images() {
    SPRITE_HANDLER = new SpriteHandler();

    #ifdef DEBUG_SIZE
    SPRITE_HANDLER->recursive_size();
    #endif

    return (SPRITE_HANDLER == NULL) ? false : true;
}

