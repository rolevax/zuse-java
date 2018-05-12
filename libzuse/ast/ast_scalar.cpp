#include "ast_scalar.h"

#include <cassert>



namespace zuse
{



AstScalar::AstScalar(Type t, const std::string &text)
    : Ast(t),
      mText(text)
{
    assert(isScalar());
}

void AstScalar::visit(std::function<void (const Ast &)> f) const
{
    f(*this);
}

void AstScalar::dump() const
{
    std::cout << mText;
}

std::unique_ptr<Ast> AstScalar::clone() const
{
    return std::make_unique<AstScalar>(getType(), mText);
}

const std::string &AstScalar::getText() const
{
    return mText;
}

void AstScalar::append(char c)
{
    if (c == '\b' && !mText.empty()) {
        mText.pop_back();
    } else {
        mText += c;
    }
}

void AstScalar::clear()
{
    mText.clear();
}



} // namespace zuse
