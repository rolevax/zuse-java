#ifndef BOPLISTAST_H
#define BOPLISTAST_H

#include "listast.h"

class BopListAst : public ListAst
{
public:
    static const int UNUSED = -1;
    static const int ADD = 0, SUB = 1;
    static const int MUL = 0, DIV = 1, MOD = 2;
    static const int DOT = 0, CALL = 1, ARR = 2;

    BopListAst(Ast::Type t, Ast *lhs, Ast *rhs, int op);

    void dump() const override;
    BopListAst *clone() const override;
    Ast *remove(size_t pos) override;
    bool illOne() const override;

    int opAt(size_t pos) const;
    void setOpAt(size_t pos, int op);
    int numOp() const;

protected:
    void doInsert(size_t pos, Ast *child) override;

private:
    BopListAst(Ast::Type t);
    void mergeIn(BopListAst *t, int lead = 0);

private:
    std::vector<int> ops;
};

#endif // BOPLISTAST_H
