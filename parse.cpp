
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <string.h>

#include <vector>
#include <string>



class JsonItem {
    private:
        JsonItem();
        ~JsonItem();
};

class JsonObject : public JsonItem {
    private:
        std::vector<JsonItem> items;

};



enum JsonType {
    JSON_STRING,
    JSON_INTEGER,
    JSON_BOOLEAN,
    JSON_NULL,
    JSON_SPECIAL
};

struct JsonToken {
    void *data;
    enum JsonType type;
};

static inline bool parse_array(std::vector<JsonToken> *tokens) {
    printf("parse_array()\n");
}

static inline bool parse_object(std::vector<JsonToken> *tokens) {
    printf("parse_object()\n");
}

bool parse(std::vector<JsonToken> *tokens) {
    JsonToken start_t = (*tokens)[0];
    if (start_t.type != JSON_SPECIAL) {
        printf("error - first char is not valid\n");
        return false;
    }

    char start_c = *(char *)start_t.data;
    if (start_c == '{') {
        parse_object(tokens);
    } else if (start_c == '[') {
        parse_array(tokens);
    }

    return false;
}

static inline bool lex_special(char **input, std::vector<JsonToken> *tokens) {
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

    char *special_pointer = (char *)malloc(sizeof(char));
    *special_pointer = *input[0];
    JsonToken token = { special_pointer, JSON_SPECIAL };
    tokens->push_back(token);

    (*input)++;
    return true;
}

static inline bool lex_null(char **input, std::vector<JsonToken> *tokens) {
    const char* json_null = "null";
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

static inline bool lex_boolean(char **input, std::vector<JsonToken> *tokens) {
    // Check if there is true boolean
    const char* json_true = "true";
    const size_t json_true_len = strlen(json_true);
    if (strlen(*input) > json_true_len) {
        if (strncmp(*input, json_true, json_true_len) == 0) {
            // Found boolean
            (*input) += json_true_len;

            // Add token
            bool *bool_pointer = (bool *)malloc(sizeof(bool));
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
            bool *bool_pointer = (bool *)malloc(sizeof(bool));
            *bool_pointer = false;
            JsonToken token = { bool_pointer, JSON_BOOLEAN };
            tokens->push_back(token);
            return true;
        }
    }

    // Did not find boolean value
    return false;
}

static inline bool lex_number(char **input, bool* error, std::vector<JsonToken> *tokens) {
    // Keep track if number is a float or an int
    bool is_float = false;
    bool is_signed = false;

    // If first char is a digit keep adding digits to number
    std::string json_number = "";
    while (strlen(*input) != 0) {
        char c = (*input)[0];
        // Legal chars for numbers are digits 0-9 minus sign and period
        // TODO: Add support for e
        if ((c > 0x2F && c < 0x3A) || c == '.' || c == '-') {
            // Add
            json_number += c;
            (*input)++;

            // Check if float
            if (c == '.') {
                if (is_float) {
                    // Error: Number contains two periods
                    fprintf(stderr,
                            "Unexpected char %c when parsing number %s\n",
                            c, json_number.c_str());
                    *error = true;
                    return false;
                } else {
                    is_float = true;
                }
            }

            // Check if signed
            if (c == '-') {
                if (is_signed) {
                    // Error: Number contains two minus signs
                    fprintf(stderr,
                            "Unexpected char %c when parsing number %s\n",
                            c, json_number.c_str());
                    *error = true;
                    return false;
                } else {
                    is_signed = true;
                }
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
            fprintf(stderr, "No support for floating point numbers like %f!\n", dnum);
            *error = true;
            return false;
        }
    } else {
        long lnum = atol(json_number.c_str());
        if (lnum > INT_MAX) {
            fprintf(stderr, "Number %ld is too big for int value!\n", lnum);
            *error = true;
            return false;
        }
        inum = (int)lnum;
    }

    int *num_pointer = (int *)malloc(sizeof(int));
    *num_pointer = inum;
    JsonToken token = { num_pointer, JSON_INTEGER };
    tokens->push_back(token);
    return true;
}

static inline bool lex_string(char **input, bool *error, std::vector<JsonToken> *tokens) {
    const char JSON_QUOTE = '"';

    // If first character is not a quote input is not a string
    if ((*input)[0] != JSON_QUOTE) {
        return false;
    } else {
        // Remove starting quote from input
        (*input)++;
    }

    // Everything until the closing quote is part of json string
    std::string json_str = "";
    while (strlen(*input) != 0) {
        char c = (*input)[0];

        if (c == JSON_QUOTE) {
            // Found closing quote -> string is complete
            (*input)++;

            // Add token
            char *out = (char *)malloc(strlen(json_str.c_str()) + 1);
            out = strcpy(out, json_str.c_str());
            JsonToken token = { out, JSON_STRING };
            tokens->push_back(token);
            return true;
        } else {
            // Add character to string
            json_str += c;
            (*input)++;
        }
    }

    // Error: Did not find a closing quote
    *error = true;
    fprintf(stderr, "Unexpected end of input while lexing string\n");
    return false;
}

static inline bool lex_all(char *input, std::vector<JsonToken> *tokens) {
    char *data = input;

    while (strlen(data) != 0) {
        // Check if there is a string
        {
            bool error = false;
            bool added = lex_string(&data, &error, tokens);

            if (error) {
                fprintf(stderr, "Parsing failed\n");
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
                fprintf(stderr, "Parsing failed\n");
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
        if (data[0] == ' ' || data[0] == '\n' /** TODO: ADD **/ ) {
            data++;
            continue;
        }

        fprintf(stderr, "  Found unexpected character %c\n", data[0]);
        return false;
    }

    return true;
}

bool lex(char *input, std::vector<JsonToken> *tokens) {
    if (!lex_all(input, tokens)) {
        return false;
    }

    printf("Lexing got %d tokens\n", (int)tokens->size());
    for (size_t i = 0; i < tokens->size(); i++) {
        JsonToken t = (*tokens)[i];
        switch (t.type) {
            case JSON_STRING:
                printf("[%02d]  str: '%s'\n", (int)i, (char *)t.data);
                break;
            case JSON_INTEGER:
                printf("[%02d]  int: '%d'\n", (int)i, *(int *)t.data);
                break;
            case JSON_BOOLEAN:
                printf("[%02d]  int: '%s'\n", (int)i, *(bool *)t.data == true ? "true" : "false");
                break;
            case JSON_NULL:
                printf("[%02d]  null\n", (int)i);
                break;
            case JSON_SPECIAL:
                printf("[%02d]  str: '%c'\n", (int)i, *(char *)t.data);
                break;
            default:
                printf("[%02d]\n", (int)i);
                break;
        }
    }

    return true;
}

int main() {
    // Input
    char content[] = "{\"foo\": [1, 2, {\"bar\": 2}], \"visible\": false},{ \"others\": null, \"number\": 5.0, \"one\": 100 }";

    // Lex json
    std::vector<JsonToken> tokens;
    lex(content, &tokens);

    printf("Lexing complete\n");
    printf("%s\n", content);

    // Parse tokens
    parse(&tokens);

    // Free tokens
    for (size_t i = 0; i < tokens.size(); i++) {
        free(tokens[i].data);
    }

    return 0;
}

