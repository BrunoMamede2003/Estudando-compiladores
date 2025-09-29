#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <tree.h>
#include <stdbool.h>

bool generator_run(Tree_node * ast, char * write_on);

#endif //CODE_GENERATOR_H