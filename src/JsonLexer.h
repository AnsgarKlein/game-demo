#pragma once
#ifndef JSON_LEXER_H
#define JSON_LEXER_H

#include <vector>
#include <string>

enum JsonTokenType {
    JSON_STRING,
    JSON_INTEGER,
    JSON_BOOLEAN,
    JSON_NULL,
    JSON_SPECIAL
};

struct JsonToken {
    void *data;
    enum JsonTokenType type;
};

std::vector<JsonToken> *lex_json(const std::string *text);

void JsonToken_free(JsonToken *token);
void JsonTokens_free(std::vector<JsonToken> *tokens);

#endif
