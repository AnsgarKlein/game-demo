
#include "View.h"

#include "SpriteHandler.h"
#include "Level.h"
#include "Game.h"
#include "Player.h"
#include "Direction.h"

// TODO: Remove asserts
#include "assert.h"

//Screen dimension
//static unsigned int WINDOW_WIDTH  = 660;
//static unsigned int WINDOW_HEIGHT = 660;
static unsigned int WINDOW_WIDTH  = 1200;
static unsigned int WINDOW_HEIGHT = 800;

const uint8_t GRID_SIZE = 32;

static bool WINDOW_MINIMIZED = false;

float SCALE = 1;

unsigned int ORIGIN_X = 0;
unsigned int ORIGIN_Y = 0;

unsigned int CAMERA_ORIGIN_X;
unsigned int CAMERA_ORIGIN_Y;

unsigned int CAMERA_X = 0;
unsigned int CAMERA_Y = 0;

const unsigned int TARGET_CAMERA_WIDTH  = 0;
const unsigned int TARGET_CAMERA_HEIGHT = 18 * GRID_SIZE;

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
    const int camera_wh = CAMERA_WIDTH  / 2;
    const int camera_hh = CAMERA_HEIGHT / 2;

    const int level_w = CURRENT_LEVEL->get_width()  * GRID_SIZE;
    const int level_h = CURRENT_LEVEL->get_height() * GRID_SIZE;

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

    //printf("level_size:    (%d * %d)\n", level_w, level_h);
    //printf("camera_size:   (%d * %d)\n", CAMERA_WIDTH, CAMERA_HEIGHT);
    //printf("camera_pos:    (%d , %d)\n", CAMERA_X, CAMERA_Y);
    //printf("camera_middle: (%d * %d)\n", middle_x, middle_y);
    //printf("\n");
}

void update_scaler() {
    SCALE = 1;
    CAMERA_WIDTH  = TARGET_CAMERA_WIDTH;
    CAMERA_HEIGHT = TARGET_CAMERA_HEIGHT;

    // Check if
    // A) Desired amount of tiles dont fit into window height at scaling 1
    // B) Desired amount of tiles fit into window height but dont fill the window
    if (CAMERA_HEIGHT * SCALE > WINDOW_HEIGHT) {
        // Reduce camera height
        while (CAMERA_HEIGHT * SCALE > WINDOW_HEIGHT) {
            CAMERA_HEIGHT--;
        }

        // Adjust camera width accordingly
        while (CAMERA_WIDTH * SCALE < WINDOW_WIDTH) {
            CAMERA_WIDTH++;
        }
        CAMERA_WIDTH--;
    } else {
        // TODO: Replace loops with calculations

        // Calculate the maximum naive scaling we can apply
        SCALE = WINDOW_HEIGHT / CAMERA_HEIGHT;

        //printf("\nMaximum naive scaling is %dX\n", (int)SCALE);

        // If desired amount of tiles don't fill the window at maximum scaling
        // there are two possible ways to handle this.
        //   1) Increase camera size until window is filled.
        //   2) Increase scaling by one more and decrease camera size
        //      until window is filled.

        // 1)
        unsigned int camera_height1 = CAMERA_HEIGHT;
        int scale1 = SCALE;
        while (camera_height1 * scale1 < WINDOW_HEIGHT) {
            camera_height1++;
        }
        camera_height1--;

        // 2)
        unsigned int camera_height2 = CAMERA_HEIGHT;
        int scale2 = SCALE + 1;
        while (camera_height2 * scale2 > WINDOW_HEIGHT) {
            camera_height2--;
        }

        //printf("Want to have a height of %dpx (%d fields)\n", TARGET_CAMERA_HEIGHT, TARGET_CAMERA_HEIGHT / GRID_SIZE);
        //printf("Possibility 1: scale @%dX with height %d (%d fields)\n", scale1, camera_height1, camera_height1 / GRID_SIZE);
        //printf("Possibility 2: scale @%dX with height %d (%d fields)\n", scale2, camera_height2, camera_height2 / GRID_SIZE);

        // Decide which way has the lower difference to the target height
        int diff1 = (int)TARGET_CAMERA_HEIGHT - (int)camera_height1;
        int diff2 = (int)TARGET_CAMERA_HEIGHT - (int)camera_height2;
        if (diff1 < 0) diff1 *= -1;
        if (diff2 < 0) diff2 *= -1;
        if (diff1 < diff2) {
            //printf("Deciding for path 1\n");
            SCALE = scale1;
            CAMERA_HEIGHT = camera_height1;
        } else {
            //printf("Deciding for path 2\n");
            SCALE = scale2;
            CAMERA_HEIGHT = camera_height2;
        }

        // Adjust width accordingly to fill camera width
        while (CAMERA_WIDTH * SCALE < WINDOW_WIDTH) {
            CAMERA_WIDTH++;
        }
        CAMERA_WIDTH--;

        //printf("Camera has size %d * %d (%d*%d) (scaling @%dX)\n",
        //       CAMERA_WIDTH, CAMERA_HEIGHT,
        //       CAMERA_WIDTH / GRID_SIZE,
        //       CAMERA_HEIGHT / GRID_SIZE,
        //       (int)SCALE);
    }

    // TODO: Create own function for calculating ORIGIN
    // If level is smaller than the camera center it inside camera
    //if (CURRENT_LEVEL->get_width() < CAMERA_WIDTH) {
    //    ORIGIN_X  = WINDOW_WIDTH  - (CAMERA_WIDTH  * SCALE);
    //    ORIGIN_X /= 2;
    //} else {
    //    ORIGIN_X = 0;
    //}

    //if (CURRENT_LEVEL->get_height() < CAMERA_HEIGHT) {
    //    ORIGIN_Y  = WINDOW_HEIGHT - (CAMERA_HEIGHT * SCALE);
    //    ORIGIN_Y /= 2;
    //} else {
    //    ORIGIN_Y = 0;
    //}

    //ORIGIN_X  = WINDOW_WIDTH  - (CAMERA_WIDTH  * SCALE);
    //ORIGIN_X /= 2;
    //ORIGIN_Y  = WINDOW_HEIGHT - (CAMERA_HEIGHT * SCALE);
    //ORIGIN_Y /= 2;

    ORIGIN_X = 0;
    ORIGIN_Y = 0;

    //printf("ORIGIN: (%d, %d)\n", ORIGIN_X, ORIGIN_Y);
}

// TODO: Rename this function and only call it on window change
void update_window_properties() {
    // Check if window has been minimized/unminimized and update state
    if (SDL_GetWindowFlags(WINDOW) & SDL_WINDOW_MINIMIZED) {
        WINDOW_MINIMIZED = true;
    } else {
        WINDOW_MINIMIZED = false;
    }

    // Set correct window size
    SDL_GetWindowSize(WINDOW, (int *)&WINDOW_WIDTH, (int *)&WINDOW_HEIGHT);

    // In case the window size has changed update the scaler
    update_scaler();

    // If window size changed the camera position also changed
    update_camera(PLAYER->get_x() * GRID_SIZE, PLAYER->get_y() * GRID_SIZE);

    // If window size changed the origin position
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

