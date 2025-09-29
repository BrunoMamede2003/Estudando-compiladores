#include <tree.h>
#include <basic_cfg.h>
#include <macros.h>
#include <stdio.h>

void parse_return_clear(Parse_return r) {
  if(r.m_parse_tree)
    tree_free(r.m_parse_tree);
}

Parse_return preced2(char const * read) {
  if(*read == 'a') {
    Tree_node * node = tree_new("a", 2, NULL);
    if(not node) {
      perror("Error calling tree_new inside preced2");
      return (Parse_return){false, NULL, NULL};
    }
    return (Parse_return){true, node, read+1};
  } else if(*read == '(') {
    Parse_return preced0_return = preced0(++read);
    if(not preced0_return.m_success)
      return (Parse_return){false, NULL, NULL};
    read = preced0_return.m_next;
    if(not (*read == ')')) {
      parse_return_clear(preced0_return);
      return (Parse_return){false, NULL, NULL};
    }
    Tree_node * node = tree_new("()", 3, NULL);
    tree_link_nodes(node, preced0_return.m_parse_tree);
    return (Parse_return){true, node, read + 1};
  } else {
    return (Parse_return){false, NULL, NULL};
  }
}

Parse_return cont_1(char const * read, Tree_node * const left_operand) {
  char const op[2] = {[0] = *read,[1] = '\0'};
  if(op[0] != '*' and op[0] != '/')
    return (Parse_return){true, left_operand, read};
  Parse_return preced2_result = preced2(++read);
  if(not preced2_result.m_success) {
    tree_free(left_operand);
    return (Parse_return){false, NULL, NULL};
  }
  Tree_node * node = tree_new(op, 2, NULL);
  tree_link_nodes(node, left_operand);
  tree_link_nodes(node, preced2_result.m_parse_tree);
  return cont_1(preced2_result.m_next, node);
}

Parse_return preced1(char const * read) {
  Parse_return preced2_return = preced2(read);
  if(not preced2_return.m_success)
    return (Parse_return){false, NULL, NULL};
  return cont_1(preced2_return.m_next, preced2_return.m_parse_tree);
}

Parse_return cont_0(char const * read, Tree_node * const left_operand) {
  char const op[2] = {[0] = *read,[1] = 0};
  if(op[0] != '+' and op[0] != '-')
    return (Parse_return){true, left_operand, read};
  Parse_return preced1_result = preced1(++read);
  if(not preced1_result.m_success) {
    tree_free(left_operand);
    parse_return_clear(preced1_result);
    return (Parse_return){false, NULL, NULL};
  }
  Tree_node * node = tree_new(op, 2, NULL);
  tree_link_nodes(node, left_operand);
  tree_link_nodes(node, preced1_result.m_parse_tree);
  return cont_0(preced1_result.m_next, node);
}

Parse_return preced0(char const * read) {
  Parse_return preced1_return = preced1(read);
  if(not preced1_return.m_success)
    return (Parse_return){false, NULL, NULL};
  return cont_0(preced1_return.m_next, preced1_return.m_parse_tree);
}

Parse_return parse_expression(const char *read) {
  return preced0(read);
}