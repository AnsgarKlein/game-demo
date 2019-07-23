
#include "Level.h"

#include "SpriteHandler.h"
#include "View.h"

#include "Wall.h"
#include "Woodplank.h"
#include "Tree.h"
#include "Target.h"

#include "Free.h"
#include "Grass.h"
#include "Sand.h"
#include "Water.h"
#include "Box.h"

const char BOARD_FREE      = ' ';
const char BOARD_BOX       = '$';
const char BOARD_TARGET    = '.';
const char BOARD_BOXTARGET = '*';
const char BOARD_WALL      = '#';

Level *CURRENT_LEVEL = NULL;

const char *DEFAULT_LEVEL =
    "Sokoban\n"
    "####################################wwwwwwwwwww\n"
    "#       @     GTTTTTTTGGGwwwwwwwwwwwwwwwwwwwwww\n"
    "#  ####    W  GTTTTTTTGGGwwwwwwwwwwwwwwwwwwwwww\n"
    "#  #.     WWW  GTTTTTGGGGwwwwwwwwwwwwwwwwwwwwww\n"
    "# $#     WWWWW GGGGGGGGSSwwwwwwwwwwwwwwwwwwwwww\n"
    "#  # $   W WWW  GGGGGGSSSwwwwwwwwwwwwwwwwwwwwww\n"
    "#  ##.#         GGGGGSSSSwwwwwwwwwwwwwwwwwwwwww\n"
    "#     #         GGSSSSSSSwwwwwwwwwwwwwwwwwwwwww\n"
    "#            SSSSSSSSSSSSwwwwwwwwwwwwwwwwwwwwww\n"
    "########  ###########SSSSwwwwwwwwwwwwwwwwwwwwww\n"
    "#             SSSSSSSSSSSwwwwwwwwwwwwwwwwwwwwww\n"
    "#              SSSSSSSSSSwwwwwwwwwwwwwwwwwwwwww\n"
    "#               SSSSSSSSSwwwwwwwwwwwwwwwwwwwwww\n"
    "#                SSSSSSSSwwwwwwwwwwwwwwwwwwwwww\n"
    "#                SSSSSSSWWWWWWWWWWWWWWWWWWWWWww\n"
    "#                SSSSSSSSSSSSSSSSSSSSSSSSSSSWww\n"
    "#                SSSSSSSSSSSSSSSSSSSSSSSSSSSWww\n"
    "#                SSSSSSSWWWWWWWWWWWWWWWWWWWWWww\n"
    "#                SSSSSSSSwwwwwwwwwwwwwwwwwwwwww\n"
    "#                SSSSSSSSwwwwwwwwwwwwwwwwwwwwww\n"
    "##########################wwwwwwwwwwwwwwwwwwwww\n"
    ;

Level::Level(Player *player, uint8_t width, uint8_t height, StaticObject **background, std::vector<DynamicObject *> *game_objects) {
    this->player = player;
    this->width = width;
    this->height = height;
    this->background = background;
    this->game_objects = game_objects;

    //printf("Created new level object...\n");
    //printf("Background:\n");
    //for (int y = 0; y < height; y++) {
    //    for (int x = 0; x < width; x++) {
    //        int i = y * height + x;
    //        printf("'%s'\t", background[i]->get_str());
    //    }
    //    printf("\n");
    //}
    //printf("\n");

    //printf("Game objects:\n");
    //for (int y = 0; y < height; y++) {
    //    for (int x = 0; x < width; x++) {
    //        std::vector<DynamicObject *> *objects = get_objects(x, y);

    //        if (objects->size() == 0) {
    //            printf("'\t\t'\t");
    //        } else {
    //            DynamicObject *obj = (*objects)[0];
    //            printf("'%s'[%ld]\t", obj->get_str(), objects->size());
    //        }
    //        delete objects;
    //    }
    //    printf("\n");
    //}
}

Level::~Level() {
    if (player != NULL) {
        delete player;
        player = NULL;
    }

    if (background != NULL) {
        for (int i = 0; i < width * height; i++) {
            delete background[i];
        }
        free(background);
        background = NULL;
    }

    if (game_objects != NULL) {
        for (size_t i = 0; i < game_objects->size(); i++) {
            delete (*game_objects)[i];
        }
        delete game_objects;
        game_objects = NULL;
    }
}

