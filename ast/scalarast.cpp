#include "ast/scalarast.h"

#include <cassert>

ScalarAst::ScalarAst(Type t, const std::string &text)
    : Ast(t),
      text(text)
{

}

void ScalarAst::dump() const
{
    std::cout << text;
}

ScalarAst *ScalarAst::clone() const
{
    ScalarAst *ret = new ScalarAst(getType(), text);
    return ret;
}

const std::string &ScalarAst::getText() const
{
    return text;
}

void ScalarAst::append(char c)
{
    if (c == '\b' && !text.empty()) {
        text.pop_back();
    } else {
        text += c;
    }
}

void ScalarAst::clear()
{
    text.clear();
}



