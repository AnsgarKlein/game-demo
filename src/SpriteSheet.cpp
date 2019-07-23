
#include "SpriteSheet.h"

#include "View.h"
#include "Game.h"
#include "Level.h"

#include "View.h"

//static bool INTERNAL_render_static(const SDL_Texture *text, unsigned int x, unsigned int y) {
//    // Dont clip image
//    SDL_Rect clip;
//    clip.x = 0;
//    clip.y = 0;
//    clip.w = GRID_SIZE;
//    clip.h = GRID_SIZE;
//
//    // Don't render if position is off camera
//    //    (Actually do render the tile exactly next to the 
//    //    camera because it could partly be inside the camera)
//    if (x + GRID_SIZE < CAMERA_X || y + GRID_SIZE < CAMERA_Y) {
//        return false;
//    }
//    if (x + GRID_SIZE - CAMERA_X > CAMERA_WIDTH) {
//        return false;
//    }
//    if (y + GRID_SIZE - CAMERA_Y > CAMERA_HEIGHT) {
//        return false;
//    }
//
//    // Calculate position inside of camera
//    int pos_x = x - CAMERA_X;
//    int pos_y = y - CAMERA_Y;
//
//    // Note:
//    // This position could actually be negative.
//    // For example if a sprite begins off camera but is partly inside
//    // the camera.
//    // If we give this negative coordinate to SDL it seems to do the
//    // right thing. If this makes problems in the future we can cut out
//    // the necessary part of the texture and render it at coordinate 0.
//
//    // Set viewport to field
//    SDL_Rect view;
//    view.x = ORIGIN_X + (pos_x * SCALE);
//    view.y = ORIGIN_Y + (pos_y * SCALE);
//    view.w = GRID_SIZE * SCALE;
//    view.h = GRID_SIZE * SCALE;
//
//    if (SDL_RenderSetViewport(WINRENDERER, &view) != 0) {
//        fprintf(stderr, "Could not set viewport for rendering!\n"
//                "SDL_Error: '%s'\n", SDL_GetError());
//        return false;
//    }
//
//    int render_success = SDL_RenderCopy(WINRENDERER, (SDL_Texture *)text,
//                                        &clip, NULL);
//    if (render_success != 0) {
//        fprintf(stderr, "Could not render texture!\n"
//                "SDL_Error: '%s'\n", SDL_GetError());
//        return false;
//    }
//
//    return true;
//}

//static bool INTERNAL_render_animated(const SDL_Texture *text, int x, int y, SpriteState *state) {
//    // Check how many frames are available
//    int frames_c = 0;
//    Point **frames_v = NULL;
//    state->get_frames(&frames_c, &frames_v);
//
//    // Select the correct frame
//    int frame_i = (GAME_TIME / state->get_frame_time()) % frames_c;
//    Point *frame = frames_v[frame_i];
//
//    // If this is an overlay animation first draw the base frame
//    // then draw animation on top
//    if (state->get_overlay() && frame_i != 0) {
//        INTERNAL_render_static(text, x, y);
//    }
//
//    // Cut correct animation frame from texture
//    SDL_Rect clip;
//    clip.x = frame->x * GRID_SIZE;
//    clip.y = frame->y * GRID_SIZE;
//    clip.w = GRID_SIZE;
//    clip.h = GRID_SIZE;
//
//    // Set viewport to field
//    SDL_Rect view;
//    view.x = ORIGIN_X + (x * GRID_SIZE * SCALE);
//    view.y = ORIGIN_Y + (y * GRID_SIZE * SCALE);
//    view.w = GRID_SIZE * SCALE;
//    view.h = GRID_SIZE * SCALE;
//
//    if (SDL_RenderSetViewport(WINRENDERER, &view) != 0) {
//        fprintf(stderr, "Could not set viewport for rendering!\n"
//                "SDL_Error: '%s'\n", SDL_GetError());
//        return false;
//    }
//
//    int render_success = SDL_RenderCopy(WINRENDERER, (SDL_Texture *)text,
//                                        &clip, NULL);
//    if (render_success != 0) {
//        fprintf(stderr, "Could not render texture!\n"
//                "SDL_Error: '%s'\n", SDL_GetError());
//        return false;
//    }
//
//    return true;
//}

