#pragma once
#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "JsonLexer.h"

enum JsonObjectType {
    JSON_OBJECT_STRING,
    JSON_OBJECT_INTEGER,
    JSON_OBJECT_BOOLEAN,
    JSON_OBJECT_NULL,
    JSON_OBJECT_ARRAY
};

class JsonBaseObject {
    public:
        virtual std::string *as_string(int indent) = 0;
        virtual std::string *as_string() {
            return as_string(0);
        }
        virtual ~JsonBaseObject() { };
};

class JsonObject : public JsonBaseObject {
    private:
        std::unordered_map<std::string *, JsonBaseObject *> *children;
    public:
        JsonObject();
        ~JsonObject();
        void add(std::string *key, JsonBaseObject *value);
        std::unordered_map<std::string *, JsonBaseObject *> *get_children();
        std::string *as_string(int indent);
        std::string *as_string() {
            return as_string(0);
        }
};

class JsonSimple : public JsonBaseObject {
    private:
        enum JsonObjectType type_id;
        void *data;
    public:
        JsonSimple(std::string *value);
        JsonSimple(int *value);
        JsonSimple(bool *value);
        JsonSimple();
        JsonSimple(std::vector<JsonBaseObject *> *array);
        ~JsonSimple();
        std::string *as_string(int indent);
        std::string *as_string() {
            return as_string(0);
        }
        JsonObjectType get_type();
        std::string *get_string();
        int *get_int();
        bool *get_bool();
        std::vector<JsonBaseObject *> *get_array();
};

JsonBaseObject *parse_json(JsonToken *tokens, JsonToken *last);
JsonBaseObject *parse_json(std::vector<JsonToken> *tokens);
JsonBaseObject *parse_json(std::string *content);

#endif
