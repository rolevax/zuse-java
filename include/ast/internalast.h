#ifndef INTERNALAST_H
#define INTERNALAST_H

#include "ast.h"

class InternalAst : public Ast
{
public:
    InternalAst(Type t);
    virtual ~InternalAst() = default;

    void change(size_t pos, Ast *next);
    void nestAsLeft(size_t pos, InternalAst *nester);
    void expose(size_t vanisher, size_t exposee);
    virtual size_t size() const = 0;
    virtual Ast &at(size_t pos) const = 0;
    virtual size_t indexOf(const Ast *child) const = 0;

    static Ast::Type typeAt(Type type, size_t pos);
    Ast::Type typeAt(size_t pos) const;

protected:
    virtual void doChange(size_t pos, Ast *next) = 0;
};

#endif // INTERNALAST_H
