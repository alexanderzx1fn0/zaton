#include "Stream.h"

Stream::Stream(const char *name) : file(NULL), pos(0), size(0) {
    file = fopen(name, "rb");
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
}

Stream::~Stream() {
    fclose(file);
}

void Stream::seek(int offset) {
    fseek(file, offset, SEEK_CUR);
    pos += offset;
}

int Stream::read(void *data, int count) {
    int res = fread(data, 1, count, file);
    pos += res;
    return res;
}

char* Stream::readStr() {
    uint8 len;
    read(&len, 1);
    if (len == 0)
        return NULL;
    char *str = new char[len + 1];
    read(str, len);
    str[len] = 0;
    return str;
}
