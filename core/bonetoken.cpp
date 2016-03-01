#include "core/bonetoken.h"

#include <vector>
#include <cassert>

std::string BoneToken::syms[32]
{
    "class ", " {", "{", "}", "void ", "(", ")",
    "return ", "if ", " else ", "while ", "do", "for ",
    " ", ";", ".", ", ", "\"", "'",
    " = ", " + ", " - ", " * ", " / ",
    "??"
};

BoneToken::BoneToken(const Ast *ast, Sym sym) :
    Token(ast, Role::BONE),
    sym(sym)
{
}

const std::string &BoneToken::getText() const
{
    static std::vector<std::string> tab;

    if (sym == Sym::RBRACE) {
        size_t level = getAst()->indentLevel();
        if (level >= tab.size())
            for (size_t i = 0; i <= level; i++)
                tab.push_back(std::string(i * 4, ' ') + '}');
        return tab[level];
    } else {
        return syms[static_cast<size_t>(sym)];
    }
}

