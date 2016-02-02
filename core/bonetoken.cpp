#include "core/bonetoken.h"

#include <cassert>

std::string BoneToken::syms[32]
{
    "class ", " {", "}", "void ", "(", ")",
    " ", ";", ", ",
    " = ", " + ", " - ", " * ", " / "
};

BoneToken::BoneToken(const Ast *ast, Sym sym) :
    Token(ast, Role::BONE),
    sym(sym)
{
}

const std::string &BoneToken::getText() const
{
    static const std::string tab[4] = {
        "}", "    }", "        }", "            }"
    };

    if (sym == Sym::RBRACE)
        return tab[getAst()->indentLevel()];
    else
        return syms[static_cast<size_t>(sym)];
}

