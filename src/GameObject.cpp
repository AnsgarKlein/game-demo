
#include "GameObject.h"

#include "Game.h"
#include "Level.h"
#include "View.h"

GameObject::GameObject(int x, int y, SpriteSheet *sprites) {
    this->x = x;
    this->y = y;
    this->sprites = sprites;
}

GameObject::~GameObject() {
    //
}

int GameObject::get_x() {
    return x;
}

int GameObject::get_y() {
    return y;
}

bool GameObject::render() {
    if (sprites == NULL) {
        std::cerr << "Error when displaying GameObject at (";
        std::cerr << x << ',' << y << ")" << std::endl;
        std::cerr << "No sprite to display!" << std::endl;
        std::cerr << std::endl;
        return true;
    }

    return sprites->render(x * 32, y * 32);
}

// TODO: Remove old render code outside of SpriteSheet
//bool GameObject::render_animated(const SDL_Texture *text, int x, int y, uint16_t frame_time, uint8_t frames, bool overlay) {
//    int frame = (GAME_TIME / frame_time) % frames;
//
//    // If this is an overlay animation first draw the base frame
//    // then draw animation on top
//    if (overlay && frame != 0) {
//        render_static(text, x, y);
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

//bool GameObject::render_with_direction(const SDL_Texture *text, int x, int y, enum Direction direction) {
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

//bool GameObject::render_with_variation(const SDL_Texture *text, int x, int y, int variations, int commonness) {
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
//        render_static(text, x, y);
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

//bool GameObject::render_permutated(const SDL_Texture *text, int x, int y) {
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

//bool GameObject::render_static(const SDL_Texture *text, int x, int y) {
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

