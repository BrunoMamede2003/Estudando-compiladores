#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

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

typedef enum __token_type TokenType;

char const * const token_type_to_str(TokenType num);

#endif // TOKEN_TYPE_H