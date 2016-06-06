#ifndef BOPLISTAST_H
#define BOPLISTAST_H

#include "listast.h"

class BopListAst : public ListAst
{
public:
    static const int UNUSED = -1, DEFAULT = 0;
    static const int ADD = 0, SUB = 1;
    static const int MUL = 0, DIV = 1, MOD = 2;
    static const int DOT = 0, CALL = 1, ARR = 2;

    BopListAst(Ast::Type t, Ast *lhs, Ast *rhs, int op);
    BopListAst(Ast *name, int dims);

    static BopListAst *makeDims(Ast *a);
    static BopListAst *makeDims(int dims);
    void addDims(Ast *a);
    void addDims(int dims);

    void dump() const override;
    BopListAst *clone() const override;
    Ast *remove(size_t pos) override;
    void clear() override;

    int opAt(size_t pos) const;
    void setOpAt(size_t pos, int op);
    int numOp() const;

protected:
    void doInsert(size_t pos, Ast *child) override;

private:
    BopListAst(Ast::Type t);
    void mergeIn(BopListAst *t, int lead = 0);
    bool isLeftAssoc() const;
    bool isRightAssoc() const;

private:
    std::vector<int> ops;
};

#endif // BOPLISTAST_H
