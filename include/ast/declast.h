#ifndef DECLAST_H
#define DECLAST_H

#include "ast.h"

class DeclAst : public Ast
{
public:
    DeclAst(Ast *type, Ast *bean);

    void dump() const override
    {
        type->dump();
        bean->dump();
    }
    size_t size() const override;
    Ast &at(size_t pos) const override;
    size_t indexOf(const Ast *child) const override;

protected:
    void doChange(size_t pos, Ast *next) override;

private:
    std::unique_ptr<Ast> type;
    std::unique_ptr<Ast> bean;
};

#endif // DECLAST_H
