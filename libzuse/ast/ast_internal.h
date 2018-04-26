#ifndef ZUSE_AST_INTERNAL_H
#define ZUSE_AST_INTERNAL_H

#include "ast.h"



namespace zuse
{



class AstInternal : public Ast
{
public:
    AstInternal(Type t);
    virtual ~AstInternal() = default;

    AstInternal(AstInternal &&move) = default;
    AstInternal &operator=(AstInternal &&moveAssign) = default;

    void change(size_t pos, std::unique_ptr<Ast> next);
    void nestAsLeft(size_t pos, std::unique_ptr<AstInternal> nester);
    void nestAsRight(size_t pos, std::unique_ptr<AstInternal> nester);
    void expose(size_t vanisher, size_t exposee);
    virtual size_t size() const = 0;
    virtual Ast &at(size_t pos) const = 0;
    virtual size_t indexOf(const Ast *child) const = 0;

    static Ast::Type typeAt(Type mType, size_t pos, bool allowHidden = true);
    Ast::Type typeAt(size_t pos, bool allowHidden = true) const;

protected:
    virtual void doChange(size_t pos, std::unique_ptr<Ast> next) = 0;
};



} // namespace zuse



#endif // ZUSE_AST_INTERNAL_H
