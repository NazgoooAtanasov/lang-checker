#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "./file_types.h"

typedef struct {
    const char *file_type;
    int count;
} FileType;

#define FILE_TYPES_CAP (4 * 1024)
FileType file_types[FILE_TYPES_CAP] = {0};
size_t file_types_sz = 0;
int files_count = 0;

int check_proper_dir(const char *path_name)
{
    return strcmp(".", path_name) && strcmp("..", path_name);
}

// TODO: lookup how to handle these kinds of stuff. Obs something is breaking the current stuff.
const char *construct_path(const char *base, const char *separator, const char *addition)
{
    char *path = (char *)malloc(strlen(base) + strlen(separator) + strlen(addition));
    path[0] = '\0';

    strcat(path, base);
    strcat(path, separator);
    strcat(path, addition);

    return path;
}

FileType *find_ft(const char *extension)
{
    for(size_t i = 0; i < file_types_sz; i++)
    {
        if (file_types[i].file_type != NULL && strcmp(extension, file_types[i].file_type) == 0)
            return &file_types[i];
    }
    return NULL;
}

void parse_file(const char *file_name)
{
    const char *file_extension = strrchr(file_name, '.');

    if (file_extension)
    {
        FileType *ft = find_ft(file_extension);

        if (ft == NULL)
        {
            FileType *new_ft = malloc(sizeof(FileType));
            new_ft->count = 1;
            new_ft->file_type = file_extension;

            file_types[file_types_sz] = *new_ft;
            file_types_sz++;
        }
        else
        {
            ft->count++;
        }

        files_count++;
    }

    /* TODO: add support for `other` file types */
}

int is_forbiden(int file_type, const char* file_name)
{
    if (file_type == DT_DIR)
    {
        for (size_t i = 0; i < FORBIDEN_DIRECTORIES_CAP; ++i)
            if (strcmp(file_name, FORBIDEN_DIRECTORIES[i]) == 0)
                return 1;
    }
    else if (file_type == DT_REG)
    {
        for (size_t i = 0; i < FORBIDEN_FILES_CAP; ++i)
            if (strcmp(file_name, FORBIDEN_FILES[i]) == 0)
                return 1;
    }

    return 0;
}

void iterate(const char *path_name)
{
    DIR *dir;
    struct dirent *dir_dirent;

    dir = opendir(path_name);

    if (!dir)
    {
        fprintf(stderr, "Error opening dir\n");
        exit(1);
    }

    dir_dirent = readdir(dir);

    while (dir_dirent != NULL)
    {
        if (check_proper_dir(dir_dirent->d_name))
        {
            if (dir_dirent->d_type == DT_REG && !is_forbiden(DT_REG, dir_dirent->d_name))
            {
                parse_file(dir_dirent->d_name);
            }
            else if (dir_dirent->d_type == DT_DIR && !is_forbiden(DT_DIR, dir_dirent->d_name))
            {
                iterate(construct_path(path_name, "/", dir_dirent->d_name));
            }
        }

        dir_dirent = readdir(dir);
    }

    closedir(dir);
}

void get_results()
{
    for (size_t i = 0; i < file_types_sz; i++)
    {
        printf("%s, %d: %.2f%%\n",
               file_types[i].file_type,
               file_types[i].count,
               ((float) file_types[i].count / files_count) * 100
        );
    }
}

int main(int argc, char *arg[])
{
    if (argc <= 1)
    {
        fprintf(stderr, "Not enough args!\n");
        exit(1);
    }

    const char *path_name = arg[1];

    iterate(path_name);
    get_results();

    return 0;
}
