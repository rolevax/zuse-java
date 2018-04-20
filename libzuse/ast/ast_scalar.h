#ifndef ZUSE_AST_SCALAR_H
#define ZUSE_AST_SCALAR_H

#include "ast.h"
#include <string>

#include <iostream>

class AstScalar : public Ast
{
public:
    AstScalar(Type t, const std::string &mText);

    void dump() const override;
    AstScalar *clone() const override;

    const std::string &getText() const;

    void append(char c);
    void clear();

private:
    std::string mText;
};

#endif // ZUSE_AST_SCALAR_H
