#include "ast/scalarast.h"

#include <cassert>

ScalarAst::ScalarAst(Type t, const std::string &text)
    : Ast(t),
      mText(text)
{
    assert(isScalar());
}

void ScalarAst::dump() const
{
    std::cout << mText;
}

ScalarAst *ScalarAst::clone() const
{
    ScalarAst *ret = new ScalarAst(getType(), mText);
    return ret;
}

const std::string &ScalarAst::getText() const
{
    return mText;
}

void ScalarAst::append(char c)
{
    if (c == '\b' && !mText.empty()) {
        mText.pop_back();
    } else {
        mText += c;
    }
}

void ScalarAst::clear()
{
    mText.clear();
}



