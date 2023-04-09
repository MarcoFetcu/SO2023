#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <dirent.h>
#include <sys/stat.h>

void simple_list(char *path, int rec) {
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if (dir == NULL) {
        perror("Could not open directory");
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if (lstat(fullPath, &statbuf) == 0) {
                printf("%s\n", fullPath);
                if (S_ISDIR(statbuf.st_mode) && rec == 1) {
                    simple_list(fullPath, rec);
                }
            }
        }
    }
    closedir(dir);
}

int main(int argc, char **argv) {
    if (argc >= 2) {
        if (strcmp(argv[1], "variant") == 0) {
            printf("84020\n");
        } else if (strcmp(argv[1], "list") == 0) {
            char *path = NULL;
            int rec = 0;
            int size_bool = 0;
            int perm_bool = 0;
            char *size_smaller = NULL;
            char *has_perm_execute = NULL;
            for (int i = 2; i < argc; i++) {
                if (strcmp(argv[i], "recursive") == 0) {
                    rec = 1;
                } else if (strncmp(argv[i], "size_smaller=", 13) == 0) {
                    size_bool = 1;
                    size_smaller = (char *) malloc(strlen(argv[i] + 13) + 1);
                    strcpy(size_smaller, argv[i] + 13);
                    printf("%s\n", size_smaller);
                } else if (strncmp(argv[i], "has_perm_execute=", 17) == 0) {
                    perm_bool = 1;
                    has_perm_execute = (char *) malloc(strlen(argv[i] + 17) + 1);
                    strcpy(has_perm_execute, argv[i] + 17);
                    printf("%s\n", has_perm_execute);
                } else if (strncmp(argv[i], "path=", 5) == 0) {
                    path = (char *) malloc(strlen(argv[i] + 5) + 1);
                    strcpy(path, argv[i] + 5);
                    //printf("%s\n", path);
                }
            }

            if (path == NULL) {
                printf("ERROR\ninvalid directory path\n");
            } else {
                DIR *dir = NULL;
                dir = opendir(path);
                if (dir == NULL) {
                    printf("ERROR\ninvalid directory path\n");
                    return -1;
                }
                printf("SUCCESS\n");
                closedir(dir);
                if (size_bool == 0 && size_smaller == 0) {
                    simple_list(path, rec);
                }
            }
        }
    }
    return 0;
}