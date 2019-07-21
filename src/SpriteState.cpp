
#include "SpriteState.h"

#include <stdlib.h>
#include <stdio.h>


void SpriteState::init(const char *str,
                       int frames_c,
                       Point *frames_v[],
                       int frame_time,
                       int rotate[],
                       bool mirror_h[],
                       bool mirror_v[]) {

    // TODO: Remove or adjust overlay rendering code
    this->overlay = false;

    // String
    if (str != NULL) {
        this->str = str;
    } else {
        this->str = "default";
    }

    //
    this->frames_c = frames_c;

    // Points
    if (frames_v != NULL) {
        this->frames_v = frames_v;
    } else {
        Point **points = (Point **)malloc(sizeof(Point *) * frames_c);
        for (int i = 0; i < frames_c; i++) {
            points[i] = new Point { 0, 0 };
        }
        this->frames_v = points;
    }

    this->frame_time = frame_time;

    // Rotate
    if (rotate != NULL) {
        this->rotate = rotate;
    } else {
        int *rotate = (int *)malloc(sizeof(int) * frames_c);
        for (int i = 0; i < frames_c; i++) {
            rotate[i] = 0;
        }
        this->rotate = rotate;
    }

    // Mirror-H
    if (mirror_h != NULL) {
        this->mirror_h = mirror_h;
    } else {
        bool *mirror_h = (bool *)malloc(sizeof(bool) * frames_c);
        for (int i = 0; i < frames_c; i++) {
            mirror_h[i] = false;
        }
        this->mirror_h = mirror_h;
    }

    // Mirror_V
    if (mirror_v != NULL) {
        this->mirror_v = mirror_v;
    } else {
        bool *mirror_v = (bool *)malloc(sizeof(bool) * frames_c);
        for (int i= 0; i< frames_c; i++) {
            mirror_v[i] = false;
        }
        this->mirror_v = mirror_v;
    }
}

SpriteState::SpriteState() {
    init(NULL, 1, NULL, 0, NULL, NULL, NULL);
}

SpriteState::SpriteState(const char *str,  Point *frame) {
    Point **frames = (Point **)malloc(sizeof(Point *) * 1);
    frames[0] = frame;
    init(str, 1, frames, 0, NULL, NULL, NULL);
}

SpriteState::SpriteState(const char *str,
                         int frames_c,
                         Point *frames_v[],
                         int frame_time) {
    init(str, frames_c, frames_v, frame_time, NULL, NULL, NULL);
}

SpriteState::SpriteState(const char *str,
                         Point *frame,
                         int rotate[],
                         bool mirror_h[],
                         bool mirror_v[]) {
    Point **frames = (Point **)malloc(sizeof(Point *) * 1);
    frames[0] = frame;
    init(str, 1, frames, 0, rotate, mirror_h, mirror_v);
}

SpriteState::SpriteState(const char *str,
                         int frames_c,
                         Point *frames_v[],
                         int rotate[],
                         bool mirror_h[],
                         bool mirror_v[]) {
    init(str, frames_c, frames_v, 0, rotate, mirror_h, mirror_v);
}

SpriteState::SpriteState(const char *str,
                        int frames_c,
                        Point *frames_v[],
                        int frame_time,
                        int rotate[],
                        bool mirror_h[],
                        bool mirror_v[]) {
    init(str, frames_c, frames_v, frame_time, rotate, mirror_h, mirror_v);
}

SpriteState::~SpriteState() {
    // Free all frames
    if (frames_v != NULL) {
        for (int i = 0; i < frames_c; i++) {
            delete frames_v[i];
        }
        free(frames_v);
        frames_v = NULL;
    }

    //
    if (rotate != NULL) {
        free(rotate);
    }
    rotate = NULL;

    if (mirror_h != NULL) {
        free(mirror_h);
    }
    mirror_h = NULL;

    if (mirror_v != NULL) {
        free(mirror_v);
    }
    mirror_v = NULL;

}

const char *SpriteState::get_str() {
    if (str == NULL) {
        printf("string is NULL for some reason...\n");
    }
    return str;
}

bool SpriteState::get_overlay() {
    return overlay;
}

int SpriteState::get_frame_time() {
    return this->frame_time;
}

void SpriteState::get_frames(int *frames_c, Point ***frames_v) {
    *frames_c = this->frames_c;
    *frames_v = this->frames_v;
}

int *SpriteState::get_rotate() {
    return this->rotate;
}

bool *SpriteState::get_mirror_h() {
    return this->mirror_h;
}

bool *SpriteState::get_mirror_v() {
    return this->mirror_v;
}

