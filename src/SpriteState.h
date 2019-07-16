#pragma once
#ifndef SPRITE_STATE_H
#define SPRITE_STATE_H

#include "Point.h"
#include "RenderType.h"

class SpriteState {
    private:
        const char *str;

        // TODO: Overlay is not really used and needs to change
        // Will be needed for example:
        //    - Wall overlay permutation
        //    - Box shiny overlay
        //    - Overlay of clothes on characters
        // Probably needs to specify on which frame to overlay not just boolean
        bool overlay;

        //
        int frames_c;                   // How many frames are there in this state

        // TODO: Use arrays instead of pointers for frame properties
        Point **frames_v;               // The offset(s) of the frame(s) in the sprite sheet

        // TODO: Use array of frame times instead of just one int limiting flexibility
        int frame_time;                 // How long to show each frame (animation only)
        int *rotate;                    // Whether (and how) to rotate specific frames
        bool *mirror_h;                 // Whether to flip specific frames horizontally
        bool *mirror_v;                 // Whether to flip specific frames vertically
        enum RenderType render_type;    // Whether to animate, permutate, ...

        // TODO: Create private init function for SpriteState to aggregate constructors
    public:
        // Default
        SpriteState();

        // Single frame not at default position
        SpriteState(const char *str, Point *frame);

        // Multiple animated frames
        SpriteState(const char *str, int frame_time, int frames_c, Point **frames_v);
        virtual ~SpriteState();

        const char *get_str();
        bool get_overlay();
        int get_frame_time();
        void get_frames(int *frames_c, Point ***frames_v);

        #ifdef DEBUG_SIZE
        long recursive_size();
        #endif
};

#endif
