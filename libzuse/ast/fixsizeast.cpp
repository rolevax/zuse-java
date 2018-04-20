#include "fixsizeast.h"

#include <algorithm>

template<std::size_t N>
void FixSizeAst<N>::dump() const
{
    std::cout << "(";
    for (size_t i = 0; i < N; i++) {
        mSubtrees[i]->dump();
        if (i != N - 1)
            std::cout << ", ";
    }
    std::cout << ")";
}

template<std::size_t N>
FixSizeAst<N> *FixSizeAst<N>::clone() const
{
    FixSizeAst<N> *ret = new FixSizeAst<N>(getType());
    ret->mModifiers = mModifiers;
    for (size_t i = 0; i < N; i++)
        ret->change(i, at(i).clone());
    return ret;
}

template<std::size_t N>
size_t FixSizeAst<N>::size() const
{
    return N;
}

template<std::size_t N>
Ast &FixSizeAst<N>::at(size_t pos) const
{
    assert(pos < N);
    return *mSubtrees[pos];
}

template<std::size_t N>
size_t FixSizeAst<N>::indexOf(const Ast *child) const
{
    auto eq = [child](const std::unique_ptr<Ast> &p) { return p.get() == child; };
    auto it = std::find_if(mSubtrees.begin(), mSubtrees.end(), eq);
    assert(it != mSubtrees.end());
    return it - mSubtrees.begin();
}

template<std::size_t N>
Modifiers &FixSizeAst<N>::getModifiers()
{
    return mModifiers;
}

template<std::size_t N>
const Modifiers &FixSizeAst<N>::getModifiers() const
{
    return mModifiers;
}

template<std::size_t N>
void FixSizeAst<N>::setModifiers(const Modifiers &m)
{
    mModifiers = m;
}

template<std::size_t N>
void FixSizeAst<N>::doChange(size_t pos, Ast *next)
{
    assert(pos < N);
    mSubtrees[pos].reset(next);
}

template<std::size_t N>
FixSizeAst<N>::FixSizeAst(Ast::Type t)
    : InternalAst(t)
{

}

// instantiation for the class
// there are also some related instantiation in ast.cpp
// and also do something in Doc::newTree()
template class FixSizeAst<1>;
template class FixSizeAst<2>;
template class FixSizeAst<3>;
template class FixSizeAst<4>;
template class FixSizeAst<5>;

Modifiers &FixSizes::getModifiers(Ast &ast)
{
    assert(ast.isFixSize());

    if (ast.isFixSize(1))
        return ast.asFixSize<1>().getModifiers();
    if (ast.isFixSize(2))
        return ast.asFixSize<2>().getModifiers();
    if (ast.isFixSize(3))
        return ast.asFixSize<3>().getModifiers();
    if (ast.isFixSize(4))
        return ast.asFixSize<4>().getModifiers();
    if (ast.isFixSize(5))
        return ast.asFixSize<5>().getModifiers();
    throw "WTF";
}
