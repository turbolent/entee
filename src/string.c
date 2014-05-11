#include "string.h"

#define MAX_PREALLOC (1024 * 1024)
#define INITIAL_CAPACITY 8

string *string_new() {
    string *result = malloc(sizeof(string));
    int capacity = INITIAL_CAPACITY;
    result->chars = malloc(sizeof(char) * capacity);
    result->length = 0;
    result->capacity = capacity;
    return result;
}

void string_free(string *s) {
    free(s->chars);
    free(s);
}

bool string_make_room(string *s, size_t room) {
    size_t capacity = s->capacity;
    size_t length = s->length;
    if (length + room <= capacity) {
        return true;
    }
    capacity = length + room;
    if (capacity < MAX_PREALLOC) {
        capacity *= 2;
    } else {
        capacity += MAX_PREALLOC;
    }
    char *chars = realloc(s->chars, capacity + 1);
    if (chars == NULL) {
        return false;
    }
    s->chars = chars;
    s->capacity = capacity;
    return true;
}

bool string_add_checked(string *s, char c) {
    if (!string_make_room(s, 1)) {
        return false;
    } else {
        string_add(s, c);
        return true;
    }
}

bool string_add_codepoint(string *s, int cp) {
    if (cp < 0 || cp > 0x10ffff) {
        return false;
    } else if (cp < 0x80) {
        return string_add_checked(s, cp & 0x7F);
    } else if (cp < 0x800) {
        if (!string_make_room(s, 2))
            return false;
        string_add(s, 0xC0 | ((cp >> 6) & 0x1F));
        string_add(s, 0x80 | (cp & 0x3F));
    } else if (cp < 0x10000) {
        if (!string_make_room(s, 3))
            return false;
        string_add(s, 0xE0 | ((cp >> 12) & 0xF));
        string_add(s, 0x80 | ((cp >> 6) & 0x3F));
        string_add(s, 0x80 | (cp & 0x3F));
    } else {
        if (!string_make_room(s, 4))
            return false;
        string_add(s, 0xF0 | ((cp >> 18) & 0x7));
        string_add(s, 0x80 | ((cp >> 12) & 0x3F));
        string_add(s, 0x80 | ((cp >> 6) & 0x3F));
        string_add(s, 0x80 | (cp & 0x3F));
    }
    return true;
}

bool string_end(string *s) {
    if (!string_make_room(s, 1)) {
        return false;
    } else {
        *(s->chars + s->length) = '\0';
        return true;
    }
}

