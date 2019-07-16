
#include "SpriteState.h"

#include <stdlib.h>
#include <stdio.h>

SpriteState::SpriteState() {
    this->str = "default";
    this->overlay = false;
    this->frame_time = 0;
    this->frames_c = 1;
    this->frames_v = (Point **)malloc(sizeof(Point *) * 1);
    this->frames_v[0] = new Point { 0, 0 };
}

SpriteState::SpriteState(const char *str, Point *frame) {
    this->str = str;
    this->overlay = false;
    this->frame_time = 0;
    this->frames_c = 1;
    this->frames_v = (Point **)malloc(sizeof(Point *) * 1);
    this->frames_v[0] = frame;
}

SpriteState::SpriteState(const char *str, int frame_time, int frames_c, Point **frames_v) {
    this->str = str;
    this->overlay = false;
    this->frame_time = frame_time;
    this->frames_c = frames_c;
    this->frames_v = frames_v;
}

SpriteState::~SpriteState() {
    if (frames_v != NULL) {
        for (int i = 0; i < frames_c; i++) {
            delete frames_v[i];
        }
        free(frames_v);
        frames_v = NULL;
    }
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

#ifdef DEBUG_SIZE
long SpriteState::recursive_size() {
    long size = 0;

    // This
    size += sizeof(*this);

    // String
    size += sizeof(*str) * strlen(str);

    // Frames
    for (int i = 0; i < frames_c; i++) {
        size += sizeof(frames_v[i]);
        size += sizeof(*frames_v[i]);
    }

    printf("Size of SpriteState %s is %ld\n", get_str(), size);
    return size;
}
#endif

