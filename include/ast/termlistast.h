#ifndef TERMLISTAST_H
#define TERMLISTAST_H

#include "listast.h"

class TermListAst : public ListAst
{
public:
    enum class Op { ADD, SUB, MUL, DIV };

    static TermListAst &fromAst(Ast &ast);
    static const TermListAst &fromAst(const Ast &ast);

    static TermListAst *makeBop(Ast *lhs, Ast *rhs, Op op);

    TermListAst(Ast::Type t);
    void dump() const override;
    TermListAst *clone() const override;

    bool rasingAt(size_t pos) const;
    void setRasingAt(size_t pos, bool b);

    Ast *remove(size_t pos) override;

protected:
    void doInsert(size_t pos, Ast *child) override;

private:
    std::vector<bool> rasing;
};

#endif // TERMLISTAST_H
