#ifndef ZUSE_AST_LIST_BOP_H
#define ZUSE_AST_LIST_BOP_H

#include "ast_list.h"

class AstListBop : public AstList
{
public:
    static const int UNUSED = -1, DEFAULT = 0;
    static const int ADD = 0, SUB = 1;
    static const int MUL = 0, DIV = 1, MOD = 2;
    static const int DOT = 0, CALL = 1, ARR = 2;

    AstListBop(Ast::Type t, Ast *lhs, Ast *rhs, int op);
    AstListBop(Ast *name, int dims);

    static AstListBop *makeDims(Ast *a);
    static AstListBop *makeDims(int dims);
    void addDims(Ast *a);
    void addDims(int dims);

    void dump() const override;
    AstListBop *clone() const override;
    Ast *remove(size_t pos) override;
    void clear() override;

    int opAt(size_t pos) const;
    void setOpAt(size_t pos, int op);
    int numOp() const;

protected:
    void doInsert(size_t pos, Ast *child) override;

private:
    AstListBop(Ast::Type t);
    void mergeIn(AstListBop *t, int lead = 0);
    bool isLeftAssoc() const;
    bool isRightAssoc() const;

private:
    std::vector<int> mOps;
};

#endif // ZUSE_AST_LIST_BOP_H
