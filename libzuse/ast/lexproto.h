#ifndef ZUSE_LEX_PROTO_H
#define ZUSE_LEX_PROTO_H

// Tell Flex the lexer's prototype ...
#define YY_DECL yy::BisonParser::symbol_type yylex(const std::string&, AstRoot *)
// ... and declare it for the parser's sake.
YY_DECL;

#endif // ZUSE_LEX_PROTO_H