bool Level::is_solid(int x, int y) {
    StaticObject *obj = background[y * width + x];
    //printf("[is_solid()] Object at position (%d,%d): '%s'\n",
    //        x, y, obj->get_str());
    //printf("[is_solid()] is_solid(): %s\n", (obj->is_solid()) ? "true" : "false");
    return obj->is_solid();
}

std::vector<DynamicObject *> *Level::get_objects(int x, int y) {
    std::vector<DynamicObject *> *objects = new std::vector<DynamicObject *>;

    // Search objects at given position
    for (unsigned long i = 0; i < game_objects->size(); i++) {
        DynamicObject *obj = (*game_objects)[i];
        if (obj->get_x() == x && obj->get_y() == y) {
            objects->push_back(obj);
        }
    }

    // Sort list according to z coordinate
    if (objects->size() > 1) {
        for (size_t i = 0; i < objects->size() - 1; i++) {
            DynamicObject *obj1 = (*objects)[i];
            DynamicObject *obj2 = (*objects)[i + 1];

            if (obj1->get_z() > obj2->get_z()) {
                // Swap
                (*objects)[i + 1] = obj1;
                (*objects)[i]     = obj2;
            }
        }
    }

    // Return list of objects at given position
    return objects;
}

/**
 * @brief Draws the complete gameboard.
 */
bool Level::render() {
    // Rendering failed if at least one object failed rendering
    bool success = true;

    // Render the background
    for (int i = 0; i < width * height; i++) {
        if (!background[i]->render()) {
            success = false;
        }
    }

    // Render game objects
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            std::vector<DynamicObject *> *objs = get_objects(x, y);
            for (size_t i = 0; i < objs->size(); i++) {
                DynamicObject *obj = (*objs)[i];
                if (obj == NULL) {
                    continue;
                }
                if (!obj->render()) {
                    success = false;
                }
            }
            delete objs;
        }
    }

    return success;
}

static Level* parse_level(const char *raw, long len) {
    // Read width and height of level
    uint8_t w = 0;
    uint8_t h = 0;
    for (int i = 0; i < len; i++) {
        uint8_t line_w = 0;
        while (i < len && raw[i] != '\n') {
            line_w++;
            i++;
        }
        w = (line_w > w) ? line_w : w;
        h++;
    }

    // Parse level
    int soko_x = -1;
    int soko_y = -1;

    StaticObject **background = (StaticObject **)malloc((h * w) * sizeof(StaticObject *));
    if (background == NULL) {
        fprintf(stderr, "Could not allocate memory for buffer\n");
        return NULL;
    }

    std::vector<DynamicObject *> *game_objects = new std::vector<DynamicObject *>;
    if (background == NULL) {
        fprintf(stderr, "Could not allocate memory for buffer\n");
        return NULL;
    }

    {
        int i, j;
        for (i = 0, j = 0; i < len; i++, j++) {
            char c = raw[i];

            // If there is a newline we may have to add padding to
            // the end of the line if it is not already done.
            if (c == '\n') {
                if (j % w != 0) {
                    int spaces = w - (j % w);

                    for (int k = 0; k < spaces; k++) {
                        background[j + k] = new Free((j + k) % w, (j + k) / w);
                        j++;
                    }

                    // j has been incremented one too many times
                    j--;
                } else {
                    // Don't add newline character
                    j--;
                }
                continue;
            }

            // @ marks the start of Sokoban
            if (c == '@') {
                if (soko_x != -1 || soko_y != -1) {
                    fprintf(stderr,
                            "Sokoban position has already been set in this level!\n"
                            "Overwriting old Sokoban position (%d,%d) with (%d,%d)\n",
                            soko_x, soko_y, j % w, j / w);
                }
                soko_x = j % w;
                soko_y = j / w;

                background[j] = new Free(j % w, j / w);
                continue;
            }

            // + marks Sokoban standing on a target
            if (c == '+') {
                if (soko_x != -1 || soko_y != -1) {
                    fprintf(stderr,
                            "Sokoban position has already been set in this level!\n"
                            "Overwriting old Sokoban position (%d,%d) with (%d,%d)\n",
                            soko_x, soko_y, j % w, j / w);
                }
                soko_x = j % w;
                soko_y = j / w;

                background[j] = new Free(j % w, j / w);
                game_objects->push_back(new Target(j % w, j / w));
                continue;
            }

            // * marks a box on a target
            if (c == '*') {
                background[j] = new Free(j % w, j / w);
                game_objects->push_back(new Target(j % w, j / w));
                game_objects->push_back(new Box(j % w, j / w));
                continue;
            }

            // # marks a wall
            if (c == '#') {
                background[j] = new Wall(j % w, j / w);
                continue;
            }

            // . marks a target
            if (c == '.') {
                background[j] = new Free(j % w, j / w);
                game_objects->push_back(new Target(j % w, j / w));
                continue;
            }

            // $ marks a box
            if (c == '$') {
                background[j] = new Free(j % w, j / w);
                game_objects->push_back(new Box(j % w, j / w));
                continue;
            }

            // A space marks a free field
            if (c == ' ') {
                background[j] = new Free(j % w, j / w);
                continue;
            }

            // G marks grass
            if (c == 'G') {
                background[j] = new Grass(j % w, j / w);
                continue;
            }

            // S marks sand
            if (c == 'S') {
                background[j] = new Sand(j % w, j / w);
                continue;
            }

            // w marks water
            if (c == 'w') {
                background[j] = new Water(j % w, j / w);
                continue;
            }

            // W marks a woodplank
            if (c == 'W') {
                background[j] = new Woodplank(j % w, j / w);
                continue;
            }

            // W marks a woodplank
            if (c == 'T') {
                background[j] = new Grass(j % w, j / w);
                game_objects->push_back(new Tree(j % w, j / w));
                continue;
            }

            fprintf(stderr, "Found illegal field '%c' in level!\n"
                    "Assuming as wall\n", c);
            background[j] = new Wall(j % w, j / w);
        }
    }

    // Create new level
    PLAYER = new Player(soko_x, soko_y, "player");
    return new Level(PLAYER, w, h, background, game_objects);
}

