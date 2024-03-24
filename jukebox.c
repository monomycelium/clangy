/// jukebox
///
/// a CLI tool to match an array of strings with user input, with colour.
/// concepts: strings, pointers
///
/// TODO: make it a better jukebox by allowing users to add tracks.

#include <stdint.h>
#include <stdio.h>
#include <string.h>

char tracks[][80] = {
    "keep our love alive — powfu",
    "watch me miss — powfu & jomie",
    "memories — conan gray",
    "everytime — jomie & powfu",
};

int find_string(char const* string, char const* substring) {
    char* position = strstr(string, substring);
    if (position) {
        printf("%.*s\033[32m%s\033[0m%s\n", (int)(position - string), string,
               substring, position + strlen(substring));
        return 1;
    }

    return 0;
}

int find_track(char pattern[]) {
    int i;
    int c = 0;
    for (i = 0; i < (sizeof tracks / sizeof tracks[0]); i++)
        c += find_string(tracks[i], pattern);

    return c;
}

int main() {
    char pattern[80];
    printf("enter the name of a track to search for: ");

    if (fgets(pattern, sizeof(pattern), stdin) == NULL) {
        fprintf(stderr, "failed to read line.\n");
        return 1;
    }

    size_t len = strlen(pattern);
    if (len > 1 && pattern[len - 1] == '\n') {
        pattern[len - 1] = '\0';
    } else {
        fprintf(stderr, "please enter a non-empty string.\n");
        return 1;
    }

    if (find_track(pattern) == 0) {
        fprintf(stderr, "track not found.\n");
        return 1;
    } else {
        return 0;
    }
}
