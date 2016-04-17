#include "core/bonetoken.h"

#include <vector>
#include <cassert>

std::string BoneToken::syms[32]
{
    "class ", " {", "{", "}", "", "(", ")",
    "return ", "if ", "else ", "while ", "do", "for ",
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
    static std::vector<std::string> tabBrace;
    static std::vector<std::string> tabOnly;

    if (sym == Sym::RBRACE || sym == Sym::RBRACE_VT) {
        size_t level = getAst()->indentLevel();
        if (level >= tabBrace.size()) {
            for (size_t i = tabBrace.size(); i <= level; i++) {
                tabOnly.emplace_back(i * 4, ' ');
                tabBrace.push_back(tabOnly.back() + '}');
            }
        }
        return sym == Sym::RBRACE ? tabBrace[level] : tabOnly[level];
    } else {
        return syms[static_cast<size_t>(sym)];
    }
}