//static bool INTERNAL_render_animated(const SDL_Texture *text, int x, int y, uint16_t frame_time, uint8_t frames, bool overlay) {
//    int frame = (GAME_TIME / frame_time) % frames;
//
//    // If this is an overlay animation first draw the base frame
//    // then draw animation on top
//    if (overlay && frame != 0) {
//        INTERNAL_render_static(text, x, y);
//    }
//
//    // Select correct animation frame
//    SDL_Rect clip;
//    clip.x = frame * GRID_SIZE;
//    clip.y = 0;
//    clip.w = GRID_SIZE;
//    clip.h = GRID_SIZE;
//
//    // Set viewport to field
//    SDL_Rect view;
//    view.x = ORIGIN_X + (x * GRID_SIZE * SCALE);
//    view.y = ORIGIN_Y + (y * GRID_SIZE * SCALE);
//    view.w = GRID_SIZE * SCALE;
//    view.h = GRID_SIZE * SCALE;
//
//    if (SDL_RenderSetViewport(WINRENDERER, &view) != 0) {
//        fprintf(stderr, "Could not set viewport for rendering!\n"
//                "SDL_Error: '%s'\n", SDL_GetError());
//        return false;
//    }
//
//    int render_success = SDL_RenderCopy(WINRENDERER, (SDL_Texture *)text,
//                                        &clip, NULL);
//    if (render_success != 0) {
//        fprintf(stderr, "Could not render texture!\n"
//                "SDL_Error: '%s'\n", SDL_GetError());
//        return false;
//    }
//
//    return true;
//}

//static bool INTERNAL_render_with_direction(const SDL_Texture *text, int x, int y, enum Direction direction) {
//    // Dont clip image by default
//    SDL_Rect clip;
//    clip.x = 0;
//    clip.y = 0;
//    clip.w = GRID_SIZE;
//    clip.h = GRID_SIZE;
//
//    // Dont flip image by default
//    SDL_RendererFlip flip = SDL_FLIP_NONE;
//
//    // Depending on direction use different frame or flip image
//    switch (direction) {
//        case FACING_RIGHT:
//            break;
//        case FACING_LEFT:
//            flip = SDL_FLIP_HORIZONTAL;
//            break;
//        case FACING_DOWN:
//            clip.y = GRID_SIZE;
//            break;
//        case FACING_UP:
//            clip.y = 2 * GRID_SIZE;
//            break;
//    }
//
//    // Set viewport to field
//    SDL_Rect view;
//    view.x = ORIGIN_X + (x * GRID_SIZE * SCALE);
//    view.y = ORIGIN_Y + (y * GRID_SIZE * SCALE);
//    view.w = GRID_SIZE * SCALE;
//    view.h = GRID_SIZE * SCALE;
//
//    if (SDL_RenderSetViewport(WINRENDERER, &view) != 0) {
//        fprintf(stderr, "Could not set viewport for rendering!\n"
//                "SDL_Error: '%s'\n", SDL_GetError());
//        return false;
//    }
//
//    // Render
//    int render_success;
//    if (flip == SDL_FLIP_NONE) {
//        render_success = SDL_RenderCopy(WINRENDERER, (SDL_Texture *)text,
//                                        &clip, NULL);
//    } else {
//        render_success = SDL_RenderCopyEx(WINRENDERER, (SDL_Texture *)text,
//                                          &clip, NULL,
//                                          0, NULL, flip);
//    }
//    if (render_success != 0) {
//        fprintf(stderr, "Could not render texture!\n"
//                "SDL_Error: '%s'\n", SDL_GetError());
//        return false;
//    }
//
//    return true;
//
//}

//static bool INTERNAL_render_with_variation(const SDL_Texture *text, int x, int y, int variations, int commonness) {
//    unsigned int i = y * CURRENT_LEVEL->get_width() + x;
//    uint8_t frame = 0;
//
//    // Check if texture should be varified
//    if (i % commonness == 0) {
//        // Select the correct frame
//        unsigned int j = i / commonness;
//        frame = j % variations;
//
//        // First render the base frame, then render variation on top
//        INTERNAL_render_static(text, x, y);
//    }
//
//    // Select correct frame
//    SDL_Rect clip;
//    clip.x = frame * GRID_SIZE;
//    clip.y = 0;
//    clip.w = GRID_SIZE;
//    clip.h = GRID_SIZE;
//
//    // Set viewport to field
//    SDL_Rect view;
//    view.x = ORIGIN_X + (x * GRID_SIZE * SCALE);
//    view.y = ORIGIN_Y + (y * GRID_SIZE * SCALE);
//    view.w = GRID_SIZE * SCALE;
//    view.h = GRID_SIZE * SCALE;
//
//    if (SDL_RenderSetViewport(WINRENDERER, &view) != 0) {
//        fprintf(stderr, "Could not set viewport for rendering!\n"
//                "SDL_Error: '%s'\n", SDL_GetError());
//        return false;
//    }
//
//    int render_success = SDL_RenderCopy(WINRENDERER, (SDL_Texture *)text,
//                                        &clip, NULL);
//    if (render_success != 0) {
//        fprintf(stderr, "Could not render texture!\n"
//                "SDL_Error: '%s'\n", SDL_GetError());
//        return false;
//    }
//
//    return true;
//}

