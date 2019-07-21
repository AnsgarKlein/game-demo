
#include "JsonParser.h"

#include "FileReader.h"

JsonObject::JsonObject() {
    children = new std::unordered_map<std::string *, JsonBaseObject *>();
}

JsonObject::~JsonObject() {
    for (auto pair : *children) {
        std::string *key = pair.first;
        JsonBaseObject *val = pair.second;

        delete key;
        delete val;
    }
    delete children;
}

void JsonObject::add(std::string *key, JsonBaseObject *value) {
    std::pair<std::string *, JsonBaseObject *> pair(key, value);
    children->insert(pair);
}

std::unordered_map<std::string *, JsonBaseObject *> *JsonObject::get_children() {
    return children;
}

std::string *JsonObject::as_string(int indent) {
    std::string *str = new std::string();
    *str = "";

    // Print this object
    *str += "{\n";

    // Print all children
    for (auto pair : *children) {
        std::string *key = pair.first;
        JsonBaseObject *val = pair.second;

        for (int k = 0; k < (indent + 1) * 2; k++) {
            *str += " ";
        }
        *str += *key;
        *str += ": ";

        auto *child_str = val->as_string(indent + 1);
        *str += *(child_str);
        delete child_str;

        *str += "\n";
    }

    // Print end of this object
    for (int i = 0; i < indent * 2; i++) {
        *str += " ";
    }
    *str += "}";

    return str;
}

JsonSimple::JsonSimple(std::string *value) {
    type_id = JSON_OBJECT_STRING;
    data = value;
}

JsonSimple::JsonSimple(int *value) {
    type_id = JSON_OBJECT_INTEGER;
    data = value;
}

JsonSimple::JsonSimple(bool *value) {
    type_id = JSON_OBJECT_BOOLEAN;
    data = value;
}

JsonSimple::JsonSimple() {
    type_id = JSON_OBJECT_NULL;
    data = NULL;
}

JsonSimple::JsonSimple(std::vector<JsonBaseObject *> *array) {
    type_id = JSON_OBJECT_ARRAY;
    data = array;
}

JsonSimple::~JsonSimple() {
    switch(type_id) {
        case JSON_OBJECT_STRING:
            delete (std::string *)data;
            break;
        case JSON_OBJECT_INTEGER:
            delete (int *)data;
            break;
        case JSON_OBJECT_BOOLEAN:
            delete (bool *)data;
            break;
        case JSON_OBJECT_ARRAY:
            std::vector<JsonBaseObject *> *arr;
            arr = (std::vector<JsonBaseObject *> *)data;
            for (auto element : *arr) {
                delete element;
            }
            delete arr;
        default:
            break;
    }
}

std::string *JsonSimple::as_string(int indent) {
    switch(type_id) {
        case JSON_OBJECT_STRING:
            {
                std::string *d = (std::string *)data;
                return new std::string(*d);
            }
            break;
        case JSON_OBJECT_INTEGER:
            {
                int *i = (int *)data;
                std::string *str = new std::string();
                *str = "";
                *str += std::to_string(*i);
                return str;
            }
            break;
        case JSON_OBJECT_BOOLEAN:
            {
                bool *b = (bool *)data;

                std::string *str = new std::string();
                *str = (*b) ? "true" : "false";
                return str;
            }
            break;
        case JSON_OBJECT_NULL:
            {
                std::string *str = new std::string("null");
                return str;
            }
            break;
        case JSON_OBJECT_ARRAY:
            {
                // Print this array
                std::string *str = new std::string("[\n");
                auto *arr = (std::vector<JsonBaseObject *> *)data;

                // Print all members
                for (auto item : *arr) {
                    for (int i = 0; i < (indent + 1) * 2; i++) {
                        *str += " ";
                    }

                    auto item_str = item->as_string(indent + 1);
                    *str += *(item_str);
                    delete item_str;

                    *str += "\n";
                }

                // Print closing of this array
                for (int i = 0; i < indent * 2; i++) {
                    *str += " ";
                }
                *str += "]";
                return str;
            }
            break;
        default:
            return new std::string("<value>");
            break;
    }
}

