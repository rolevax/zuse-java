#ifndef SCALARAST_H
#define SCALARAST_H

#include "ast.h"
#include <string>

#include <iostream>

class ScalarAst : public Ast
{
public:
    ScalarAst(Type t, const std::string &mText);

    void dump() const override;
    ScalarAst *clone() const override;

    const std::string &getText() const;

    void append(char c);
    void clear();

private:
    std::string mText;
};

#endif // SCALARAST_H