//static bool INTERNAL_render_permutated(const SDL_Texture *text, int x, int y) {
//    // Dont clip image
//    SDL_Rect clip;
//    clip.x = 0;
//    clip.y = 0;
//    clip.w = GRID_SIZE;
//    clip.h = GRID_SIZE;
//
//    // Set viewport to field
//    SDL_Rect view;
//    view.x = ORIGIN_X + (x * GRID_SIZE * SCALE);
//    view.y = ORIGIN_Y + (y * GRID_SIZE * SCALE);
//    view.w = GRID_SIZE * SCALE;
//    view.h = GRID_SIZE * SCALE;
//
//    double angle;
//    SDL_RendererFlip flip;
//
//    // Calculate rotation and flipping of field depending on coordinate
//    // (We have 8 different states of texture)
//    int i = (y * CURRENT_LEVEL->get_width() + x) % 8;
//    switch (i) {
//        // Normal
//        case 0:
//            angle = 0.f;
//            flip = SDL_FLIP_NONE;
//            break;
//        // Rotate 90
//        case 1:
//            angle = 90;
//            flip = SDL_FLIP_NONE;
//            break;
//        // Rotate 180
//        case 2:
//            angle = 180;
//            flip = SDL_FLIP_NONE;
//            break;
//        // Rotate 270
//        case 3:
//            angle = 270;
//            flip = SDL_FLIP_NONE;
//            break;
//        // Flip horizontally
//        case 4:
//            angle = 0.f;
//            flip = SDL_FLIP_HORIZONTAL;
//            break;
//        // Flip vertically
//        case 5:
//            angle = 0.f;
//            flip = SDL_FLIP_VERTICAL;
//            break;
//        // Flip horizontally and rotate 90
//        case 6:
//            flip = SDL_FLIP_HORIZONTAL;
//            angle = 90;
//            break;
//        // Flip vertically and rotate 90
//        case 7:
//            flip = SDL_FLIP_VERTICAL;
//            angle = 90;
//            break;
//        default:
//            // this should never happen
//            angle = 0.f;
//            flip = SDL_FLIP_NONE;
//            break;
//    }
//
//    // Render
//    if (SDL_RenderSetViewport(WINRENDERER, &view) != 0) {
//        fprintf(stderr, "Could not set viewport for rendering!\n"
//                "SDL_Error: '%s'\n", SDL_GetError());
//        return false;
//    }
//    int render_success = SDL_RenderCopyEx(WINRENDERER, (SDL_Texture *)text,
//                                          &clip, NULL, angle, NULL, flip);
//    if (render_success != 0) {
//        fprintf(stderr, "Could not render texture!\n"
//                "SDL_Error: '%s'\n", SDL_GetError());
//        return false;
//    }
//
//    return true;
//}

/**
 * @brief Creates an SDL_Surface from raw binary data.
 *
 * @param image The memory location where the SDL_Surface will be created.
 * @param buffer The raw binary image data from which the SDL_Surface will
 *        be created.
 * @param length The length of the raw binary data.
 */
//static bool loadIMG_from_buffer(SDL_Surface **image, const char *buffer, const int length) {
//    // Create SDL_RWops from buffer (this does not copy memory!)
//    SDL_RWops *rw = SDL_RWFromConstMem(buffer, length);
//
//    // Copy buffer into SDL_Surface and free SDL_RWops
//    SDL_Surface *temp = IMG_Load_RW(rw, 0);
//    SDL_RWclose(rw);
//    if (temp == NULL) {
//        fprintf(stderr, "Could not load image from buffer\n");
//        fprintf(stderr, "IMG_Load_RW: '%s'\n", IMG_GetError());
//        return false;
//    }
//
//    *image = temp;
//
//    return true;
//}

