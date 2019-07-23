#pragma once
#ifndef SPRITE_STATE_H
#define SPRITE_STATE_H

#include "Point.h"
#include "RenderType.h"
#include "JsonParser.h"

class SpriteState {
    private:
        std::string *id;

        // TODO: Overlay is not really used and needs to change
        // Will be needed for example:
        //    - Wall overlay permutation
        //    - Box shiny overlay
        //    - Overlay of clothes on characters
        // Probably needs to specify on which frame to overlay not just boolean
        bool overlay;

        int frames_c;                   // How many frames are there in this state
        enum RenderType render_type;    // Whether to animate, permutate, ... these frames
        Point **offsets;                // The offset(s) of the frame(s) in the sprite sheet

        // TODO: Use array of frame times instead of just one int limiting flexibility
        int frame_time;                 // How long to show each frame (animation only)

        int *rotate;                    // Whether (and how) to rotate specific frames
        bool *mirror_h;                 // Whether to flip specific frames horizontally
        bool *mirror_v;                 // Whether to flip specific frames vertically

        // Private internal constructor
        void init(std::string *id,
                  int frames_c,
                  enum RenderType render_type,
                  Point *offsets[],
                  int frame_time,
                  int rotate[],
                  bool mirror_h[],
                  bool mirror_v[]);
    public:
        // Default single sprite
        SpriteState();

        // Single sprite
        SpriteState(std::string *id,
                    Point *offset,
                    int rotate,
                    bool mirror_h,
                    bool mirror_v);

        // Multiple sprites
        SpriteState(std::string *id,
                    int frames_c,
                    enum RenderType render_type,
                    Point *offsets[],
                    int frame_time,
                    int rotate[],
                    bool mirror_h[],
                    bool mirror_v[]);

        virtual ~SpriteState();

        const char *get_id();
        bool get_overlay();
        enum RenderType get_render_type();
        Point **get_offsets();
        int get_frame_time();
        int get_sprite_count();
        int *get_rotate();
        bool *get_mirror_h();
        bool *get_mirror_v();
};

SpriteState *SpriteState_from_json(JsonObject *obj);

#endif
