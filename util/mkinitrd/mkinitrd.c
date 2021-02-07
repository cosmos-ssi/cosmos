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

struct initrd_header {
    unsigned char magic;
    char name[INITRD_NAME_SIZE];
    unsigned int offset;
    unsigned int length;
};

int main(int argc, char** argv) {
    int nheaders = (argc - 1) / 2;
    struct initrd_header headers[INITRD_MAX_FILES];
    memset(&headers, 0, sizeof(struct initrd_header) * INITRD_MAX_FILES);
    printf("size of header: %lu\n", sizeof(struct initrd_header));
    unsigned int off = (sizeof(struct initrd_header) * INITRD_MAX_FILES) + sizeof(int);
    int i;
    for (i = 0; i < nheaders; i++) {
        printf("writing file %s->%s at 0x%x\n", argv[i * 2 + 1], argv[i * 2 + 2], off);
        strcpy(headers[i].name, argv[i * 2 + 2]);
        headers[i].offset = off;
        FILE* stream = fopen(argv[i * 2 + 1], "r");
        if (stream == 0) {
            printf("Error: file not found: %s\n", argv[i * 2 + 1]);
            return 1;
        }
        fseek(stream, 0, SEEK_END);
        headers[i].length = ftell(stream);
        off += headers[i].length;
        fclose(stream);
        headers[i].magic = 0xBF;
    }

    FILE* wstream = fopen("./initrd.img", "w");
    unsigned char* data = (unsigned char*)malloc(off);
    fwrite(&nheaders, sizeof(int), 1, wstream);
    fwrite(headers, sizeof(struct initrd_header), INITRD_MAX_FILES, wstream);

    for (i = 0; i < nheaders; i++) {
        FILE* stream = fopen(argv[i * 2 + 1], "r");
        unsigned char* buf = (unsigned char*)malloc(headers[i].length);
        fread(buf, 1, headers[i].length, stream);
        fwrite(buf, 1, headers[i].length, wstream);
        fclose(stream);
        free(buf);
    }

    fclose(wstream);
    free(data);

    return 0;
}