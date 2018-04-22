#include "ast_list.h"
#include "ast_scalar.h"

#include <algorithm>
#include <cassert>

AstList::AstList(Type t)
    : AstInternal(t)
{
    assert(isList());
}

void AstList::dump() const
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

AstList *AstList::clone() const
{
    AstList *ret = new AstList(getType());
    for (size_t i = 0; i < size(); i++)
        ret->append(at(i).clone());
    return ret;
}

size_t AstList::size() const
{
    return mSubtrees.size();
}

Ast &AstList::at(size_t pos) const
{
    return *mSubtrees[pos];
}

size_t AstList::indexOf(const Ast *child) const
{
    auto eq = [child](const std::unique_ptr<Ast> &a) {
        return a.get() == child;
    };
    auto it = std::find_if(mSubtrees.begin(), mSubtrees.end(), eq);

    if (it == mSubtrees.end())
        return assert(false && "child not found"), 0;
    else
        return it - mSubtrees.begin();
}

void AstList::insert(size_t pos, Ast *child)
{
    child->setParent(this);
    doInsert(pos, child);
}

void AstList::append(Ast *subtree)
{
    insert(size(), subtree);
}

void AstList::erase(size_t pos)
{
    delete remove(pos);
}

Ast *AstList::remove(size_t pos)
{
    assert(pos < mSubtrees.size());
    Ast *res = mSubtrees[pos].release();
    mSubtrees.erase(mSubtrees.begin() + pos);
    return res;
}

void AstList::clear()
{
    mSubtrees.clear();
}

bool AstList::illZero(bool assumeSize) const
{
    if (!assumeSize && 0 != size())
        return false;

    if (illOne(true)) // assume all ill-one is also ill-zero
        return true;

    Type t = getType();
    return t == Type::TRY_LIST
            || t == Type::NAME_LIST;
}

bool AstList::illOne(bool assumeSize) const
{
    if (!assumeSize && 1 != size())
        return false;

    if (isBopList()) // binary operator must be binary
        return true;

    Ast::Type t = getType();
    return t == Type::DECTOR_LIST
            || t == Type::IF_LIST;
}

void AstList::doInsert(size_t pos, Ast *child)
{
    mSubtrees.emplace(mSubtrees.begin() + pos, child);
}

void AstList::doChange(size_t pos, Ast *next)
{
    assert(pos < mSubtrees.size());
    mSubtrees[pos].reset(next);
}

