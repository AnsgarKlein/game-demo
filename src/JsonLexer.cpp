
#include "JsonLexer.h"

#include <string.h>
#include <stdio.h>
#include <limits.h>

static inline bool lex_special(const char **input, std::vector<JsonToken> *tokens) {
    bool found = false;

    if (*input[0] == '[' || *input[0] == ']') {
        found = true;
    } else if (*input[0] == '{' || *input[0] == '}') {
        found = true;
    } else if (*input[0] == ':' || *input[0] == ',') {
        found = true;
    }

    if (!found) {
        return false;
    }

    std::string *special_pointer = new std::string();
    *special_pointer = *input[0];

    JsonToken token = { special_pointer, JSON_SPECIAL };
    tokens->push_back(token);

    (*input)++;
    return true;
}

static inline bool lex_null(const char **input, std::vector<JsonToken> *tokens) {
    const char *json_null = "null";
    const size_t json_null_len = strlen(json_null);

    if (strlen(*input) > json_null_len) {
        if (strncmp(*input, json_null, json_null_len) == 0) {
            *input += json_null_len;

            JsonToken token = { NULL, JSON_NULL };
            tokens->push_back(token);
            return true;
        }
    }

    // Did not find null
    return false;
}

static inline bool lex_boolean(const char **input, std::vector<JsonToken> *tokens) {
    // Check if there is true boolean
    const char* json_true = "true";
    const size_t json_true_len = strlen(json_true);
    if (strlen(*input) > json_true_len) {
        if (strncmp(*input, json_true, json_true_len) == 0) {
            // Found boolean
            (*input) += json_true_len;

            // Add token
            bool *bool_pointer = new bool;
            *bool_pointer = true;
            JsonToken token = { bool_pointer, JSON_BOOLEAN };
            tokens->push_back(token);
            return true;
        }
    }

    // Check if there is false boolean
    const char* json_false = "false";
    const size_t json_false_len = strlen(json_false);
    if (strlen(*input) > json_false_len) {
        if (strncmp(*input, json_false, json_false_len) == 0) {
            // Found boolean
            (*input) += json_false_len;

            // Add token
            bool *bool_pointer = new bool;
            *bool_pointer = false;
            JsonToken token = { bool_pointer, JSON_BOOLEAN };
            tokens->push_back(token);
            return true;
        }
    }

    // Did not find boolean value
    return false;
}

static inline bool lex_number(const char **input, bool* error, std::vector<JsonToken> *tokens) {
    // Keep track if number is a float or an int
    bool is_float = false;
    bool is_signed = false;

    // If first char is a digit keep adding digits to number
    std::string json_number = "";
    while (strlen(*input) != 0) {
        char c = (*input)[0];
        // Legal chars for numbers are digits 0-9 minus sign and period
        if ((c > 0x2F && c < 0x3A) || c == '.' || c == '-' || c == 'e') {
            // Add
            json_number += c;
            (*input)++;

            // Check if number is floating point
            if (c == '.') {
                if (is_float) {
                    // Error: Number contains two periods
                    fprintf(stderr,
                            "Error when lexing JSON:\n"
                            "Found enexpected char %c when parsing number %s\n",
                            c, json_number.c_str());
                    *error = true;
                    return false;
                } else {
                    is_float = true;
                }
            }

            // Check if number is signed
            if (c == '-') {
                if (is_signed) {
                    // Error: Number contains two minus signs
                    fprintf(stderr,
                            "Error when lexing JSON:\n"
                            "Found unexpected char %c when parsing number %s\n",
                            c, json_number.c_str());
                    *error = true;
                    return false;
                } else {
                    is_signed = true;
                }
            }

            // Check if number is in scientific e-Notation
            if (c == 'e') {
                fprintf(stderr, "Error when lexing JSON:\n"
                        "Found unexpected char %c when parsing number %s.\n"
                        "Scientific e notation is not supported.\n",
                        c, json_number.c_str());
                *error = true;
                return false;
            }
        } else {
            // Char is not legal number char -> indicates end of number
            break;
        }
    }

    if (strlen(json_number.c_str()) == 0) {
        // Did not find a number
        return false;
    }

    int inum;
    if (is_float) {
        double dnum = atof(json_number.c_str());

        // Try to convert to int
        inum = (int)dnum;
        if (dnum - inum != 0) {
            fprintf(stderr, "Error when lexing JSON:\n"
                    "Cannot convert floating point number %f to integer.\n"
                    "Floating point numbers are not supported.\n", dnum);
            *error = true;
            return false;
        }
    } else {
        long lnum = atol(json_number.c_str());
        if (lnum > INT_MAX) {
            fprintf(stderr, "Error when lexing JSON:\n"
                    "Cannot convert number %ld to integer - Number is too big\n",
                    lnum);
            *error = true;
            return false;
        }
        inum = (int)lnum;
    }

    int *num_pointer = new int;
    *num_pointer = inum;
    JsonToken token = { num_pointer, JSON_INTEGER };
    tokens->push_back(token);
    return true;
}

