#ifndef ZUSE_AST_LIST_H
#define ZUSE_AST_LIST_H

#include "ast_internal.h"

#include <vector>

class AstList : public AstInternal
{
public:
    AstList(Type t);
    virtual ~AstList() = default;

    AstList(AstList &&move) = default;
    AstList &operator=(AstList &&moveAssign) = default;

    void dump() const override;
    std::unique_ptr<Ast> clone() const override;
    size_t size() const override;
    Ast &at(size_t pos) const override;
    size_t indexOf(const Ast *child) const override;

    void insert(size_t pos, std::unique_ptr<Ast> child);
    void append(std::unique_ptr<Ast> subtree);
    void append(Ast *subtree);
    void erase(size_t pos);
    virtual std::unique_ptr<Ast> remove(size_t pos);
    virtual void clear();

    bool illZero(bool assumeSize = false) const;
    bool illOne(bool assumeSize = false) const;

protected:
    virtual void doInsert(size_t pos, std::unique_ptr<Ast> child); // TODO move?
    void doChange(size_t pos, std::unique_ptr<Ast> next) override;

private:
    std::vector<std::unique_ptr<Ast>> mSubtrees;
};

#endif // ZUSE_AST_LIST_H
