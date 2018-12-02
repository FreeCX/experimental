#include "data.h"

uint8_t __DEBUG_FLAG__ = 0;

uint16_t bswap16(uint16_t input) { return ((input & 0xFF) << 8) | (input >> 8); }

uint32_t bswap32(uint32_t input) {
    return ((input & 0xFF) << 24) | ((input & 0xFF00) << 8) | ((input & 0xFF0000) >> 8) | (input >> 24);
}

uint64_t bswap64(uint64_t input) {
    union {
        uint64_t ll;
        uint32_t l[2];
    } w, r;

    w.ll = input;
    r.l[0] = bswap32(w.l[1]);
    r.l[1] = bswap32(w.l[0]);
    return r.ll;
}

void swap8(uint8_t *a, uint8_t *b) {
    uint8_t t = *a;
    *a = *b;
    *b = t;
}

void swap16(uint16_t *a, uint16_t *b) {
    uint16_t t = *a;
    *a = *b;
    *b = t;
}

void swap32(uint32_t *a, uint32_t *b) {
    uint32_t t = *a;
    *a = *b;
    *b = t;
}

size_t fsize(FILE *f) {
    size_t pos = ftell(f);
    size_t size;

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, pos, SEEK_SET);
    return size;
}

uint8_t getbits8(uint8_t a, uint8_t p, uint8_t n) { return (a >> (p + 1 - n)) & ~(~0 << n); }

uint32_t dec2bin(uint8_t a) {
    uint32_t result = 0, n = 0;
    do {
        result += (a % 2) * pow(10, n++);
    } while ((a /= 2) >= 1);
    return result;
}