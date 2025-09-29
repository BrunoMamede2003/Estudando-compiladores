#include <basic_cfg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <list.h>
#include <tree.h>
#include <code_generator.h>
#include <macros.h>
#include <string.h>

static void print_str(void * const str, size_t depth) {
  printf("[%s:%lu]", (char*)str, depth);
}

char const * const test_str = "a*a+(a+a)-(a/(a+a))";

int main(void) {
  Parse_return result = preced0(test_str);
  if(not result.m_success) {
    printf("Error parsing the string\n");
    return 1;
  }
  printf("Printing the parse tree: \n");
  tree_print(result.m_parse_tree, print_str);
  char * write_on = malloc(strlen(test_str));
  if(not write_on) {
    perror("Memory error in main, cannot display the compilated code\n");
    parse_return_clear(result);
    return 1;
  }
  bool success = generator_run(result.m_parse_tree, write_on);
  if(not success) {
    perror("Error found in code generation");
    parse_return_clear(result);
    return 1;
  }
  printf("\nPrinting old code and new code:\n%s\n%s\n", test_str, write_on);
  free(write_on);
  write_on = NULL;
  parse_return_clear(result);
  return 0;
}