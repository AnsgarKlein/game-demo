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

        // Private internal constructor
        void init(const char *str,
                  int frames_c,
                  Point *frames_v[],
                  int frame_time,
                  int rotate[],
                  bool mirror_h[],
                  bool mirror_v[]);
    public:
        // Default
        SpriteState();

        // Single frame not at default position
        SpriteState(const char *str,
                    Point *frame);

        // Multiple frames, animated
        SpriteState(const char *str,
                    int frames_c,
                    Point *frames_v[],
                    int frame_time);

        // Single frame, rotated / mirrored
        SpriteState(const char *str,
                    Point *frame,
                    int rotate[],
                    bool mirror_h[],
                    bool mirror_v[]);

        // Multiple frames, rotated / mirrored
        SpriteState(const char *str,
                    int frames_c,
                    Point *frames_v[],
                    int rotate[],
                    bool mirror_h[],
                    bool mirror_v[]);

        // All options
        SpriteState(const char *str,
                    int frames_c,
                    Point *frames_v[],
                    int frame_time,
                    int rotate[],
                    bool mirror_h[],
                    bool mirror_v[]);

        virtual ~SpriteState();

        const char *get_str();
        bool get_overlay();
        void get_frames(int *frames_c, Point ***frames_v);
        int get_frame_time();
        int *get_rotate();
        bool *get_mirror_h();
        bool *get_mirror_v();
};

#endif
