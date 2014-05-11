#ifndef ENTEE_PARSER_INTERNAL_H_
#define ENTEE_PARSER_INTERNAL_H_

#include "string.h"

struct entee_parser_s {
    string *string;
    int line;
    int finished;
    entee_reader reader;
    void *reader_arg;
    entee_handler handler;
    void *handler_arg;
};


// typedef void (*ParseCallbackFunction)(Triple *triple, void *callback_arg);

//int parse(ParseReadFunction read_fn, void *read_arg,
//          ParseCallbackFunction callback_fn, void *callback_arg);

#endif /* ENTEE_PARSER_INTERNAL_H_ */