JsonObjectType JsonSimple::get_type() {
    return type_id;
}

std::string *JsonSimple::get_string() {
    return (type_id == JSON_OBJECT_STRING) ? (std::string *)data : NULL;
}

int *JsonSimple::get_int() {
    return (type_id == JSON_OBJECT_INTEGER) ? (int *)data : NULL;
}

bool *JsonSimple::get_bool() {
    return (type_id == JSON_OBJECT_BOOLEAN) ? (bool *)data : NULL;
}

std::vector<JsonBaseObject *> *JsonSimple::get_array() {
    if (type_id != JSON_OBJECT_ARRAY) {
        return NULL;
    }
    return (std::vector<JsonBaseObject *> *)data;
}

// Forward declarations
static bool parse_array(JsonToken **tokens, JsonToken *last, JsonBaseObject **out);
static bool parse_object(JsonToken **tokens, JsonToken *last, JsonBaseObject **out);
static bool parse_all(JsonToken **tokens, JsonToken *last, JsonBaseObject **out);


static bool parse_array(JsonToken **tokens, JsonToken *last, JsonBaseObject **out) {
    // Check if array is empty
    {
        JsonToken *token = tokens[0];
        if (token->type == JSON_SPECIAL && (*(std::string *)token->data)[0] == ']') {
            (*tokens)++;
            *out = new JsonSimple(new std::vector<JsonBaseObject *>());
            return true;
        }
    }

    auto *arr = new std::vector<JsonBaseObject *>();
    while (*tokens != last) {
        // Read member of this array
        JsonBaseObject *member = NULL;
        if (!parse_all(tokens, last, &member)) {
            delete arr;
            return false;
        }
        arr->push_back(member);

        // Read array separator or array end character: , or ]
        JsonToken *sep_token = tokens[0];
        if (sep_token->type != JSON_SPECIAL) {
            fprintf(stderr, "Error when parsing JSON array:\n"
                    "Expected , or ] in array but found something else\n");
            for (auto element : *arr) {
                delete element;
            }
            delete arr;
            return false;
        }
        const char *sep_data = ((std::string *)sep_token->data)->c_str();
        if (sep_data[0] == ',') {
            // , means more elements will follow
            (*tokens)++;
        } else if (sep_data[0] == ']') {
            // ] means this is the end of this array
            (*tokens)++;
            *out = new JsonSimple(arr);
            return true;
        } else {
            fprintf(stderr, "Error when parsing JSON array:\n"
                    "Expected ',' or ']' but found '%c' instead\n",
                    sep_data[0]);
            for (auto element : *arr) {
                delete element;
            }
            delete arr;
            return false;
        }
    }

    // We scanned the last token but array is not finished
    fprintf(stderr, "Error when parsing JSON array:\n"
            "Unexpected end of array without closing ]\n");
    delete arr;
    return false;
}

