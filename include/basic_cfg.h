#ifndef BASIC_CFG_H
#define BASIC_CFG_H

#include <tree.h>
#include <stdbool.h>

struct __parse_return {
    bool const m_success;
    Tree_node * const m_parse_tree;
    char const * m_next;
};

typedef struct __parse_return Parse_return;

Parse_return preced0(char const * read);
Parse_return preced1(char const * read);
Parse_return preced2(char const * read);
Parse_return parse_expression(char const * read);

void parse_return_clear(Parse_return r);

#endif