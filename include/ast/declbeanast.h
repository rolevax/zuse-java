#ifndef DECLBEANAST_H
#define DECLBEANAST_H

#include "scalarast.h"
#include "internalast.h"

class DeclBeanAst : public InternalAst
{
public:
    static DeclBeanAst &fromAst(Ast &a);
    static const DeclBeanAst &fromAst(const Ast &a);

    DeclBeanAst(Ast *id, Ast *init = nullptr);

    void dump() const override
    {
        std::cout << "declb " << id->getText();
        if (init) {
            std::cout << '-';
            init->dump();
        }
    }
    DeclBeanAst *clone() const override;
    size_t size() const override;
    Ast &at(size_t pos) const override;
    size_t indexOf(const Ast *child) const override;

protected:
    void doChange(size_t pos, Ast *next) override;

private:
    std::unique_ptr<ScalarAst> id;
    std::unique_ptr<Ast> init;
};

#endif // DECLBEANAST_H
