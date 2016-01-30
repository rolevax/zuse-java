#ifndef BOPAST_H
#define BOPAST_H

#include "ast.h"

class BopAst : public Ast
{
public:
    static BopAst &fromAst(Ast &a);
    static const BopAst &fromAst(const Ast &a);

    BopAst(Type t, Ast *l, Ast *r);

    void dump() const override
    {
        std::cout << "BOP:";
        lhs->dump();
        rhs->dump();
    }
    size_t size() const override;
    Ast &at(size_t pos) const override;
    size_t indexOf(const Ast *child) const override;

protected:
    void doChange(size_t pos, Ast *next) override;

private:
    std::unique_ptr<Ast> lhs;
    std::unique_ptr<Ast> rhs;
};

#endif // BOPAST_H
