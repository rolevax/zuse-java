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

    void dump() const override
    {
        std::cout << "TML:";
        ListAst::dump();
    }
    TermListAst(Ast::Type t);

    bool rasingAt(size_t pos) const;
    void setRasingAt(size_t pos, bool b);

    std::unique_ptr<Ast> remove(size_t pos) override;

protected:
    void doInsert(size_t pos, Ast *child) override;

private:
    std::vector<bool> rasing;
};

#endif // TERMLISTAST_H
