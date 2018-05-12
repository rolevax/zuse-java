#ifndef ZUSE_AST_FIX_SIZE_H
#define ZUSE_AST_FIX_SIZE_H

#include "ast_internal.h"
#include "modifiers.h"

#include <array>
#include <cassert>



namespace zuse
{



///
/// \brief Fix-sized abstract syntax tree
/// \tparam N number of subtrees
///
template<std::size_t N>
class AstFixSize : public AstInternal
{
public:
    template<typename... T> AstFixSize(Type t, T... ts);
    virtual ~AstFixSize() = default;

    AstFixSize(AstFixSize &&move);
    AstFixSize &operator=(AstFixSize &&moveAssign);

    void dump() const override;
    std::unique_ptr<Ast> clone() const override;
    virtual size_t size() const override;
    virtual Ast &at(size_t pos) const override;
    virtual size_t indexOf(const Ast *child) const override;

    Modifiers &getModifiers();
    const Modifiers &getModifiers() const;
    void setModifiers(const Modifiers &m);

protected:
    virtual void doChange(size_t pos, std::unique_ptr<Ast> next) override;

private:
    explicit AstFixSize(Type t);

private:
    std::array<std::unique_ptr<Ast>, N> mSubtrees;
    Modifiers mModifiers;
};

///
/// \brief Construct by pointers to subtrees
/// \param type Node type
/// \param subtree Pointers to subtrees
/// \tparam T Pointer type movable to std::unique_ptr<Ast>
///
/// This definition is in the header because it's using
/// parameter pack which is tedious to explicitly instantiate.
/// The template class itself is explicitly instantiated in
/// the cpp file to avoid include loop.
///
template<std::size_t N>
template<typename... T>
AstFixSize<N>::AstFixSize(Type type, T... subtree)
    : AstInternal(type)
{
    static_assert(sizeof...(subtree) == N, "FixSizeAst bound check");
    assert(isFixSize(N));

    size_t index = 0;
    (change(index++, std::unique_ptr<Ast>(std::move(subtree))), ...);
}

namespace FixSizes
{

Modifiers &getModifiers(Ast &ast);

}



} // namespace zuse



#endif // ZUSE_AST_FIX_SIZE_H