static inline bool lex_string(const char **input, bool *error, std::vector<JsonToken> *tokens) {
    const char JSON_QUOTE = '"';

    // If first character is not a quote input is not a string
    if ((*input)[0] != JSON_QUOTE) {
        return false;
    } else {
        // Remove starting quote from input
        (*input)++;
    }

    // Everything until the closing quote is part of json string
    std::string *json_str = new std::string();
    while (strlen(*input) != 0) {
        char c = (*input)[0];

        if (c == JSON_QUOTE) {
            // Found closing quote -> string is complete
            (*input)++;

            // Add token
            JsonToken token = { json_str, JSON_STRING };
            tokens->push_back(token);
            return true;
        } else {
            // Add character to string
            json_str->push_back(c);
            (*input)++;
        }
    }

    // Error: Did not find a closing quote
    fprintf(stderr, "Error when lexing JSON:\n"
            "Unexpected end of input while lexing string: '%s'.\n",
            json_str->c_str());
    delete json_str;
    *error = true;
    return false;
}

static inline bool lex_all(const char *input, std::vector<JsonToken> *tokens) {
    const char *data = input;

    while (strlen(data) != 0) {
        // Check if there is a string
        {
            bool error = false;
            bool added = lex_string(&data, &error, tokens);

            if (error) {
                return false;
            }

            if (added) {
                continue;
            }
        }

        // Check if there is a number
        {
            bool error = false;
            bool added = lex_number(&data, &error, tokens);

            if (error) {
                return false;
            }

            if (added) {
                continue;
            }
        }

        // Check if there is a boolean
        if (lex_boolean(&data, tokens)) {
            continue;
        }

        // Check if there is a NULL value
        if (lex_null(&data, tokens)) {
            continue;
        }

        // Check if there is a syntax char
        if (lex_special(&data, tokens)) {
            continue;
        }

        // Skip whitespaces
        if (data[0] == ' ' || data[0] == '\t' || data[0] == '\n' || data [0] == '\r') {
            data++;
            continue;
        }

        fprintf(stderr, "Error when parsing JSON:\n"
                "Found unexpected character '%c'\n", data[0]);
        return false;
    }

    return true;
}

std::vector<JsonToken> *lex_json(const std::string *text) {
    const char *input = text->c_str();
    std::vector<JsonToken> *tokens = new std::vector<JsonToken>;

    if (!lex_all(input, tokens)) {
        JsonTokens_free(tokens);
        return NULL;
    }

    #ifdef DEBUG_JSON_PARSER
    printf("Lexing got %d tokens\n", (int)tokens->size());
    for (size_t i = 0; i < tokens->size(); i++) {
        JsonToken t = (*tokens)[i];
        switch (t.type) {
            case JSON_STRING:
                printf("[%03d]   string: '%s'\n", (int)i, ((std::string *)t.data)->c_str());
                break;
            case JSON_INTEGER:
                printf("[%03d]  integer: '%d'\n", (int)i, *(int *)t.data);
                break;
            case JSON_BOOLEAN:
                printf("[%03d]  boolean: '%s'\n", (int)i, *(bool *)t.data == true ? "true" : "false");
                break;
            case JSON_NULL:
                printf("[%03d]     null\n", (int)i);
                break;
            case JSON_SPECIAL:
                printf("[%03d]  special: '%c'\n", (int)i, (*(std::string *)t.data)[0]);
                break;
            default:
                printf("[%03d]\n", (int)i);
                break;
        }
    }
    #endif

    return tokens;
}

void JsonToken_free(JsonToken *token) {
    switch (token->type) {
        case JSON_STRING:
            delete (std::string *)token->data;
            break;
        case JSON_INTEGER:
            delete (int *)token->data;
            break;
        case JSON_BOOLEAN:
            delete (bool *)token->data;
            break;
        case JSON_NULL:
            break;
        case JSON_SPECIAL:
            delete (char *)token->data;
            break;
    }
}

void JsonTokens_free(std::vector<JsonToken> *tokens) {
    // Free evey token in vector
    for (size_t i = 0; i < tokens->size(); i++) {
        JsonToken token = (*tokens)[i];
        JsonToken_free(&token);
    }

    // Free vector
    delete tokens;
}
