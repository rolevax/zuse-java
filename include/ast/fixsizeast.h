#ifndef FIXSIZEAST_H
#define FIXSIZEAST_H

#include "internalast.h"

#include <array>
#include <algorithm>
#include <cassert>

template<std::size_t N>
class FixSizeAst : public InternalAst
{
public:
    template<typename... T> FixSizeAst(Type t, T... ts);

    void dump() const override;
    virtual size_t size() const override;
    virtual Ast &at(size_t pos) const override;
    virtual size_t indexOf(const Ast *child) const override;

protected:
    virtual void doChange(size_t pos, Ast *next) override;

private:
    std::array<std::unique_ptr<Ast>, N> subtrees;
};

// template definitions

template<std::size_t N>
void FixSizeAst<N>::dump() const
{
    for (const auto &p : subtrees)
        p->dump();
}

template<std::size_t N>
template<typename... T>
FixSizeAst<N>::FixSizeAst(Type t, T... ts)
    : InternalAst(t)
{
    static_assert(sizeof...(ts) == N, "FixSizeAst bound check");
    std::array<Ast*, N> tmp{ ts... };
    for (size_t i = 0; i < N; i++)
        subtrees[i].reset(tmp[i]);
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
    return *subtrees[pos];
}

template<std::size_t N>
size_t FixSizeAst<N>::indexOf(const Ast *child) const
{
    auto eq = [child](const std::unique_ptr<Ast> &p) { return p.get() == child; };
    auto it = std::find_if(subtrees.begin(), subtrees.end(), eq);
    assert(it != subtrees.end());
    return it - subtrees.begin();
}

template<std::size_t N>
void FixSizeAst<N>::doChange(size_t pos, Ast *next)
{
    assert(pos < N);
    subtrees[pos].reset(next);
}

#endif // FIXSIZEAST_H
