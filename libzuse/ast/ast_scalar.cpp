#include "ast_scalar.h"

#include <cassert>

AstScalar::AstScalar(Type t, const std::string &text)
    : Ast(t),
      mText(text)
{
    assert(isScalar());
}

void AstScalar::dump() const
{
    std::cout << mText;
}

AstScalar *AstScalar::clone() const
{
    AstScalar *ret = new AstScalar(getType(), mText);
    return ret;
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



