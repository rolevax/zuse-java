#ifndef METHODAST_H
#define METHODAST_H

#include "scalarast.h"
#include "listast.h"

class MethodAst : public Ast
{
public:
    static MethodAst &fromAst(Ast &a);
    static const MethodAst &fromAst(const Ast &a);

    MethodAst(const std::string &id, ListAst *params, ListAst *stmts);

    void dump() const override
    {
        std::cout << "method " << id->getText() << ':' << std::endl;
        stmts->dump();
    }
    size_t size() const override;
    Ast &at(size_t pos) const override;
    size_t indexOf(const Ast *child) const override;

protected:
    void doChange(size_t pos, Ast *next) override;

private:
    std::unique_ptr<ScalarAst> id;
    std::unique_ptr<ListAst> params;
    std::unique_ptr<ListAst> stmts;
};

#endif // METHODAST_H
