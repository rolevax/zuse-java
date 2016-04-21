#include "core/bonetoken.h"

#include <vector>
#include <cassert>

std::string BoneToken::syms[32]
{
    "class ", " {", "{", "}", "", "(", ")",
    "return ", "if ", "else ", "while ", "do", "for ",
    " ", ";", ".", ", ", "\"", "'",
    " = ", " + ", " - ", " * ", " / ", " || ", " && ", " | ", " ^ ", " & ",
    "??"
};

BoneToken::BoneToken(const Ast *ast, Sym sym) :
    Token(ast, Role::BONE),
    sym(sym)
{
}

BoneToken::BoneToken(const Ast *ast) :
    Token(ast, Role::BONE)
{
    assert(ast->isFixSize(2));
    switch (ast->getType()) {
    case Ast::Type::LOGIC_OR:
        sym = Sym::LOGIC_OR;
        break;
    case Ast::Type::LOGIC_AND:
        sym = Sym::LOGIC_AND;
        break;
    case Ast::Type::BIT_OR:
        sym = Sym::BIT_OR;
        break;
    case Ast::Type::BIT_XOR:
        sym = Sym::BIT_XOR;
        break;
    case Ast::Type::BIT_AND:
        sym = Sym::BIT_AND;
        break;
    case Ast::Type::ASSIGN:
        sym = Sym::ASSIGN;
        break;
    default:
        throw "unhandled";
    }
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