static bool loadIMG_from_file(SDL_Texture **texture, const char *filename) {
    // TODO: Move file handling code to extra class

    // Cut off the file ending (last 3 characters)
    const char *ending = "";
    if (strlen(filename) > 3) {
        ending = &(filename[strlen(filename) - 3]);
    }

    // Create SDL_Surface from file
    SDL_Surface *surf = IMG_Load(filename);
    if (surf == NULL) {
        fprintf(stderr, "Could not load image '%s', SDL_Error: '%s'\n",
                filename, SDL_GetError());
        return false;
    }

    // Create texture from surface
    *texture = SDL_CreateTextureFromSurface(WINRENDERER, surf);
    if (*texture == NULL) {
        fprintf(stderr, "Could not create texture from loaded SDL_Surface\n"
                "SDL_Error: '%s'\n", SDL_GetError());
        SDL_FreeSurface(surf);
        return false;
    }
    SDL_FreeSurface(surf);

    return true;
}

SpriteSheet::SpriteSheet(std::string *id, std::string *path) {
    this->id = id;

    // Load texture data
    SDL_Texture *text;
    loadIMG_from_file(&text, path->c_str());
    this->texture = text;
    delete path;

    // Set default state
    this->states_c = 1;
    this->states_v = new SpriteState*[1] { new SpriteState() };
}

SpriteSheet::SpriteSheet(std::string *id, std::string *path, int states_c, SpriteState *states_v[]) {
    this->id = id;

    // Load texture data
    SDL_Texture *text;
    loadIMG_from_file(&text, path->c_str());
    this->texture = text;
    delete path;

    this->states_c = states_c;
    this->states_v = states_v;
}

SpriteSheet::~SpriteSheet() {
    // Free id
    delete this->id;

    // Delete texture data
    SDL_DestroyTexture(texture);

    // Delete all states
    if (states_v != NULL) {
        for (int i = 0; i < states_c; i++) {
            delete states_v[i];
        }
        delete[] states_v;
        states_v = NULL;
    }

}

std::string *SpriteSheet::get_id() {
    return this->id;
}

SDL_Texture *SpriteSheet::get_texture() {
    return this->texture;
}

SpriteState *SpriteSheet::get_state(std::string *state_str) {
    // Default state is always the first one
    if (*state_str == "default") {
        return states_v[0];
    }

    // Go through all states and search correct one
    for (int i = 0; i < states_c; i++) {
        if (states_v[i]->get_id() == *state_str) {
            return states_v[i];
        }
    }

    fprintf(stderr, "Could not find requested state '%s'\n", state_str->c_str());
    return NULL;
}

bool SpriteSheet::render(unsigned int x, unsigned int y) {
    return render(x, y, "default");
}

bool SpriteSheet::render(unsigned int x, unsigned int y, const char *state_str) {
    std::string str = std::string(state_str);
    SpriteState *state = get_state(&str);

    // We didnt find the state we were looking for
    if (state == NULL) {
        if (strcmp(state_str, "default") != 0) {
            fprintf(stderr, "State '%s' does not exist - rendering default state\n", state_str);
            return render(x, y);
        } else {
            fprintf(stderr, "Could not render default state\n");
            return false;
        }
    }

    // Render
    return renderINT(x, y, state);
}

