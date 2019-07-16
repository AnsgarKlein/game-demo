#pragma once
#ifndef VIEW_H
#define VIEW_H

#include "stdbool.h"

#include <SDL.h>


extern SDL_Window   *WINDOW;
extern SDL_Renderer *WINRENDERER;

extern const uint8_t GRID_SIZE;

extern float SCALE;

extern unsigned int ORIGIN_X;
extern unsigned int ORIGIN_Y;

extern unsigned int CAMERA_ORIGIN_X;
extern unsigned int CAMERA_ORIGIN_Y;

extern unsigned int CAMERA_X;
extern unsigned int CAMERA_Y;

extern unsigned int CAMERA_WIDTH;
extern unsigned int CAMERA_HEIGHT;


bool initialize();

void deinitialize();

void update_window_title(float fps);

void update_camera(int middle_x, int middle_y);

//void initialize_scaler();

void update_window_properties();

bool render();

#endif
