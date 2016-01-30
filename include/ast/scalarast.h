#ifndef SCALARAST_H
#define SCALARAST_H

#include "ast/ast.h"
#include <string>

#include <iostream>

class ScalarAst : public Ast
{
public:
    static ScalarAst &fromAst(Ast &ast);
    static const ScalarAst &fromAst(const Ast &ast);

    ScalarAst(Type t, const std::string &text);

    void dump() const override
    {
        std::cout << text;
    }
    ScalarAst *clone() const override;

    const std::string &getText() const;

    void append(char c);
    void clear();

private:
    std::string text;
};

#endif // SCALARAST_H
