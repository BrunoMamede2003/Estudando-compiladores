#include <token_type.h>

/*
enum __token_type {
  ID = 300,
  LITERAL_INT,
  LITERAL_FLOAT,
  STATIC_STRING,
  OPERATOR,
  ASSIGNMENT,
  SENTENCE_END,
  ERROR
};
*/

char const * const token_type_to_str(TokenType num) {
  switch(num) {
    case ID: return "ID";
    case LITERAL_INT: return "LITERAL_INT";
    case LITERAL_FLOAT: return "LITERAL_FLOAT";
    case STATIC_STRING: return "STATIC_STRING";
    case OPERATOR: return "OPERATOR";
    case ASSIGNMENT: return "ASSIGNMENT";
    case SENTENCE_END: return "SENTENCE_END";
    default: return "ERROR";
  }
}