#include <ftw.h>
#include <string.h>

#define EXIT_FUNC (1)

char *g_file_to_find;
const char *g_found_path;

int analyze_file(const char *file_path, const struct stat *stat_ptr, int flag) {
    if (flag == FTW_F) {
        int index = 0;
        for (int i = 0; file_path[i] != '\0'; i++) {
            if (file_path[i] == '/') {
                index = i;
            }
        }
        const char *found_string = file_path + index + 1;
        if (strcmp(found_string, g_file_to_find) == 0) {
            g_found_path = file_path;
            return EXIT_FUNC;
        }
    }
    return 0;
}

const char *make_library(char *directory, char *file_to_find) {
    g_file_to_find = file_to_find;
    ftw(directory, analyze_file, 20);
    return g_found_path;
}
