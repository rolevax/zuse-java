#ifndef LISTAST_H
#define LISTAST_H

#include "internalast.h"

#include <vector>

class ListAst : public InternalAst
{
public:
    static ListAst &fromAst(Ast &a);
    static const ListAst &fromAst(const Ast &a);

    ListAst(Type t);
    virtual ~ListAst() = default;

    void dump() const override
    {
        std::cout << '[';
        for (size_t i = 0; i < size(); i++) {
            const auto &t = at(i);
            t.dump();
            if (i != size() - 1)
                std::cout << ',';
        }
        std::cout << ']';
    }

    ListAst *clone() const override;
    size_t size() const override;
    Ast &at(size_t pos) const override;
    size_t indexOf(const Ast *child) const override;

    // TODO move to VarSizeAst
    void insert(size_t pos, Ast *child);
    void append(Ast *subtree);
    virtual std::unique_ptr<Ast> remove(size_t pos);

protected:
    virtual void doInsert(size_t pos, Ast *child); // TODO move?
    void doChange(size_t pos, Ast *next) override;

private:
    std::vector<std::unique_ptr<Ast>> subtrees;
};

#endif // LISTAST_H
