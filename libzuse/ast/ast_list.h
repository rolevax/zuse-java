#ifndef ZUSE_AST_LIST_H
#define ZUSE_AST_LIST_H

#include "ast_internal.h"

#include <vector>

class AstList : public AstInternal
{
public:
    AstList(Type t);
    virtual ~AstList() = default;

    void dump() const override;
    AstList *clone() const override;
    size_t size() const override;
    Ast &at(size_t pos) const override;
    size_t indexOf(const Ast *child) const override;

    void insert(size_t pos, Ast *child);
    void append(Ast *subtree);
    void erase(size_t pos);
    virtual Ast *remove(size_t pos);
    virtual void clear();

    bool illZero(bool assumeSize = false) const;
    bool illOne(bool assumeSize = false) const;

protected:
    virtual void doInsert(size_t pos, Ast *child); // TODO move?
    void doChange(size_t pos, Ast *next) override;

private:
    std::vector<std::unique_ptr<Ast>> mSubtrees;
};

#endif // ZUSE_AST_LIST_H