bool SpriteSheet::renderINT(unsigned int x, unsigned int y, SpriteState *state) {
    if (state == NULL) {
        fprintf(stderr, "Did not get valid Sprite frame data for rendering!\n");
        return false;
    }

    // Don't render if position is off camera
    //    (Except do render the tile exactly next to the 
    //    camera because it could partly be inside the camera)
    if (x + GRID_SIZE < CAMERA_X || y + GRID_SIZE < CAMERA_Y) {
        return false;
    }
    if (x + GRID_SIZE - CAMERA_X > CAMERA_WIDTH + GRID_SIZE) {
        return false;
    }
    if (y + GRID_SIZE - CAMERA_Y > CAMERA_HEIGHT + GRID_SIZE) {
        return false;
    }

    // Calculate position inside of camera
    //
    // Note:
    // This position could actually be negative if a sprite begins off
    // camera but is partly inside the camera.
    // If we give this negative coordinate to SDL it seems to do the
    // right thing. If this makes problems in the future we can cut out
    // the necessary part of the texture and render it at coordinate 0.
    int pos_x = x - CAMERA_X;
    int pos_y = y - CAMERA_Y;


    // TODO: Get all information at once with SpriteState::getAll()

    // Get all frames in this sprite sheet
    int sprite_count = state->get_sprite_count();
    Point **frames_v = state->get_offsets();
    if (sprite_count == 0) {
        std::cerr << "Error when rendering sprite: Did not get any sprite to render" << std::endl;
        return false;
    }

    // Select the correct sprite
    int frame_i;
    if (state->get_render_type() == RENDER_ANIMATED) {
        int frame_time = state->get_frame_time();
        if (frame_time == 0) {
            // Dont divide by zero when frame time
            // is not set (but should be set).
            frame_i = 0;
        } else {
            frame_i = (GAME_TIME / state->get_frame_time()) % sprite_count;
        }
    } else if (state->get_render_type() == RENDER_PERMUTATED) {
        frame_i = ((y / GRID_SIZE) * CURRENT_LEVEL->get_width() + (x / GRID_SIZE)) % sprite_count;
    } else {
        frame_i = 0;
    }
    Point *frame = frames_v[frame_i];

    // Select the location of the correct frame in the sprite sheet
    SDL_Rect clip;
    clip.w = GRID_SIZE;
    clip.h = GRID_SIZE;
    clip.x = frame->x * GRID_SIZE;
    clip.y = frame->y * GRID_SIZE;

    // Set viewport (where to render) to correct field in level
    SDL_Rect view;
    view.x = ORIGIN_X + (pos_x * SCALE);
    view.y = ORIGIN_Y + (pos_y * SCALE);
    view.w = GRID_SIZE * SCALE;
    view.h = GRID_SIZE * SCALE;

    if (SDL_RenderSetViewport(WINRENDERER, &view) != 0) {
        fprintf(stderr, "Could not set viewport for rendering!\n"
                "SDL_Error: '%s'\n", SDL_GetError());
        return false;
    }

    // Check if we need to rotate 
    int rotate = state->get_rotate()[frame_i];
    bool mirror_h = state->get_mirror_h()[frame_i];
    bool mirror_v = state->get_mirror_v()[frame_i];

    // Render
    int render_success;
    if (rotate == 0 && !mirror_h && !mirror_v) {
        render_success = SDL_RenderCopy(WINRENDERER, texture, &clip, NULL);
    } else {
        SDL_RendererFlip flip;

        if (mirror_h) {
            flip = SDL_FLIP_HORIZONTAL;
        } else if (mirror_v) {
            flip = SDL_FLIP_VERTICAL;
        } else {
            flip = SDL_FLIP_NONE;
        }

        render_success = SDL_RenderCopyEx(WINRENDERER, texture, &clip, NULL,
                                          (float)rotate, NULL, flip);
    }

    if (render_success != 0) {
        std::cerr << "Could not render texture!" << std::endl;
        std::cerr << "SDL_Error: '" << SDL_GetError() << "'" << std::endl;

        return false;
    }

    return true;
}

// TODO: Move includes
#include "FileReader.h"
#include "JsonLexer.h"
#include "JsonParser.h"

SpriteSheet *SpriteSheet_from_file(std::string path) {
    // Read file
    std::string *content = read_file(path);
    if (content == NULL) {
        return NULL;
    }

    // Lex file content to tokens
    std::vector<JsonToken> *tokens = lex_json(content);
    if (tokens == NULL) {
        delete content;
        return NULL;
    }
    delete content;

    // Parse 
    JsonBaseObject *top = parse_json(tokens);
    if (top == NULL) {
        JsonTokens_free(tokens);
        return NULL;
    }
    JsonTokens_free(tokens);

    // Check if top element of JSON is an object
    if (typeid(*top) != typeid(JsonObject)) {
        std::cerr << "Error when parsing SpriteSheet '" << path << "'." << std::endl;
        std::cerr << "Top element in JSON should be an object." << std::endl;
        delete top;
        return NULL;
    }
    JsonObject *top_obj = (JsonObject *)top;

    // Create SpriteSheet from JSON object
    SpriteSheet *sheet = SpriteSheet_from_json(top_obj);
    delete top_obj;
    return sheet;
}

