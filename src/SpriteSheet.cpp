
#include "SpriteSheet.h"

#include "Direction.h"
#include "View.h"
#include "Game.h"
#include "Level.h"

#include "View.h"

static bool INTERNAL_render_static(const SDL_Texture *text, unsigned int x, unsigned int y) {
    // Dont clip image
    SDL_Rect clip;
    clip.x = 0;
    clip.y = 0;
    clip.w = GRID_SIZE;
    clip.h = GRID_SIZE;

    // Dont render if position is offscreen (off camera)
    if (x < CAMERA_X || x - CAMERA_X > CAMERA_WIDTH) {
        return false;
    }
    if (y < CAMERA_Y || y - CAMERA_Y > CAMERA_HEIGHT) {
        return false;
    }

    // Calculate position relative to camera
    unsigned int pos_x = x - CAMERA_X;
    unsigned int pos_y = y - CAMERA_Y;

    // Set viewport to field
    SDL_Rect view;
    //view.x = ORIGIN_X + (x * GRID_SIZE * SCALE);
    //view.y = ORIGIN_Y + (y * GRID_SIZE * SCALE);
    //view.w = GRID_SIZE * SCALE;
    //view.h = GRID_SIZE * SCALE;
    view.x = ORIGIN_X + (pos_x * GRID_SIZE * SCALE);
    view.y = ORIGIN_Y + (pos_y * GRID_SIZE * SCALE);
    view.w = GRID_SIZE * SCALE;
    view.h = GRID_SIZE * SCALE;

    if (SDL_RenderSetViewport(WINRENDERER, &view) != 0) {
        fprintf(stderr, "Could not set viewport for rendering!\n"
                "SDL_Error: '%s'\n", SDL_GetError());
        return false;
    }

    int render_success = SDL_RenderCopy(WINRENDERER, (SDL_Texture *)text,
                                        &clip, NULL);
    if (render_success != 0) {
        fprintf(stderr, "Could not render texture!\n"
                "SDL_Error: '%s'\n", SDL_GetError());
        return false;
    }

    return true;
}

static bool INTERNAL_render_animated(const SDL_Texture *text, int x, int y, SpriteState *state) {
    //TODO: Remove this
    return INTERNAL_render_static(text, x, y);



    // Check how many frames are available
    int frames_c = 0;
    Point **frames_v = NULL;
    state->get_frames(&frames_c, &frames_v);

    // Select the correct frame
    int frame_i = (GAME_TIME / state->get_frame_time()) % frames_c;
    Point *frame = frames_v[frame_i];

    // If this is an overlay animation first draw the base frame
    // then draw animation on top
    if (state->get_overlay() && frame_i != 0) {
        INTERNAL_render_static(text, x, y);
    }

    // Cut correct animation frame from texture
    SDL_Rect clip;
    clip.x = frame->x * GRID_SIZE;
    clip.y = frame->y * GRID_SIZE;
    clip.w = GRID_SIZE;
    clip.h = GRID_SIZE;

    // Set viewport to field
    SDL_Rect view;
    view.x = ORIGIN_X + (x * GRID_SIZE * SCALE);
    view.y = ORIGIN_Y + (y * GRID_SIZE * SCALE);
    view.w = GRID_SIZE * SCALE;
    view.h = GRID_SIZE * SCALE;

    if (SDL_RenderSetViewport(WINRENDERER, &view) != 0) {
        fprintf(stderr, "Could not set viewport for rendering!\n"
                "SDL_Error: '%s'\n", SDL_GetError());
        return false;
    }

    int render_success = SDL_RenderCopy(WINRENDERER, (SDL_Texture *)text,
                                        &clip, NULL);
    if (render_success != 0) {
        fprintf(stderr, "Could not render texture!\n"
                "SDL_Error: '%s'\n", SDL_GetError());
        return false;
    }

    return true;
}

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

