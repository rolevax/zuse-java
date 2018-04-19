#ifndef FIXSIZEAST_H
#define FIXSIZEAST_H

#include "internalast.h"
#include "modifiers.h"

#include <array>
#include <cassert>

template<std::size_t N>
class FixSizeAst : public InternalAst
{
public:
    template<typename... T> FixSizeAst(Type t, T... ts);

    void dump() const override;
    FixSizeAst<N> *clone() const override;
    virtual size_t size() const override;
    virtual Ast &at(size_t pos) const override;
    virtual size_t indexOf(const Ast *child) const override;
    Modifiers &getModifiers();
    const Modifiers &getModifiers() const;
    void setModifiers(const Modifiers &m);

protected:
    virtual void doChange(size_t pos, Ast *next) override;

private:
    explicit FixSizeAst(Type t);

private:
    std::array<std::unique_ptr<Ast>, N> mSubtrees;
    Modifiers mModifiers;
};

/**
 * This definition is in the header because it's using
 * parameter pack which is tedious to be explicitly instantiated.
 * The template class itself is explicitly instantiated in
 * the cpp file because putting everything in headers might
 * lead headers to mutually include each other.
 */
template<std::size_t N>
template<typename... T>
FixSizeAst<N>::FixSizeAst(Type t, T... ts)
    : InternalAst(t)
{
    static_assert(sizeof...(ts) == N, "FixSizeAst bound check");
    assert(isFixSize(N));
    std::array<Ast*, N> tmp{ ts... };
    for (size_t i = 0; i < N; i++)
        change(i, tmp[i]);
}

namespace FixSizes
{

Modifiers &getModifiers(Ast &ast);

}

#endif // FIXSIZEAST_H
