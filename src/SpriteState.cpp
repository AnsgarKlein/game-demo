
#include "SpriteState.h"

#include <stdlib.h>
#include <stdio.h>

void SpriteState::init(std::string *str,
                       int frames_c,
                       Point *offsets[],
                       int frame_time,
                       int rotate[],
                       bool mirror_h[],
                       bool mirror_v[]) {

    // TODO: Remove or adjust overlay rendering code
    this->overlay = false;

    // String
    this->str = (str != NULL) ? str : new std::string("default");

    //
    this->frames_c = frames_c;

    // Points
    if (offsets != NULL) {
        this->offsets = offsets;
    } else {
        Point **offsets = new Point*[frames_c];
        for (int i = 0; i < frames_c; i++) {
            offsets[i] = new Point { 0, 0 };
        }
        this->offsets = offsets;
    }

    // Frame time
    this->frame_time = frame_time;

    // Rotate
    if (rotate != NULL) {
        this->rotate = rotate;
    } else {
        this->rotate = new int[frames_c];
        for (int i = 0; i < frames_c; i++) {
            this->rotate[i] = 0;
        }
    }

    // Mirror-H
    if (mirror_h != NULL) {
        this->mirror_h = mirror_h;
    } else {
        this->mirror_h = new bool[frames_c];
        for (int i = 0; i < frames_c; i++) {
            this->mirror_h[i] = false;
        }
    }

    // Mirror-V
    if (mirror_v != NULL) {
        this->mirror_v = mirror_v;
    } else {
        this->mirror_v = new bool[frames_c];
        for (int i = 0; i < frames_c; i++) {
            this->mirror_v[i] = false;
        }
    }
}

SpriteState::SpriteState() {
    init(NULL, 1, NULL, 0, NULL, NULL, NULL);
}

SpriteState::SpriteState(std::string *str, Point *offset) {
    Point **offsets = new Point*[1] { offset };
    init(str, 1, offsets, 0, NULL, NULL, NULL);
}

SpriteState::SpriteState(std::string *str,
                         int frames_c,
                         Point *offsets[],
                         int frame_time) {

    init(str, frames_c, offsets, frame_time, NULL, NULL, NULL);
}

SpriteState::SpriteState(std::string *str,
                         Point *offset,
                         int rotate[],
                         bool mirror_h[],
                         bool mirror_v[]) {

    Point **offsets = new Point*[1] { offset };
    init(str, 1, offsets, 0, rotate, mirror_h, mirror_v);
}

SpriteState::SpriteState(std::string *str,
                         int frames_c,
                         Point *offsets[],
                         int rotate[],
                         bool mirror_h[],
                         bool mirror_v[]) {

    init(str, frames_c, offsets, 0, rotate, mirror_h, mirror_v);
}

SpriteState::SpriteState(std::string *str,
                        int frames_c,
                        Point *offsets[],
                        int frame_time,
                        int rotate[],
                        bool mirror_h[],
                        bool mirror_v[]) {

    init(str, frames_c, offsets, frame_time, rotate, mirror_h, mirror_v);
}

SpriteState::~SpriteState() {
    // Free string
    if (str != NULL) {
        delete str;
        str = NULL;
    }

    // Free all offset information
    if (offsets != NULL) {
        for (int i = 0; i < frames_c; i++) {
            delete offsets[i];
        }
        delete[] offsets;
        offsets = NULL;
    }

    // Free all rotate information
    if (rotate != NULL) {
        delete[] rotate;
        rotate = NULL;
    }

    // Free all mirror-h information
    if (mirror_h != NULL) {
        delete[] mirror_h;
    }

    // Free all mirror-v information
    if (mirror_v != NULL) {
        delete[] mirror_v;
    }
}

const char *SpriteState::get_str() {
    return str->c_str();
}

bool SpriteState::get_overlay() {
    return overlay;
}

int SpriteState::get_frame_time() {
    return this->frame_time;
}