static bool INTERNAL_render_with_direction(const SDL_Texture *text, int x, int y, enum Direction direction) {
    //TODO: Remove this
    return INTERNAL_render_static(text, x, y);



    // Dont clip image by default
    SDL_Rect clip;
    clip.x = 0;
    clip.y = 0;
    clip.w = GRID_SIZE;
    clip.h = GRID_SIZE;

    // Dont flip image by default
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    // Depending on direction use different frame or flip image
    switch (direction) {
        case FACING_RIGHT:
            break;
        case FACING_LEFT:
            flip = SDL_FLIP_HORIZONTAL;
            break;
        case FACING_DOWN:
            clip.y = GRID_SIZE;
            break;
        case FACING_UP:
            clip.y = 2 * GRID_SIZE;
            break;
    }

    // Set viewport to field
    SDL_Rect view;
    view.x = ORIGIN_X + (x * GRID_SIZE * SCALE);
    view.y = ORIGIN_Y + (y * GRID_SIZE * SCALE);
    view.w = GRID_SIZE * SCALE;
    view.h = GRID_SIZE * SCALE;

    if (SDL_RenderSetViewport(WINRENDERER, &view) != 0) {
        fprintf(stderr, "Could not set viewport for rendering!\n"
                "SDL_Error: '%s'\n", SDL_GetError());
        return false;
    }

    // Render
    int render_success;
    if (flip == SDL_FLIP_NONE) {
        render_success = SDL_RenderCopy(WINRENDERER, (SDL_Texture *)text,
                                        &clip, NULL);
    } else {
        render_success = SDL_RenderCopyEx(WINRENDERER, (SDL_Texture *)text,
                                          &clip, NULL,
                                          0, NULL, flip);
    }
    if (render_success != 0) {
        fprintf(stderr, "Could not render texture!\n"
                "SDL_Error: '%s'\n", SDL_GetError());
        return false;
    }

    return true;

}

static bool INTERNAL_render_with_variation(const SDL_Texture *text, int x, int y, int variations, int commonness) {
    //TODO: Remove this
    return INTERNAL_render_static(text, x, y);



    unsigned int i = y * CURRENT_LEVEL->get_width() + x;
    uint8_t frame = 0;

    // Check if texture should be varified
    if (i % commonness == 0) {
        // Select the correct frame
        unsigned int j = i / commonness;
        frame = j % variations;

        // First render the base frame, then render variation on top
        INTERNAL_render_static(text, x, y);
    }

    // Select correct frame
    SDL_Rect clip;
    clip.x = frame * GRID_SIZE;
    clip.y = 0;
    clip.w = GRID_SIZE;
    clip.h = GRID_SIZE;

    // Set viewport to field
    SDL_Rect view;
    view.x = ORIGIN_X + (x * GRID_SIZE * SCALE);
    view.y = ORIGIN_Y + (y * GRID_SIZE * SCALE);
    view.w = GRID_SIZE * SCALE;
    view.h = GRID_SIZE * SCALE;

    if (SDL_RenderSetViewport(WINRENDERER, &view) != 0) {
        fprintf(stderr, "Could not set viewport for rendering!\n"
                "SDL_Error: '%s'\n", SDL_GetError());
        return false;
    }

    int render_success = SDL_RenderCopy(WINRENDERER, (SDL_Texture *)text,
                                        &clip, NULL);
    if (render_success != 0) {
        fprintf(stderr, "Could not render texture!\n"
                "SDL_Error: '%s'\n", SDL_GetError());
        return false;
    }

    return true;
}

