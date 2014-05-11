#ifndef ENTEE_PARSER_H_
#define ENTEE_PARSER_H_

#include "string.h"

typedef struct entee_parser_s entee_parser;

entee_parser *entee_new_parser();

void entee_free_parser(entee_parser *parser);

int entee_parser_parse(entee_parser *parser);

typedef int (*entee_reader)(unsigned char *buffer, unsigned len, void *arg);

void entee_parser_set_reader(entee_parser *parser,
                             entee_reader reader,
                             void *reader_arg);


typedef enum {
    ENTEE_IRI,
    ENTEE_BLANK_NODE,
    ENTEE_SIMPLE_LITERAL_VALUE,
    ENTEE_LANGUAGE_TAGGED_LITERAL_VALUE,
    ENTEE_DATATYPE_LITERAL_VALUE,
    ENTEE_LANGUAGE_TAGGED_LITERAL_LANGUAGE,
    ENTEE_DATATYPE_LITERAL_IRI,
} entee_token_type;

typedef void (*entee_handler)(entee_token_type type, string *s);

void entee_parser_set_handler(entee_parser *parser,
                              entee_handler handlerr,
                              void *handler_arg);


#endif /* ENTEE_PARSER_H_ */
