#ifndef PARENAST_H
#define PARENAST_H

#include "ast.h"

class ParenAst : public Ast
{
public:
    static ParenAst &fromAst(Ast &a);
    static const ParenAst &fromAst(const Ast &a);

    ParenAst(Ast *sub);

    void dump() const override
    {
        std::cout << '(';
        subtree->dump();
        std::cout << ')';
    }
    size_t size() const override;
    Ast &at(size_t pos) const override;
    size_t indexOf(const Ast *child) const override;

protected:
    void doChange(size_t pos, Ast *next) override;

private:
    std::unique_ptr<Ast> subtree;
};

#endif // PARENAST_H
