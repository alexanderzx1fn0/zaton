#ifndef STREAM_H_
#define STREAM_H_

#include <stdio.h>
#include "utils.h"

struct Stream {
    FILE *file;
    int  pos;
    int  size;

    Stream(const char *name);
    ~Stream();

    void seek(int offset);
    int read(void *data, int count);
    char* readStr();
};
#endif
