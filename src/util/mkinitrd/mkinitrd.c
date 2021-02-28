//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// http://www.jamesmolloy.co.uk/tutorial_html/8.-The%20VFS%20and%20the%20initrd.html

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITRD_NAME_SIZE 64
#define INITRD_MAX_FILES 64

#define INITRD_IMAGE_NAME "./initrd.img"

struct initrd_file_header {
    unsigned char magic;
    char name[INITRD_NAME_SIZE];
    unsigned int offset;
    unsigned int length;
};

struct initrd_fs_header {
    int nheaders;
    struct initrd_file_header headers[INITRD_MAX_FILES];
};

struct f {
    char longname[INITRD_NAME_SIZE];
    char shortname[INITRD_NAME_SIZE];
};

struct filenames {
    int count;
    struct f names[INITRD_MAX_FILES];
};

/*
* fill the structure fs_header
*/
unsigned int makeheaders(struct initrd_fs_header* fs_header, struct filenames* fns) {
    unsigned int off = sizeof(struct initrd_fs_header);
    for (int i = 0; i < fs_header->nheaders; i++) {
        char* shortname = fns->names[i].shortname;
        char* longname = fns->names[i].longname;
        printf("writing file '%s' -> '%s' at 0x%x\n", longname, shortname, off);
        strcpy(fs_header->headers[i].name, shortname);
        fs_header->headers[i].offset = off;
        FILE* stream = fopen(longname, "r");
        if (stream == 0) {
            printf("Error: file not found: %s\n", longname);
            return 1;
        }
        fseek(stream, 0, SEEK_END);
        fs_header->headers[i].length = ftell(stream);
        off += fs_header->headers[i].length;
        fclose(stream);
        fs_header->headers[i].magic = 0xBF;
    }
    return off;
}

/*
* write the header and files
*/
void addfiles(struct initrd_fs_header* fs_header, struct filenames* fns, unsigned int off) {
    FILE* wstream = fopen(INITRD_IMAGE_NAME, "w");
    /*
    * write the headers
    */
    fwrite(fs_header, sizeof(struct initrd_fs_header), 1, wstream);

    /*
    * write the files
    */
    unsigned char* data = (unsigned char*)malloc(off);
    for (int i = 0; i < fs_header->nheaders; i++) {
        char* longname = fns->names[i].longname;
        FILE* stream = fopen(longname, "r");
        unsigned char* buf = (unsigned char*)malloc(fs_header->headers[i].length);
        fread(buf, 1, fs_header->headers[i].length, stream);
        fwrite(buf, 1, fs_header->headers[i].length, wstream);
        fclose(stream);
        free(buf);
    }
    fclose(wstream);
    free(data);
}

void addfile(struct filenames* fns, char* fn) {
    strcpy((fns->names[fns->count].longname), fn);
    char* c = strrchr(fn, '/');
    if (0 == c) {
        strcpy((fns->names[fns->count].shortname), fn);
    } else {
        strcpy((fns->names[fns->count].shortname), c + 1);
    }
    fns->count = fns->count + 1;
}

void fillnames(char* dirname, struct filenames* fns) {
    struct dirent* dir;
    DIR* d = opendir(dirname);
    if (0 != d) {
        while ((dir = readdir(d)) != 0) {
            if ((0 != strcmp(dir->d_name, "..")) && (0 != strcmp(dir->d_name, "."))) {
                char fn[1024];
                strcpy(fn, dirname);
                //                strcat(fn, "/");
                strcat(fn, dir->d_name);
                //     printf("%s\n", fn);
                addfile(fns, fn);
            }
        }
        closedir(d);
    }
}

int main(int argc, char** argv) {
    printf("CosmOS mkinitrd\n");
    /*
    * fn list
    */
    struct filenames fns;
    fns.count = 0;
    /*
    * walk args
    */
    int nargs = argc - 1;
    for (int i = 1; i <= nargs; i++) {
        char* a = argv[i];
        if (a[strlen(a) - 1] == '/') {
            fillnames(a, &fns);
        } else {
            addfile(&fns, a);
        }
    }
    //
    /*
    * make the fs header
    */
    struct initrd_fs_header fs_header;
    memset(&fs_header, 0, sizeof(struct initrd_fs_header));
    fs_header.nheaders = fns.count;
    printf("Size of filesystem header: %lu\n", sizeof(struct initrd_fs_header));
    printf("Size of initrd file header: %lu\n", sizeof(struct initrd_file_header));

    /*
    * add the headers, return the current file offset
    */
    unsigned int off = makeheaders(&fs_header, &fns);
    /*
    * add files
    */
    addfiles(&fs_header, &fns, off);
    return 0;
}