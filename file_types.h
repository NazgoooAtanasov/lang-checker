#ifndef FILE_TYPES_H_
#define FILE_TYPES_H_

#define FORBIDEN_DIRECTORIES_CAP 4
const char *FORBIDEN_DIRECTORIES[FORBIDEN_DIRECTORIES_CAP] = {
    ".git",
    ".log",
    "out",
    "node_modules"
};

#define FORBIDEN_FILES_CAP 2
const char *FORBIDEN_FILES[FORBIDEN_FILES_CAP] = {
    ".gitignore",
    "package.json"
};

#endif // FILE_TYPES_H_
