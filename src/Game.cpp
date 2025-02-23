
#include "Game.h"

#include "Player.h"

/**
 * @brief Time that has passed (in milliseconds).
 *
 * Time that has passed (in milliseconds) since the start of the game
 * or the last overflow of this value.
 *
 * Should be used for calculating the duration of animations.
 *
 */
uint16_t GAME_TIME = 0;

static const int TARGET_FPS = 30;

/**
 * @brief Handles key press.
 *
 * Gets a key press and tries to move Sokoban in the
 * corresponding direction.
 *
 * @param key The key to handle
 */
static void do_command(SDL_Keycode key) {
    int x = 0;
    int y = 0;

    switch (key) {
        case SDLK_k:
        case SDLK_UP:
            y--;
            break;
        case SDLK_j:
        case SDLK_DOWN:
            y++;
            break;
        case SDLK_h:
        case SDLK_LEFT:
            x--;
            break;
        case SDLK_l:
        case SDLK_RIGHT:
            x++;
            break;
        default:
            return;
            break;
    }

    PLAYER->move(x, y);
}

void game_loop() {
    bool quit = false;

    // Game timer keeps track of time
    struct Timer game_timer;
    Timer_start(&game_timer);

    // Frame timer keeps track how much time a single frame took
    struct Timer frame_timer;

    // FPS timer keeps track of current fps
    uint8_t rendered_frames = 0;
    struct Timer fps_timer;
    Timer_start(&fps_timer);

    // Loop
    SDL_Event e;
    while (!quit) {
        Timer_start(&frame_timer);

        // Process event queue (block for at most 100ms)
        //if (SDL_WaitEventTimeout(&e, 10) != 0) {

        while (SDL_PollEvent(&e) != 0) {
            // Check which type of event happened
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_WINDOWEVENT) {
                switch(e.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        quit = true;
                        break;
                    case SDL_WINDOWEVENT_MINIMIZED:
                    case SDL_WINDOWEVENT_RESTORED:
                    case SDL_WINDOWEVENT_HIDDEN:
                    case SDL_WINDOWEVENT_SHOWN:
                    case SDL_WINDOWEVENT_RESIZED:
                        update_window_properties();
                        break;
                    default:
                        break;
                }
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_q:
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    default:
                        do_command(e.key.keysym.sym);
                        break;
                }
            }
        }

        // Calculate fps
        float fps = 0;
        {
            uint16_t ticks = Timer_get_ticks(&fps_timer);

            // Dividing by zero is bad
            if (ticks == 0) {
                ticks = 1;
            }
            fps = rendered_frames / (ticks / 1000.0f);

            // Restart fps timer every few seconds
            if (ticks > 5000) {
                Timer_stop(&fps_timer);
                Timer_start(&fps_timer);
                rendered_frames = 0;
            }
        }
        update_window_title(fps);

        // Wait to get desired fps
        {
            uint8_t desired_frame_time = 1000 / TARGET_FPS;
            uint8_t frame_time_elapsed = Timer_get_ticks(&frame_timer);
            if (desired_frame_time > frame_time_elapsed) {
                SDL_Delay(desired_frame_time - frame_time_elapsed);
            }
        }

        // Update current game time
        GAME_TIME = Timer_get_ticks(&game_timer);

        // Refresh the screen
        render();
        rendered_frames++;
    }

    // Game loop has stopped, render one last time
    render();
    SDL_Delay(100);
}