SpriteSheet *SpriteSheet_from_json(JsonObject *obj) {
    // Constants for parsing
    const std::string JSON_KEY_TEXTURE = "texture";
    const std::string JSON_KEY_ID      = "id";
    const std::string JSON_KEY_STATES  = "states";

    // Variables fo parsed values
    std::string                *parsed_texture  = NULL;
    std::string                *parsed_id       = NULL;
    int                         parsed_states_c = 0;
    std::vector<SpriteState *> *parsed_states_v = NULL;


    // Go through all children of this object and parse them
    bool parsing_error = false;
    for (auto pair : *(obj->get_children())) {
        std::string *key = pair.first;
        JsonBaseObject *val = pair.second;

        if (parsing_error) {
            break;
        }

        // Parse texture key-value pair
        if (*key == JSON_KEY_TEXTURE) {
            JsonSimple *texture = (JsonSimple *)val;

            if (typeid(*val) != typeid(JsonSimple) ||
                texture->get_type() != JSON_OBJECT_STRING ||
                texture->get_string() == NULL) {

                std::cerr << "Error when parsing SpriteSheet." << std::endl;
                std::cerr << "Key '" << *key << "' has unknown value." << std::endl;
                std::cerr << "Expected value of type to be string - ignoring pair..." << std::endl;
                std::cerr << std::endl;
                continue;
            }

            parsed_texture = new std::string(*texture->get_string());
            continue;
        }

        // Parse states key-value pair
        if (*key == JSON_KEY_STATES) {
            JsonSimple *states = (JsonSimple *)val;

            if (typeid(*val) != typeid(JsonSimple) ||
                states->get_type() != JSON_OBJECT_ARRAY ||
                states->get_array() == NULL) {

                std::cerr << "Error when parsing SpriteSheet." << std::endl;
                std::cerr << "Key '" << *key << "' has unknown value." << std::endl;
                std::cerr << "Expected value of type to be array - ignoring pair..." << std::endl;
                std::cerr << std::endl;
                continue;
            }

            // Allocate space for all states
            if (parsed_states_v == NULL) {
                parsed_states_v = new std::vector<SpriteState *>();
            }

            // Go through all states and parse them
            for (JsonBaseObject *base_obj : *states->get_array()) {
                if (typeid(*base_obj) != typeid(JsonObject)) {
                    std::cerr << "Error when parsing SpriteSheet." << std::endl;
                    std::cerr << "State should be of type object - ignoring state..." << std::endl;
                    std::cerr << std::endl;
                    continue;
                }

                // Parse state
                JsonObject *state_obj = (JsonObject *)base_obj;
                SpriteState *state = SpriteState_from_json(state_obj);
                if (state != NULL) {
                    parsed_states_v->push_back(state);
                }
            }

            continue;
        }

        // Parse id key-value pair
        if (*key == JSON_KEY_ID) {
            JsonSimple *id = (JsonSimple *)val;

            if (typeid(*val) != typeid(JsonSimple) ||
                id->get_type() != JSON_OBJECT_STRING ||
                id->get_string() == NULL) {

                std::cerr << "Error when parsing SpriteSheet." << std::endl;
                std::cerr << "Key '" << *key << "' has unknown value." << std::endl;
                std::cerr << "Expected value to of type string - ignoring pair..." << std::endl;
                std::cerr << std::endl;
                continue;
            }

            parsed_id = new std::string(*id->get_string());
            continue;
        }

        // Unknown key-value pair will get ignored
        std::cerr << "Error when parsing SpriteSheet." << std::endl;
        std::cerr << "Found unknown key '" << *key << "' - ignoring ..." << std::endl;
        std::cerr << std::endl;
    }
    parsed_states_c = parsed_states_v->size();


    // Check if all necessary pairs where found
    if (parsed_texture == NULL ||
        parsed_id == NULL ||
        parsed_states_v == NULL ||
        parsed_states_c == 0) {

        std::cerr << "Error when parsing SpriteSheet." << std::endl;
        std::cerr << "Did not find all necessary key-value pairs." << std::endl;
        std::cerr << "Necessary are:" << std::endl;
        std::cerr << "  - " << JSON_KEY_TEXTURE << std::endl;
        std::cerr << "  - " << JSON_KEY_ID << std::endl;
        std::cerr << "  - " << JSON_KEY_STATES << std::endl;

        parsing_error = true;
    }

    if (parsing_error) {
        if (parsed_texture != NULL) {
            delete parsed_texture;
        }

        if (parsed_id != NULL) {
            delete parsed_id;
        }

        if (parsed_states_v != NULL) {
            delete parsed_states_v;
        }

        return NULL;
    }

    // Copy content of vectors to arrays
    SpriteState **parsed_states_v_arr = new SpriteState*[parsed_states_c];
    std::copy(parsed_states_v->begin(), parsed_states_v->end(),  parsed_states_v_arr);
    delete parsed_states_v;
    parsed_states_v = NULL;

    // Create SpriteSheet from parsed results
    return new SpriteSheet(parsed_id, parsed_texture, parsed_states_c, parsed_states_v_arr);
}

