#include "ast_fix_size.h"

#include <algorithm>


namespace zuse
{



template<std::size_t N>
void AstFixSize<N>::dump() const
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
std::unique_ptr<Ast> AstFixSize<N>::clone() const
{
    AstFixSize<N> res(getType());
    res.mModifiers = mModifiers;
    for (size_t i = 0; i < N; i++)
        res.change(i, at(i).clone());

    return std::make_unique<AstFixSize<N>>(std::move(res));
}

template<std::size_t N>
size_t AstFixSize<N>::size() const
{
    return N;
}

template<std::size_t N>
Ast &AstFixSize<N>::at(size_t pos) const
{
    assert(pos < N);
    return *mSubtrees[pos];
}

template<std::size_t N>
size_t AstFixSize<N>::indexOf(const Ast *child) const
{
    auto eq = [child](const std::unique_ptr<Ast> &p) { return p.get() == child; };
    auto it = std::find_if(mSubtrees.begin(), mSubtrees.end(), eq);
    assert(it != mSubtrees.end());
    return it - mSubtrees.begin();
}

template<std::size_t N>
Modifiers &AstFixSize<N>::getModifiers()
{
    return mModifiers;
}

template<std::size_t N>
const Modifiers &AstFixSize<N>::getModifiers() const
{
    return mModifiers;
}

template<std::size_t N>
void AstFixSize<N>::setModifiers(const Modifiers &m)
{
    mModifiers = m;
}

template<std::size_t N>
void AstFixSize<N>::doChange(size_t pos, std::unique_ptr<Ast> next)
{
    assert(pos < N);
    mSubtrees[pos] = std::move(next);
}

template<std::size_t N>
AstFixSize<N>::AstFixSize(Ast::Type t)
    : AstInternal(t)
{

}

// instantiation for the class
// there are also some related instantiation in ast.cpp
// and also do something in Doc::newTree()
template class AstFixSize<1>;
template class AstFixSize<2>;
template class AstFixSize<3>;
template class AstFixSize<4>;
template class AstFixSize<5>;

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



} // namespace zuse
