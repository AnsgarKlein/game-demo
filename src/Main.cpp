
#include "Main.h"

#include <string.h>

#include "Game.h"
#include "Level.h"

/**
 * @brief Prints helping information to terminal.
 *
 * @param argv0 Value in argv[0], which is the name with
 *        which this application was started.
 *
 * @param error Whether help output should be printed as an error
 *        (true -> stderr) or as normal output (false -> stdout)
 */
static void help(char *argv0, bool error) {
    FILE *out = error ? stderr : stdout;

    fprintf(out, "Usage: %s [OPTIONS]\n", argv0);
    fprintf(out, "\n");
    fprintf(out, "OPTIONS\n");
    fprintf(out, "  -h, --help\n");
    fprintf(out, "      Print this help\n");
    fprintf(out, "\n");
    fprintf(out, "  There aren't any other options (yet?)\n");
}

/**
 * @brief Parses arguments given to application on command line
 *
 * @param argc Number of arguments to parse
 * @param argv String array of arguments to parse
 *
 * @return False if application should not keep running, true otherwise
 */
static bool parse_arguments(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];

        if (strcmp(arg, "-h")     == 0 ||
            strcmp(arg, "--help") == 0) {
            help(argv[0], false);
            return false;
        }

        // Unknown command line option
        fprintf(stderr, "Unknown option '%s'\n", arg);
        help(argv[0], true);
        return false;
    }

    return true;
}

/**
 * @brief main() function
 */
int main(int argc, char **argv) {
    // Parse command line options
    if (!parse_arguments(argc, argv)) {
        return 0;
    }

    // Initialize SDL and other basic functionality
    if (initialize() == false) {
        return 1;
    }

    if(!set_level_from_buffer(DEFAULT_LEVEL, strlen(DEFAULT_LEVEL))) {
        fprintf(stderr, "Could not load level\n");
        return 1;
    }

    // Start the game
    game_loop();

    // End
    deinitialize();
    return 0;
}

