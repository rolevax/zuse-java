#ifndef ZUSE_AST_SCALAR_H
#define ZUSE_AST_SCALAR_H

#include "ast.h"
#include <string>

#include <iostream>

namespace zuse
{



///
/// \brief Scalar abstract syntax tree nnode
///
class AstScalar : public Ast
{
public:
    AstScalar(Type t, const std::string &mText);

    AstScalar(AstScalar &&move) = default;
    AstScalar &operator=(AstScalar &&moveAssign) = default;

    void visit(std::function<void (const Ast &)> f) const override;
    void dump() const override;
    std::unique_ptr<Ast> clone() const override;

    const std::string &getText() const;

    void append(char c);
    void clear();

private:
    std::string mText;
};



} // namespace zuse



#endif // ZUSE_AST_SCALAR_H
