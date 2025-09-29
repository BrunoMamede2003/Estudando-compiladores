#include <code_generator.h>
#include <tree.h>
#include <macros.h>
#include <stdio.h>

static char * generator_match(Tree_node * ast_view, char * write_on) {
  if(not ast_view)
    return NULL;
  char const cr = ((char *)tree_access_item(ast_view))[0];
  if(cr == 'a') {
    *write_on = 'a';
    return write_on + 1;
  } else if(cr == '(') {
    char * write_second = generator_match(tree_access_child(ast_view, 0), write_on);
    if(not write_second) {
      perror("Error found in ast structure, aborting code generation");
      return NULL;
    }
    return write_second;
  } else if(cr == '*' or cr == '/' or cr == '+' or cr == '-') {
    char * write_second = generator_match(tree_access_child(ast_view, 0), write_on);
    if(not write_second) {
      perror("Error found in ast structure, aborting code generation");
      return NULL;
    }
    write_second = generator_match(tree_access_child(ast_view, 1), write_second);
    if(not write_second) {
      perror("Error found in ast structure, aborting code generation");
      return NULL;
    }
    *write_second = cr;
    return write_second + 1;
  } else
    return NULL;
}

bool generator_run(Tree_node * ast, char * write_on) {
  char * const final_write = generator_match(ast, write_on); 
  if(not final_write)
    return false;
  *final_write = '\0';
  return true;
}