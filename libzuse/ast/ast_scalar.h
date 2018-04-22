#ifndef ZUSE_AST_SCALAR_H
#define ZUSE_AST_SCALAR_H

#include "ast.h"
#include <string>

#include <iostream>

class AstScalar : public Ast
{
public:
    AstScalar(Type t, const std::string &mText);

    AstScalar(AstScalar &&move) = default;
    AstScalar &operator=(AstScalar &&moveAssign) = default;

    void dump() const override;
    std::unique_ptr<Ast> clone() const override;

    const std::string &getText() const;

    void append(char c);
    void clear();

private:
    std::string mText;
};

#endif // ZUSE_AST_SCALAR_H
