
#include "View.h"

#include "SpriteHandler.h"
#include "Level.h"
#include "Game.h"
#include "Player.h"
#include "Direction.h"

//Screen dimension
static unsigned int WINDOW_WIDTH  = 660;
static unsigned int WINDOW_HEIGHT = 660;
//static unsigned int WINDOW_WIDTH  = 1200;
//static unsigned int WINDOW_HEIGHT = 800;

const uint8_t GRID_SIZE = 32;

static bool WINDOW_MINIMIZED = false;

float SCALE = 1;

unsigned int ORIGIN_X;
unsigned int ORIGIN_Y;

unsigned int CAMERA_ORIGIN_X;
unsigned int CAMERA_ORIGIN_Y;

unsigned int CAMERA_X = 0;
unsigned int CAMERA_Y = 0;

const unsigned int TARGET_CAMERA_WIDTH  = 15;
const unsigned int TARGET_CAMERA_HEIGHT = 0;

unsigned int CAMERA_WIDTH  = TARGET_CAMERA_WIDTH;
unsigned int CAMERA_HEIGHT = TARGET_CAMERA_HEIGHT;

static const char *WINDOW_TITLE = "Demo";

/**
 * @brief The main window.
 */
SDL_Window *WINDOW;

/**
 * @brief Renderer of the main window.
 */
SDL_Renderer *WINRENDERER;

/**
 * @brief Initialize everything.
 *
 * Initialize SDL and create all necessary images.
 */
