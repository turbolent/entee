#ifndef ENTEE_STRING_H_
#define ENTEE_STRING_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    char *chars;
    size_t capacity;
    size_t length;
} string;

string *string_new();

void string_free(string *s);

static inline void string_add(string *s, char c) {
    *(s->chars + s->length++) = c;
}

bool string_make_room(string *s, size_t room);

bool string_add_checked(string *s, char c);

bool string_add_codepoint(string *s, int cp);

bool string_end(string *s);

#endif /* ENTEE_STRING_H_ */
