
#include <string.h>

#include <vector>
#include <string>

void lex_null(char **input, bool *added) {
    printf("lex_null(): %s\n", *input);

    const char* json_null = "null";
    const size_t json_null_len = strlen(json_null);

    if (strlen(*input) > json_null_len) {
        if (strncmp(*input, json_null, json_null_len) == 0) {
            *added = true;
            (*input) += json_null_len;
            printf("  Found null value\n");
            return;
        }
    }
    return;
}

void lex_boolean(char **input, bool *added) {
    printf("lex_boolean(): %s\n", *input);

    const char* json_true = "true";
    const char* json_false = "false";
    const size_t json_true_len = strlen(json_true);
    const size_t json_false_len = strlen(json_false);

    if (strlen(*input) > json_true_len) {
        if (strncmp(*input, json_true, json_true_len) == 0) {
            *added = true;
            (*input) += json_true_len;
            printf("  Found boolean TRUE\n");
            return;
        }
    }

    if (strlen(*input) > json_false_len) {
        if (strncmp(*input, json_false, json_false_len) == 0) {
            *added = true;
            (*input) += json_false_len;
            printf("  Found boolean FALSE\n");
            return;
        }
    }
}

void lex_number(char **input, bool *added) {
    std::string number = "";

    printf("lex_number(): %s\n", *input);

    // If first char is a digit keep adding digits
    while (strlen(*input) != 0) {
        char c = (*input)[0];
        // TODO: . - e ...
        if (c > 0x2F && c < 0x3A) {
            number += c;
            *added = true;
            (*input)++;
        } else {
            break;
        }
    }

    if (number != "") {
        printf("  Found number: %s\n", number.c_str());
    }
}

bool lex_string(char **input, bool *added) {
    const char JSON_QUOTE = '"';

    printf("lex_string(): %s\n", *input);
    //printf(" %c\n", *input[0]);
    //(*input)++;
    //printf(" %c\n", *input[0]);
    //(*input)++;
    //printf(" %c\n", *input[0]);
    //(*input)++;
    //printf(" %c\n", *input[0]);

    // If first character is not a quote input is not a string
    if ((*input)[0] != JSON_QUOTE) {
        //printf("lex_string() first char is not \"...\n");
        return true;
    }

    // Remove starting quote from input
    (*input)++;
    *added = true;

    std::string str = "";

    // Everything until the closing quote is part of json string
    //for (int i = 0; i < strlen(*input); i++) {
    while (strlen(*input) != 0) {
        char c = (*input)[0];
        if (c == JSON_QUOTE) {
            (*input)++;
            printf("  Found string: %s\n", str.c_str());
            return true;
        } else {
            str += c;
            (*input)++;
        }
    }

    fprintf(stderr, "Unexpected end of input while lexing string\n");
    return false;
}

bool lex(char *input) {
    printf("lex()\n");
    char *data = input;

    //char* foo = "\"foo\"";
    //lex_string(&foo);

    while (strlen(data) != 0) {
        //char *out = (char *)malloc(100);
        //memset(out, 0, 100);

        bool added = false;

        // Check if there is a string
        if(!lex_string(&data, &added)) {
            fprintf(stderr, "Parsing failed\n");
            return false;
        }
        if (added) {
            //printf("added something\n");
            continue;
        }

        // Check if there is a number
        lex_number(&data, &added);
        if (added) {
            //printf("added something\n");
            continue;
        }

        // Check if there is a boolean
        lex_boolean(&data, &added);
        if (added) {
            continue;
        }

        // Check if there is a NULL value
        lex_null(&data, &added);
        if (added) {
            continue;
        }

        // Check if there is a syntax char
        if (data[0] == '[' || data[0] == ']') {
            printf("  Found syntax char %c\n", data[0]);
            data++;
            continue;
        }
        if (data[0] == '{' || data[0] == '}') {
            printf("  Found syntax char %c\n", data[0]);
            data++;
            continue;
        }
        if (data[0] == ':' || data[0] == ',') {
            printf("  Found syntax char %c\n", data[0]);
            data++;
            continue;
        }

        // Skip whitespaces
        if (data[0] == ' ' || data[0] == '\n' /** TODO: ADD **/ ) {
            printf("  Found whitespace\n");
            data++;
            continue;
        }

        fprintf(stderr, "  Found unexpected character %c\n", data[0]);
        data++;     // TODO: Debug only
    }

    return true;
}

int main() {

    char content[] = "{\"foo\": [1, 2, {\"bar\": 2}], \"visible\": false},{ \"others\": null }";
    int current = 0;
    //std::vector<std::string> tokens;
    lex(content);
    return 0;
}