bool initialize() {
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not be initialized! SDL_Error: '%s'\n", SDL_GetError());
        return false;
    }

    // Initialize SDL_Image (only png part)
    {
        int init = IMG_Init(IMG_INIT_PNG);
        if ((init & IMG_INIT_PNG) != IMG_INIT_PNG) {
            fprintf(stderr, "Could not initialize SDL_Image with PNG support\n");
            SDL_Quit();
            return false;
        }
    }

    // Create window
    WINDOW = NULL;
    WINDOW = SDL_CreateWindow(WINDOW_TITLE,
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            WINDOW_WIDTH, WINDOW_HEIGHT,
                            SDL_WINDOW_SHOWN |
                            SDL_WINDOW_RESIZABLE);
    if (WINDOW == NULL) {
        fprintf(stderr, "Could not create Window!\n"
                "SDL_Error: '%s'\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Create renderer for window
    WINRENDERER = SDL_CreateRenderer(
                    WINDOW, -1,
                    //SDL_RENDERER_ACCELERATED |
                    SDL_RENDERER_PRESENTVSYNC);
    if (WINRENDERER == NULL) {
        fprintf(stderr, "Could not create renderer for window!\n"
                "SDL_Error: '%s'\n", SDL_GetError());
        SDL_DestroyWindow(WINDOW);
        IMG_Quit();
        SDL_Quit();
        return false;
    }


    // Initialize and decode images
    if(!initialize_images()) {
        SDL_DestroyRenderer(WINRENDERER);
        SDL_DestroyWindow(WINDOW);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    return true;
}

/**
 * @brief Deinitializes everything
 *
 * Initializes SDL and decodes images.
 */
void deinitialize() {
    delete CURRENT_LEVEL;
    deinitialize_images();

    SDL_DestroyRenderer(WINRENDERER);
    SDL_DestroyWindow(WINDOW);

    // Clean up dynamically loaded SDL_Image libraries
    // Deinitialize SDL_Image
    IMG_Quit();

    // Deinitialize SDL
    SDL_Quit();
}

void update_window_title(float fps) {
    if (fps > 99) {
        fps = 99;
    } else if (fps < 0) {
        fps = 0;
    }

    // Put together string
    int len = 0;
    len += strlen(WINDOW_TITLE);   // Title
    len += 1;                      // Space
    len += 1;                      // (
    len += 5;                      // 6 digit float
    len += 1;                      // Space
    len += 3;                      // "fps"
    len += 1;                      // )
    len += 1;                      // NULL
    char *str = (char *)malloc(len);

    sprintf(str, "%s (%05.2f fps)", WINDOW_TITLE, fps);
    SDL_SetWindowTitle(WINDOW, str);

    free(str);
}

void update_camera(int middle_x, int middle_y) {
    int camera_wh = (CAMERA_WIDTH  % 2 == 0) ? (CAMERA_WIDTH  / 2) : (CAMERA_WIDTH  / 2 + 1);
    int camera_hh = (CAMERA_HEIGHT % 2 == 0) ? (CAMERA_HEIGHT / 2) : (CAMERA_HEIGHT / 2 + 1);

    int level_w = CURRENT_LEVEL->get_width();
    int level_h = CURRENT_LEVEL->get_height();

    printf("camera_size: (%d * %d)\n", CAMERA_WIDTH, CAMERA_HEIGHT);
    printf("level_size:  (%d * %d)\n", level_w, level_h);
    printf("middle:      (%d * %d)\n", middle_x, middle_y);
    printf("\n");

    // Set x and y position
    if (middle_x < camera_wh) {
        CAMERA_X = 0;
    } else if (middle_x + camera_wh > level_w) {
        CAMERA_X = level_w - CAMERA_WIDTH;
    } else {
        CAMERA_X = middle_x - camera_wh;
    }

    if (middle_y < camera_hh) {
        CAMERA_Y = 0;
    } else if (middle_y + camera_hh > level_h) {
        CAMERA_Y = level_h - CAMERA_HEIGHT;
    } else {
        CAMERA_Y = middle_y - camera_hh;
    }
}

void initialize_scaler() {
    // TODO: Change scaling calculation

    // We should always show ~40 tiles in width
    //unsigned int TARGET_W_TILES = 40;
    SCALE = 1;
    CAMERA_WIDTH  = TARGET_CAMERA_WIDTH;
    CAMERA_HEIGHT = TARGET_CAMERA_HEIGHT;

    //printf("Want to fit %d tiles (each %dpx => %dpx) horizontally inside window of %dx%d\n",
    //        CAMERA_WIDTH, GRID_SIZE, CAMERA_WIDTH * GRID_SIZE, WINDOW_WIDTH, WINDOW_HEIGHT);

    // If desired amount of tiles dont fit into window width at scaling 1
    printf("%d * %dpx * %d = %d\n", CAMERA_WIDTH, GRID_SIZE, (int)SCALE, CAMERA_WIDTH * GRID_SIZE * (int)SCALE);
    printf("window width: %d\n", WINDOW_WIDTH);
    if (CAMERA_WIDTH * GRID_SIZE * SCALE > WINDOW_WIDTH) {
        // Reduce camera width
        while (CAMERA_WIDTH * GRID_SIZE * SCALE > WINDOW_WIDTH) {
            printf("foo\n");
            CAMERA_WIDTH--;
        }

        // Adjust camera height
        while (CAMERA_HEIGHT * GRID_SIZE * SCALE < WINDOW_HEIGHT) {
            CAMERA_HEIGHT++;
        }
        CAMERA_HEIGHT--;

        return;
    }

    // If desired amount of tiles dont fill the window scale up
    while (CAMERA_WIDTH * GRID_SIZE * SCALE < WINDOW_WIDTH) {
        SCALE++;
    }
    SCALE--;

    // After scaling up to the correct factor we increase camera size
    while (CAMERA_WIDTH * GRID_SIZE * SCALE < WINDOW_WIDTH) {
        CAMERA_WIDTH++;
    }
    CAMERA_WIDTH--;

    while (CAMERA_HEIGHT * GRID_SIZE * SCALE < WINDOW_HEIGHT) {
        CAMERA_HEIGHT++;
    }
    CAMERA_HEIGHT--;

    // If desired amount of tiles dont fit into window size we need to show fewer tiles
    // TODO: If desired amount of tiles dont fit to we scale down or show fewer tiles?
    //while (TARGET_W_TILES * GRID_SIZE > WINDOW_WIDTH) {
    //    printf("  %d tiles (%d x %dpx = %dpx) don't fit into window width (%dpx) showing fewer...\n",
    //            TARGET_W_TILES,
    //            TARGET_W_TILES, GRID_SIZE, TARGET_W_TILES * GRID_SIZE,
    //            WINDOW_WIDTH);
    //    TARGET_W_TILES--;
    //}

    // Calculate scale size
    //{
    //    // Board width and height both have to fit after scaling
    //    float scale1 = WINDOW_WIDTH  / (CURRENT_LEVEL->get_width()  * (float)GRID_SIZE);
    //    float scale2 = WINDOW_HEIGHT / (CURRENT_LEVEL->get_height() * (float)GRID_SIZE);
    //    SCALE = (scale1 > scale2) ? scale2 : scale1;

    //    // TODO: Change scaler to only scale in steps of 1.0

    //    // Only adjust scale in steps of 0.5 so distortion does not get too bad
    //    int r = 0;
    //    while (SCALE >= 0.5) {
    //        SCALE -= 0.5;
    //        r++;
    //    }
    //    SCALE = 0.5f * r;

    //    // Dont scale to zero
    //    if (SCALE < 0.5) {
    //        SCALE = 0.5;
    //    }
    //}

    //printf("  Scaling with %f means level (%dx%d) is %fpx x %fpx\n",
    //        SCALE,
    //        CURRENT_LEVEL->get_width(),
    //        CURRENT_LEVEL->get_height(),
    //        CURRENT_LEVEL->get_width() * GRID_SIZE * SCALE,
    //        CURRENT_LEVEL->get_height() * GRID_SIZE * SCALE);

    // If level is smaller than the camera center it inside camera
    if (CURRENT_LEVEL->get_width() < CAMERA_WIDTH) {
        ORIGIN_X  = WINDOW_WIDTH  - (CAMERA_WIDTH  * GRID_SIZE * SCALE);
        ORIGIN_X /= 2;
    } else {
        ORIGIN_X = 0;
    }

    if (CURRENT_LEVEL->get_height() < CAMERA_HEIGHT) {
        ORIGIN_Y  = WINDOW_HEIGHT - (CAMERA_HEIGHT * GRID_SIZE * SCALE);
        ORIGIN_Y /= 2;
    } else {
        ORIGIN_Y = 0;
    }

}

void update_window_properties() {
    // Check if window has been minimized/unminimized
    if (SDL_GetWindowFlags(WINDOW) & SDL_WINDOW_MINIMIZED) {
        WINDOW_MINIMIZED = true;
    } else {
        WINDOW_MINIMIZED = false;
    }

    // Set correct window size
    SDL_GetWindowSize(WINDOW, (int *)&WINDOW_WIDTH, (int *)&WINDOW_HEIGHT);

    // In case the window size has changed update the scaler
    // TODO: Only update the scaler if window size really changed?
    initialize_scaler();

    // If window size changed the camera position also changed
    update_camera(PLAYER->get_x(), PLAYER->get_y());
}



/**
 * @brief Render the current game status.
 *
 * This calls other functions in order to fill the render buffer
 * with the correct textures and then flips the back buffer.
 *
 * @return True on success. False otherwise.
 */
bool render() {
    // Dont render when the window is minimized
    if (WINDOW_MINIMIZED) {
        return true;
    }

    // Initialize buffer
    SDL_SetRenderDrawColor(WINRENDERER, 0x00, 0x00, 0x00, 0xFF);
    if (SDL_RenderClear(WINRENDERER) != 0) {
        fprintf(stderr, "Could not clear window!\n"
                "SDL_Error: '%s'\n", SDL_GetError());
        return false;
    }

    if (false) {
        // Render menu if requested
    } else {
        // Render game

        // Draw the board
        CURRENT_LEVEL->render();

        // Draw sokoban
        PLAYER->render();
        //draw_soko();
    }

    // Flip buffers
    SDL_RenderPresent(WINRENDERER);

    return true;
}

