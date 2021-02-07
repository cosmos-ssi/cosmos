//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// http://www.jamesmolloy.co.uk/tutorial_html/8.-The%20VFS%20and%20the%20initrd.html

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

unsigned int makeheaders(struct initrd_fs_header* fs_header, char** argv) {
    unsigned int off = sizeof(struct initrd_fs_header);
    for (int i = 0; i < fs_header->nheaders; i++) {
        printf("writing file %s->%s at 0x%x\n", argv[i * 2 + 1], argv[i * 2 + 2], off);
        strcpy(fs_header->headers[i].name, argv[i * 2 + 2]);
        fs_header->headers[i].offset = off;
        FILE* stream = fopen(argv[i * 2 + 1], "r");
        if (stream == 0) {
            printf("Error: file not found: %s\n", argv[i * 2 + 1]);
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

void addfiles(struct initrd_fs_header* fs_header, char** argv, unsigned int off) {
    FILE* wstream = fopen(INITRD_IMAGE_NAME, "w");
    unsigned char* data = (unsigned char*)malloc(off);
    fwrite(fs_header, sizeof(struct initrd_fs_header), 1, wstream);

    for (int i = 0; i < fs_header->nheaders; i++) {
        FILE* stream = fopen(argv[i * 2 + 1], "r");
        unsigned char* buf = (unsigned char*)malloc(fs_header->headers[i].length);
        fread(buf, 1, fs_header->headers[i].length, stream);
        fwrite(buf, 1, fs_header->headers[i].length, wstream);
        fclose(stream);
        free(buf);
    }
    fclose(wstream);
    free(data);
}

int main(int argc, char** argv) {
    printf("CosmOS mkinitrd\n");
    int nheaders = (argc - 1) / 2;

    /*
    * make the fs header
    */
    struct initrd_fs_header fs_header;
    memset(&fs_header, 0, sizeof(struct initrd_fs_header));
    fs_header.nheaders = nheaders;
    printf("Size of filesystem header: %lu\n", sizeof(struct initrd_fs_header));
    printf("Size of initrd file header: %lu\n", sizeof(struct initrd_file_header));

    /*
    * add the headers, return the current file offset
    */
    unsigned int off = makeheaders(&fs_header, argv);
    /*
    * add files
    */
    addfiles(&fs_header, argv, off);
    return 0;
}