void SpriteState::get_frames(int *frames_c, Point ***offsets) {
    *frames_c = this->frames_c;
    *offsets = this->offsets;
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

SpriteState *SpriteState_from_json(JsonObject *obj) {
    // Constants for parsing
    const std::string JSON_KEY_ID           = "id";
    const std::string JSON_KEY_FRAME_TIME   = "frame_time";
    const std::string JSON_KEY_POINTS       = "points";
    const std::string JSON_KEY_ROTATE       = "rotate";
    const std::string JSON_KEY_MIRROR_H     = "mirror_h";
    const std::string JSON_KEY_MIRROR_V     = "mirror_v";

    // Variables fo parsed values
    std::string          *parsed_id         = NULL;
    int                   parsed_frame_time = 0;
    int                   parsed_frames     = 0;
    std::vector<Point *> *parsed_offsets    = NULL;
    std::vector<int>     *parsed_rotates    = NULL;
    std::vector<bool>    *parsed_mirror_h   = NULL;
    std::vector<bool>    *parsed_mirror_v   = NULL;


    // Go through all children of this object and parse them
    bool parsing_error = false;
    for (auto pair : *(obj->get_children())) {
        std::string *key = pair.first;
        JsonBaseObject *val = pair.second;

        if (parsing_error) {
            break;
        }

        // Parse id key-value pair
        if (*key == JSON_KEY_ID) {
            JsonSimple *id = (JsonSimple *)val;

            if (typeid(*val) != typeid(JsonSimple) ||
                id->get_type() != JSON_OBJECT_STRING ||
                id->get_string() == NULL) {

                std::cerr << "Error when parsing states of SpriteSheet." << std::endl;
                std::cerr << "Key '" << *key << "' has unknown value." << std::endl;
                std::cerr << "Expected value of type to be string - ignoring pair..." << std::endl;
                continue;
            }

            parsed_id = new std::string(*id->get_string());
            continue;
        }

        // Parse frame time key-value pair
        if (*key == JSON_KEY_FRAME_TIME) {
            JsonSimple *frame_time = (JsonSimple *)val;

            if (typeid(*val) != typeid(JsonSimple) ||
                frame_time->get_type() != JSON_OBJECT_INTEGER ||
                frame_time->get_int() == NULL) {

                std::cerr << "Error when parsing states of SpriteSheet." << std::endl;
                std::cerr << "Key '" << *key << "' has unknown value." << std::endl;
                std::cerr << "Expected value of type to be integer - ignoring pair..." << std::endl;
                continue;
            }

            parsed_frame_time = *(frame_time->get_int());
            continue;
        }

        // Parse frame offsets key-value pair
        if (*key == JSON_KEY_POINTS) {
            JsonSimple *offsets = (JsonSimple *)val;

            if (typeid(*val) != typeid(JsonSimple) ||
                offsets->get_type() != JSON_OBJECT_ARRAY ||
                offsets->get_array() == NULL) {

                std::cerr << "Error when parsing states of SpriteSheet." << std::endl;
                std::cerr << "Key '" << *key << "' has unknown value." << std::endl;
                std::cerr << "Expected value of type to be array - ignoring pair..." << std::endl;
                continue;
            }

            // Allocate space for all states
            if (parsed_offsets == NULL) {
                parsed_offsets = new std::vector<Point *>();
            }

            // Go through all offsets and parse them
            for (JsonBaseObject *offset_baseobj : *offsets->get_array()) {
                if (typeid(*offset_baseobj) != typeid(JsonObject)) {
                    std::cerr << "Error when parsing states of SpriteSheet." << std::endl;
                    std::cerr << "Value of key '" << *key << "' should be array of objects" << std::endl;
                    continue;
                }
                JsonObject *offset_obj = (JsonObject *)offset_baseobj;

                // Go through all children of offset object and parse them
                int offset_x;
                int offset_y;
                bool found_x = false;
                bool found_y = false;
                for (auto offset_pos_pair : *offset_obj->get_children()) {
                    std::string *offset_pos_key = offset_pos_pair.first;
                    auto         offset_pos_val_base = offset_pos_pair.second;
                    auto        *offset_pos_val = (JsonSimple *)offset_pos_val_base;

                    if (typeid(*offset_pos_val_base) != typeid(JsonSimple) ||
                        offset_pos_val->get_type() != JSON_OBJECT_INTEGER ||
                        offset_pos_val->get_int() == NULL){

                        std::cerr << "Error when parsing states of SpriteSheet." << std::endl;
                        std::cerr << "Value of key '" << *key << "' should be array of objects" << std::endl;
                        continue;
                    }

                    if (*offset_pos_key == "x") {
                        offset_x = *(offset_pos_val->get_int());
                        found_x = true;
                        continue;
                    }
                    if (*offset_pos_key == "y") {
                        offset_y = *(offset_pos_val->get_int());
                        found_y = true;
                        continue;
                    }
                }

                if (!found_x || !found_y) {
                    std::cerr << "Error when parsing states of SpriteSheet." << std::endl;
                    std::cerr << "Offset of state is malformed (missing x/y coordinate)." << std::endl;
                    continue;
                }

                // Add parsed offset to other parsed offsets
                Point *offset = new Point { offset_x, offset_y };
                parsed_offsets->push_back(offset);
            }

            continue;
        }

        // Parse frame rotate key-value pair
        if (*key == JSON_KEY_ROTATE) {
            JsonSimple *rotates = (JsonSimple *)val;

            if (typeid(*val) != typeid(JsonSimple) ||
                rotates->get_type() != JSON_OBJECT_ARRAY ||
                rotates->get_array() == NULL) {

                std::cerr << "Error when parsing states of SpriteSheet." << std::endl;
                std::cerr << "Key '" << *key << "' has unknown value." << std::endl;
                std::cerr << "Expected value of type to be array - ignoring pair..." << std::endl;
                continue;
            }

            // Allocate space for rotate options
            if (parsed_rotates == NULL) {
                parsed_rotates = new std::vector<int>();
            }

            // Go through all rotate integers and parse them
            for (JsonBaseObject *rotate_baseobj : *rotates->get_array()) {
                JsonSimple *rotate_obj = (JsonSimple *)rotate_baseobj;
                if (typeid(*rotate_baseobj) != typeid(JsonSimple) ||
                    rotate_obj->get_type() != JSON_OBJECT_INTEGER ||
                    rotate_obj->get_int() == NULL) {

                    std::cerr << "Error when parsing states of SpriteSheet." << std::endl;
                    std::cerr << "Key '" << *key << "' has unknown value." << std::endl;
                    std::cerr << "Expected value of type to be array of integers..." << std::endl;
                    continue;
                }

                // Parse rotate option
                // TODO: Change copy code
                int copy = *(rotate_obj->get_int());
                parsed_rotates->push_back(copy);
            }

            continue;
        }

        // Parse frame mirror_h key-value pair
        if (*key == JSON_KEY_MIRROR_H) {
            JsonSimple *mirror_h = (JsonSimple *)val;

            if (typeid(*val) != typeid(JsonSimple) ||
                mirror_h->get_type() != JSON_OBJECT_ARRAY ||
                mirror_h->get_array() == NULL) {

                std::cerr << "Error when parsing states of SpriteSheet." << std::endl;
                std::cerr << "Key '" << *key << "' has unknown value." << std::endl;
                std::cerr << "Expected value of type to be array - ignoring pair..." << std::endl;
                continue;
            }

            // Allocate space for rotate options
            if (parsed_mirror_h == NULL) {
                parsed_mirror_h = new std::vector<bool>();
            }

            // Go through all mirror_h integers and parse them
            for (JsonBaseObject *mirrorh_baseobj : *mirror_h->get_array()) {
                JsonSimple *mirrorh_obj = (JsonSimple *)mirrorh_baseobj;
                if (typeid(*mirrorh_baseobj) != typeid(JsonSimple) ||
                    mirrorh_obj->get_type() != JSON_OBJECT_BOOLEAN ||
                    mirrorh_obj->get_bool() == NULL) {

                    std::cerr << "Error when parsing states of SpriteSheet." << std::endl;
                    std::cerr << "Key '" << *key << "' has unknown value." << std::endl;
                    std::cerr << "Expected value of type to be array of booleans..." << std::endl;
                    continue;
                }

                // Parse mirror option
                // TODO: Change copy code
                bool copy = *(mirrorh_obj->get_bool());
                parsed_mirror_h->push_back(copy);
            }

            continue;
        }

        // Parse frame mirror_v key-value pair
        if (*key == JSON_KEY_MIRROR_V) {
            JsonSimple *mirror_v = (JsonSimple *)val;

            if (typeid(*val) != typeid(JsonSimple) ||
                mirror_v->get_type() != JSON_OBJECT_ARRAY ||
                mirror_v->get_array() == NULL) {

                std::cerr << "Error when parsing states of SpriteSheet." << std::endl;
                std::cerr << "Key '" << *key << "' has unknown value." << std::endl;
                std::cerr << "Expected value of type to be array - ignoring pair..." << std::endl;
                continue;
            }

            // Allocate space for rotate options
            if (parsed_mirror_v == NULL) {
                parsed_mirror_v = new std::vector<bool>();
            }

            // Go through all mirror_v integers and parse them
            for (JsonBaseObject *mirrorv_baseobj : *mirror_v->get_array()) {
                JsonSimple *mirrorv_obj = (JsonSimple *)mirrorv_baseobj;
                if (typeid(*mirrorv_baseobj) != typeid(JsonSimple) ||
                    mirrorv_obj->get_type() != JSON_OBJECT_BOOLEAN ||
                    mirrorv_obj->get_bool() == NULL) {

                    std::cerr << "Error when parsing states of SpriteSheet." << std::endl;
                    std::cerr << "Key '" << *key << "' has unknown value." << std::endl;
                    std::cerr << "Expected value of type to be array of booleans..." << std::endl;
                    continue;
                }

                // Parse mirror option
                // TODO: Change copy code
                bool copy = *(mirrorv_obj->get_bool());
                parsed_mirror_v->push_back(copy);
            }

            continue;
        }
    }
    if (parsed_offsets != NULL) {
        parsed_frames = parsed_offsets->size();
    }

    // Check for errors
    if (parsed_frames == 0 ||
        parsed_id == NULL ||
        parsed_offsets == NULL ||
        parsed_rotates == NULL ||
        parsed_mirror_h == NULL ||
        parsed_mirror_v == NULL) {

        std::cerr << "Error when parsing states of SpriteSheet." << std::endl;
        std::cerr << "Did not find all necessary key-value pairs." << std::endl;
        std::cerr << "Necessary are:" << std::endl;
        std::cerr << "  - " << JSON_KEY_ID << std::endl;
        std::cerr << "  - " << JSON_KEY_FRAME_TIME << std::endl;
        std::cerr << "  - " << JSON_KEY_POINTS << std::endl;
        std::cerr << "  - " << JSON_KEY_ROTATE << std::endl;
        std::cerr << "  - " << JSON_KEY_MIRROR_H << std::endl;
        std::cerr << "  - " << JSON_KEY_MIRROR_V << std::endl;

        parsing_error = true;
    } else if (parsed_mirror_h->size() != (size_t)parsed_frames ||
               parsed_mirror_v->size() != (size_t)parsed_frames ||
               parsed_offsets->size()  != (size_t)parsed_frames) {

        std::cerr << "Error when parsing states of SpriteSheet." << std::endl;
        std::cerr << "Key-value pairs are malformed." << std::endl;
        std::cerr << "Necessary arrays dont have the same length." << std::endl;

        parsing_error = true;
    }

    if (parsing_error) {
        if (parsed_id != NULL) {
            delete parsed_id;
        }

        if (parsed_offsets != NULL) {
            for (Point *p : *parsed_offsets) {
                delete p;
            }
            delete parsed_offsets;
        }

        if (parsed_rotates != NULL) {
            delete parsed_rotates;
        }

        if (parsed_mirror_h != NULL) {
            delete parsed_mirror_h;
        }

        if (parsed_mirror_v != NULL) {
            delete parsed_mirror_v;
        }

        return NULL;
    }

    // Copy content of vectors to arrays
    Point **parsed_offsets_arr  = new Point*[parsed_frames];
    int    *parsed_rotates_arr  = new int[parsed_frames];
    bool   *parsed_mirror_h_arr = new bool[parsed_frames];
    bool   *parsed_mirror_v_arr = new bool[parsed_frames];

    std::copy(parsed_offsets->begin(),  parsed_offsets->end(),  parsed_offsets_arr);
    std::copy(parsed_rotates->begin(),  parsed_rotates->end(),  parsed_rotates_arr);
    std::copy(parsed_mirror_h->begin(), parsed_mirror_h->end(), parsed_mirror_h_arr);
    std::copy(parsed_mirror_v->begin(), parsed_mirror_v->end(), parsed_mirror_v_arr);

    delete parsed_offsets;
    delete parsed_rotates;
    delete parsed_mirror_h;
    delete parsed_mirror_v;

    parsed_offsets  = NULL;
    parsed_rotates  = NULL;
    parsed_mirror_h = NULL;
    parsed_mirror_v = NULL;

    // Create new SpriteState from parsed results
    return new SpriteState(parsed_id,
                           parsed_frames,
                           parsed_offsets_arr,
                           parsed_frame_time,
                           parsed_rotates_arr,
                           parsed_mirror_h_arr,
                           parsed_mirror_v_arr);

}
