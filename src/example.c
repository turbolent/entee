
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <getopt.h>
#include <zlib.h>
#include <bzlib.h>

#include "parser.h"
#include "string.h"

typedef enum {
    STATE_SUBJECT,
    STATE_PREDICATE,
    STATE_OBJECT,
    STATE_EXTRA
} parser_state;

parser_state state = STATE_SUBJECT;
int triples = 0;

int read_gzip(unsigned char* p, unsigned int len, void *arg) {
    gzFile file = arg;
    return gzread(file, p, len);
}

int read_bzip(unsigned char* p, unsigned int len, void *arg) {
    BZFILE *file = arg;
    return BZ2_bzread(file, p, len);
}

void handler(entee_token_type type, const char *s, void *arg) {
    switch (state) {
        case STATE_SUBJECT: {
            printf("subject: %s\n", s);
            state = STATE_PREDICATE;
            triples += 1;
            return;
        }
        case STATE_PREDICATE: {
            printf("predicate: %s\n", s);
            state = STATE_OBJECT;
            return;
        }
        case STATE_OBJECT: {
            printf("object: %s\n", s);
            switch (type) {
                case ENTEE_LANGUAGE_TAGGED_LITERAL_VALUE:
                case ENTEE_DATATYPE_LITERAL_VALUE: {
                    state = STATE_EXTRA;
                    return;
                }
                default: {
                    state = STATE_SUBJECT;
                    return;
                }
            }
            return;
        }
        case STATE_EXTRA: {
            printf("...: %s\n", s);
            state = STATE_SUBJECT;
            return;
        }
    }
}


int main(int argc, char **argv) {
    char *filename = NULL;

    bool isBzip = false;

    // parse options
    int c;
    while ((c = getopt(argc, argv, "bf:")) != -1) {
        switch (c) {
        case 'f':
            filename = optarg;
            break;
        case 'b':
            isBzip = true;
            break;
        default:
            return EXIT_FAILURE;
        }
    }

    if (!filename) {
        return EXIT_FAILURE;
    }

    // open
    void *file = (isBzip
                  ? BZ2_bzopen(filename, "rb")
                  : gzopen(filename, "rb"));

    if (!file) {
        fprintf(stderr, "Unable to open file \"%s\"", filename);
        return EXIT_FAILURE;
    }

    // parse
    entee_parser *parser = entee_new_parser();

    entee_reader reader = isBzip ? read_bzip : read_gzip;

    entee_parser_set_reader(parser, reader, file);

    entee_parser_set_handler(parser, handler, NULL);

    entee_parser_parse(parser);

    entee_free_parser(parser);

    fprintf(stderr, "triples: %d\n", triples);

    // close
    if (isBzip) {
        BZ2_bzclose(file);
    } else {
        if (!gzeof(file)) {
            int err;
            const char *error_string = gzerror(file, &err);
            if (err) {
                fprintf(stderr, "gzip error: %s.\n", error_string);
                return EXIT_FAILURE;
            }
        }

        gzclose(file);
    }

    return EXIT_SUCCESS;
}