static bool INTERNAL_render_permutated(const SDL_Texture *text, int x, int y) {
    //TODO: Remove this
    return INTERNAL_render_static(text, x, y);



    // Dont clip image
    SDL_Rect clip;
    clip.x = 0;
    clip.y = 0;
    clip.w = GRID_SIZE;
    clip.h = GRID_SIZE;

    // Set viewport to field
    SDL_Rect view;
    view.x = ORIGIN_X + (x * GRID_SIZE * SCALE);
    view.y = ORIGIN_Y + (y * GRID_SIZE * SCALE);
    view.w = GRID_SIZE * SCALE;
    view.h = GRID_SIZE * SCALE;

    double angle;
    SDL_RendererFlip flip;

    // Calculate rotation and flipping of field depending on coordinate
    // (We have 8 different states of texture)
    int i = (y * CURRENT_LEVEL->get_width() + x) % 8;
    switch (i) {
        // Normal
        case 0:
            angle = 0.f;
            flip = SDL_FLIP_NONE;
            break;
        // Rotate 90
        case 1:
            angle = 90;
            flip = SDL_FLIP_NONE;
            break;
        // Rotate 180
        case 2:
            angle = 180;
            flip = SDL_FLIP_NONE;
            break;
        // Rotate 270
        case 3:
            angle = 270;
            flip = SDL_FLIP_NONE;
            break;
        // Flip horizontally
        case 4:
            angle = 0.f;
            flip = SDL_FLIP_HORIZONTAL;
            break;
        // Flip vertically
        case 5:
            angle = 0.f;
            flip = SDL_FLIP_VERTICAL;
            break;
        // Flip horizontally and rotate 90
        case 6:
            flip = SDL_FLIP_HORIZONTAL;
            angle = 90;
            break;
        // Flip vertically and rotate 90
        case 7:
            flip = SDL_FLIP_VERTICAL;
            angle = 90;
            break;
        default:
            // this should never happen
            angle = 0.f;
            flip = SDL_FLIP_NONE;
            break;
    }

    // Render
    if (SDL_RenderSetViewport(WINRENDERER, &view) != 0) {
        fprintf(stderr, "Could not set viewport for rendering!\n"
                "SDL_Error: '%s'\n", SDL_GetError());
        return false;
    }
    int render_success = SDL_RenderCopyEx(WINRENDERER, (SDL_Texture *)text,
                                          &clip, NULL, angle, NULL, flip);
    if (render_success != 0) {
        fprintf(stderr, "Could not render texture!\n"
                "SDL_Error: '%s'\n", SDL_GetError());
        return false;
    }

    return true;
}

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
    // Cut off the file ending (last 3 characters)
    const char *ending = "";
    if (strlen(filename) > 3) {
        ending = &(filename[strlen(filename) - 3]);
    }

    // Decide depending on file ending which method to use
    SDL_Surface *surf = NULL;
    if (strcmp(ending, "bmp") == 0) {
        surf = SDL_LoadBMP(filename);
    } else {
        surf = IMG_Load(filename);
    }
    if (surf == NULL) {
        fprintf(stderr, "Could not load image '%s', SDL_Error: '%s'\n", filename, SDL_GetError());
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

SpriteSheet::SpriteSheet(std::string *path) : SpriteSheet(path->c_str()) {
    //
}

SpriteSheet::SpriteSheet(std::string *path, int states_c, SpriteState **states_v) : SpriteSheet(path->c_str(), states_c, states_v) {
    //
}

SpriteSheet::SpriteSheet(const char *path) {
    // Load the actual sprite sheet
    SDL_Texture *text;
    loadIMG_from_file(&text, path);
    this->texture = text;

    // Save the path string as an identifier
    this->str = path;

    // Set default state
    this->states_c = 1;
    this->states_v = (SpriteState **)malloc(sizeof(SpriteState *) * 1);
    this->states_v[0] = new SpriteState();
}

SpriteSheet::SpriteSheet(const char *path, int states_c, SpriteState **states_v) {
    // Load the actual sprite sheet
    SDL_Texture *text;
    loadIMG_from_file(&text, path);
    this->texture = text;

    // Save the path string as an identifier
    this->str = path;

    this->states_c = states_c;
    this->states_v = states_v;
}

SpriteSheet::~SpriteSheet() {
    // Delete all states
    if (states_v != NULL) {
        for (int i = 0; i < states_c; i++) {
            delete states_v[i];
        }
        free(states_v);
        states_v = NULL;
    }

    // Free texture data
    SDL_DestroyTexture(texture);
}

char *SpriteSheet::get_str() {
    return (char *)this->str;
}

SDL_Texture *SpriteSheet::get_texture() {
    return this->texture;
}

SpriteState *SpriteSheet::get_state(const char *state_str) {
    // Default state is always the first one
    if (strcmp(state_str, "default") == 0) {
        return states_v[0];
    }

    // Go through all states and search correct one
    SpriteState *state = NULL;
    for (int i = 0; i < states_c; i++) {
        if (strcmp(states_v[i]->get_str(), state_str) == 0) {
            state = states_v[i];
            break;
        }
    }

    if (state == NULL) {
        fprintf(stderr, "Could not find requested state '%s'\n", state_str);
    }
    return state;
}

bool SpriteSheet::render(int x, int y) {
    return render(x, y, "default");
}

bool SpriteSheet::render(int x, int y, const char *state_str) {
    // Go through all states and search correct one
    SpriteState *state = get_state(state_str);

    // We didnt find the state we were looking for
    if (state == NULL) {
        if (strcmp(state_str, "default") != 0) {
            fprintf(stderr, "State '%s' does not exist - rendering default state\n", state_str);
            return render(x, y);
        } else {
            fprintf(stderr, "Could not render defautl state\n");
            return false;
        }
    }

    // Check how to render with settings from state
    int frames_c = 0;
    Point **frames_v = NULL;
    state->get_frames(&frames_c, &frames_v);

    if (frames_c == 0 || frames_v == NULL) {
        fprintf(stderr, "Did not get valid Sprite frame data for rendering!\n");
        return false;
    }

    // Render
    if (frames_c > 1) {
        return INTERNAL_render_animated(get_texture(), x, y, state);
    } else {
        return INTERNAL_render_static(get_texture(), x, y);
    }
}

#ifdef DEBUG_SIZE
long SpriteSheet::recursive_size() {
    long size = 0;

    // This
    size += sizeof(*this);

    // String
    size += sizeof(*str) * strlen(str);

    // Texture
    // (This does not include the actual texture data!)

    // States
    for (int i = 0; i < states_c; i++) {
        size += sizeof(states_v[i]);
        size += states_v[i]->recursive_size();
    }

    printf("Size of SpriteSheet %s is %ld\n", get_str(), size);

    return size;
}
#endif

