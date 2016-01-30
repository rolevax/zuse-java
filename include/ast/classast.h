#ifndef CLASSAST_H
#define CLASSAST_H

#include "ast/listast.h"
#include "ast/scalarast.h"

#include <cassert>

class ClassAst : public Ast
{
public:
    static ClassAst &fromAst(Ast &a);
    static const ClassAst &fromAst(const Ast &a);

    ClassAst(const std::string &id, ListAst *methods);

    void dump() const override
    {
        std::cout << "class " << id->getText() << ';' << std::endl;
        methods->dump();
    }
    size_t size() const override;
    Ast &at(size_t pos) const override;
    size_t indexOf(const Ast *child) const override;

protected:
    void doChange(size_t pos, Ast *next) override;

private:
    std::unique_ptr<ScalarAst> id;
    std::unique_ptr<ListAst> methods;
};


#endif // CLASSAST_H
