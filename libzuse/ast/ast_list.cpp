#include "ast_list.h"
#include "ast_scalar.h"

#include <algorithm>
#include <cassert>


namespace zuse
{



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

std::unique_ptr<Ast> AstList::clone() const
{
    auto res = std::make_unique<AstList>(getType());
    for (size_t i = 0; i < size(); i++)
        res->append(at(i).clone());
    return res;
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

void AstList::insert(size_t pos, std::unique_ptr<Ast> child)
{
    child->setParent(this);
    doInsert(pos, std::move(child));
}

void AstList::append(std::unique_ptr<Ast> subtree)
{
    insert(size(), std::move(subtree));
}

void AstList::append(util::Owner<Ast *> subtree)
{
    append(std::unique_ptr<Ast>(subtree));
}

std::unique_ptr<Ast> AstList::remove(size_t pos)
{
    assert(pos < mSubtrees.size());
    auto res = std::move(mSubtrees[pos]);
    mSubtrees.erase(mSubtrees.begin() + pos);
    return res;
}

void AstList::clear()
{
    mSubtrees.clear();
}

///
/// \return True if this node has no subtree and is therefore ill-formed
/// \param assumeSize Assume the size is zero
///
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

///
/// \return True if this node has onne subtree and is therefore ill-formed
/// \param assumeSize Assume the size is one
///
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

void AstList::doInsert(size_t pos, std::unique_ptr<Ast> child)
{
    mSubtrees.emplace(mSubtrees.begin() + pos, std::move(child));
}

void AstList::doChange(size_t pos, std::unique_ptr<Ast> next)
{
    assert(pos < mSubtrees.size());
    mSubtrees[pos] = std::move(next);
}



} // namespace zuse