static bool parse_object(JsonToken **tokens, JsonToken *last, JsonBaseObject **out) {
    // Check if object is empty
    {
        JsonToken *token = tokens[0];
        if (token->type == JSON_SPECIAL && (*(std::string *)token->data)[0] == '}') {
            (*tokens)++;
            *out = new JsonObject();
            return true;
        }
    }

    // Read all key-value pairs of this object and add it
    JsonObject *json_obj = new JsonObject();
    while (*tokens != last) {
        // Read the json key
        JsonToken *key_token = tokens[0];
        if (key_token->type != JSON_STRING) {
            fprintf(stderr,
                    "Error when parsing JSON key:\n"
                    "Expected string starts with \" but found something else\n");
            delete json_obj;
            return false;
        }
        std::string *json_key = new std::string(*((std::string *)key_token->data));
        if (*tokens == last) {
            break;
        }
        (*tokens)++;

        // Read key-value separator colon
        JsonToken *sep_token = tokens[0];
        if (sep_token->type != JSON_SPECIAL || (*(std::string *)sep_token->data)[0] != ':') {
            fprintf(stderr, "Error when parsing JSON key-value:\n"
                    "Expected colon between key and value but found something else\n");
            delete json_obj;
            delete json_key;
            return false;
        }
        if (*tokens == last) {
            break;
        }
        (*tokens)++;

        // Read json value
        JsonBaseObject *json_val = NULL;
        if (!parse_all(tokens, last, &json_val)) {
            delete json_obj;
            delete json_key;
            return false;
        }

        // Add key and value to this object
        json_obj->add(json_key, json_val);

        // After key-value pair there should either be a comma or a closing bracket
        JsonToken *end_token = tokens[0];
        if (end_token->type != JSON_SPECIAL) {
            fprintf(stderr, "Error when parsing JSON object:\n"
                    "Expected either , or } after key value pair but found something else\n");
            delete json_obj;
            return false;
        }
        const char end_data = (*(std::string *)end_token->data)[0];
        if (end_data == ',') {
            // , means more key-value pairs will follow
            if (*tokens == last) {
                break;
            }
            (*tokens)++;
        } else if (end_data == '}') {
            // } means this is the end of this object
            if (*tokens == last) {
                break;
            }
            (*tokens)++;
            *out = json_obj;
            return true;
        } else {
            // Token has to be either , or }
            fprintf(stderr, "Error when parsing JSON object:\n"
                    "Expected either , or } after key value pair but found '%c' instead.\n",
                    end_data);
            delete json_obj;
            return false;
        }
    }

    // We scanned the last token but object is not finished
    fprintf(stderr, "Error when parsing JSON object:\n"
            "Unexpected end of object without closing }\n");
    delete json_obj;
    return false;
}

static bool parse_all(JsonToken **tokens, JsonToken *last, JsonBaseObject **out) {
    // Check token and decide how to parse it
    JsonToken *token = tokens[0];
    switch (token->type) {
        case JSON_STRING:
            {
                std::string *value = new std::string();
                *value = *(std::string *)token->data;

                *out = new JsonSimple(value);
                (*tokens)++;
                return true;
            }
            break;
        case JSON_INTEGER:
            {
                int *value = new int;
                *value = *(int *)token->data;
                *out = new JsonSimple(value);
                (*tokens)++;
                return true;
            }
            break;
        case JSON_BOOLEAN:
            {
                bool *value = new bool;
                *value = *(bool *)token->data;
                *out = new JsonSimple(value);
                (*tokens)++;
                return true;
            }
            break;
        case JSON_NULL:
            {
                *out = new JsonSimple();
                (*tokens)++;
                return true;
            }
            break;
        case JSON_SPECIAL:
            {
                // Check whether following tokens are array or object
                // and parse them accordingly
                const char c = (*(std::string *)token->data)[0];
                if (c == '{') {
                    (*tokens)++;
                    JsonBaseObject *obj = NULL;
                    if (!parse_object(tokens, last, &obj)) {
                        return false;
                    }
                    *out = obj;
                    return true;
                } else if (c == '[') {
                    (*tokens)++;
                    JsonBaseObject *arr = NULL;
                    if (!parse_array(tokens, last, &arr)) {
                        return false;
                    }
                    *out = arr;
                    return true;
                } else {
                    fprintf(stderr,
                            "Error when parsing start of JSON Array/Object:\n"
                            "Expected [ or { but found '%c'\n", c);
                    return false;
                }
            }
            break;
        default:
            fprintf(stderr, "Error when parsing JSON: Unknown JSON token\n");
            return false;
            break;
    }
}

JsonBaseObject *parse_json(JsonToken *tokens, JsonToken *last) {
    JsonBaseObject *item = NULL;
    parse_all(&tokens, last, &item);

    #ifdef DEBUG_JSON_PARSER
    // Check for success and print result
    if (item != NULL) {
        std::string *text = item->as_string();

        std::cout << "JSON representation:" << std::endl;
        std::cout << *text << std::endl;

        delete text;
    }
    #endif

    return item;
}

JsonBaseObject *parse_json(std::string *content) {
    // Lex content first
    std::vector<JsonToken> *tokens = lex_json(content);
    if (tokens == NULL) {
        return NULL;
    }

    // Parse
    JsonToken last = (*tokens)[tokens->size()];
    JsonBaseObject *result = parse_json(tokens->data(), &last);
    JsonTokens_free(tokens);

    return result;
}

