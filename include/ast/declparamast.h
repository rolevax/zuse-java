#ifndef DECLPARAMAST_H
#define DECLPARAMAST_H

#include "scalarast.h"

class DeclParamAst : public Ast
{
public:
    DeclParamAst(Ast *type, const std::string &id);

    void dump() const override
    {
        type->dump();
        std::cout << ' ';
        id->dump();
    }

    size_t size() const override;
    Ast &at(size_t pos) const override;
    size_t indexOf(const Ast *child) const override;

protected:
    void doChange(size_t pos, Ast *next) override;

private:
    std::unique_ptr<Ast> type;
    std::unique_ptr<ScalarAst> id;
};

#endif // DECLPARAMAST_H