bool set_level_from_file(const char *filepath) {
    Level *level = Level_from_file(filepath);

    if (level == NULL) {
        return false;
    }

    // Set new level
    if (CURRENT_LEVEL != NULL) {
        delete CURRENT_LEVEL;
    }
    CURRENT_LEVEL = level;

    // After loading level reinitialize the scaler
    // TODO: Is it necessary to reinitialize the scaler on level change?
    update_window_properties();

    return true;
}

bool set_level_from_buffer(const char *buf, long len) {
    Level *level = Level_from_buffer(buf, len);

    if (level == NULL) {
        return false;
    }

    // Set new level
    if (CURRENT_LEVEL != NULL) {
        delete CURRENT_LEVEL;
    }
    CURRENT_LEVEL = level;

    // After loading level reinitialize the scaler
    // TODO: Is it necessary to reinitialize the scaler on level change?
    update_window_properties();

    return true;
}

/**
 * @brief Loads a level from a give file
 *
 * @param filepath The path to the level file
 *
 * @return True if loading was successful, false otherwise
 */
Level* Level_from_file(const char *filepath) {
    char *buf;
    long len;

    // Read file
    {
        FILE *f = fopen(filepath, "r");
        if (f == NULL) {
            fprintf(stderr, "Could not open level file '%s'\n",
                    filepath);
            return NULL;
        }

        fseek(f, 0, SEEK_END);
        len = ftell(f);
        fseek(f, 0, SEEK_SET);
        buf = (char *)malloc(len);

        if (buf == NULL) {
            fprintf(stderr, "Could not allocate memory for file buffer\n");
            fclose(f);
            free(buf);
            return NULL;
        }

        fread(buf, 1, len, f);
        fclose(f);
    }

    Level *level = Level_from_buffer(buf, len);
    free(buf);
    return level;
}

/**
 * @brief Loads a level from a buffer
 *
 * @param buf The buffer containing the level
 * @param len The length of the given buffer
 *
 * @return True if loading was successful, false otherwise
 */
Level* Level_from_buffer(const char *buf, long len) {
    // TODO: Implement real level format

    char *buf2;
    long len2;
    // Extract level (Skip first line)
    {
        // Check how many characters are in first line
        len2 = len;
        for (int i = 0; i < len; i++) {
            len2--;
            if (buf[i] == '\n') {
                break;
            }
        }

        // Create new buffer and copy relevant bytes from old buffer
        buf2 = (char *)malloc(len2);
        if (buf2 == NULL) {
            fprintf(stderr, "Could not allocate memory for file buffer\n");
            free(buf2);
            return NULL;
        }
        {
            int i, j;
            for (i = len - len2, j = 0; i < len; i++, j++) {
                buf2[j] = buf[i];
            }
        }
    }

    Level *level = parse_level(buf2, len2);
    free(buf2);
    return level;
}

