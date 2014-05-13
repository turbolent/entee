
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <getopt.h>
#include <zlib.h>
#include <bzlib.h>

#include "parser.h"
#include "string.h"


int read_gzip(unsigned char* p, unsigned int len, void *read_arg) {
    gzFile file = read_arg;
    return gzread(file, p, len);
}

int read_bzip(unsigned char* p, unsigned int len, void *read_arg) {
    BZFILE *file = read_arg;
    return BZ2_bzread(file, p, len);
}

void handler(entee_token_type type, string *s) {
    printf("%d => %s\n", type, s->chars);
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
