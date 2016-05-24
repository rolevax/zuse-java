#include "core/bonetoken.h"

#include <vector>
#include <cassert>

std::string BoneToken::syms[128]
{
    "class ", " {", "{", "}", "", "(", ")",
    "return ", "if ", "else ", "while ", "do", "for ",
    "try", " catch ", " finally",
    "abstract ", "final ", "public ", "protected ", "private ", "static ",
    "transient ", "volatile ", "native ", "synchronized ",
    " ", ";", ".", ", ", "\"", "'",
    " = ", " += ", " -= ", " *= ", " /= ", " %= ",
    " &= ", " ^= ", " |= ", " <<= ", " >>= ", " >>>= ",
    " + ", " - ", " * ", " / ", " % ", " || ", " && ", " | ", " ^ ", " & ",
    " < ", " <= ", " > ", " >= ", " instanceof ", " == ", " != ",
    " << ", " >> ", " >>> ", "++", "--", "+", "-", "!", "~",
    " ? ", " : ",
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
    switch (ast->getType()) {
    case Ast::Type::SHL:
        sym = Sym::SHL;
        break;
    case Ast::Type::SHR:
        sym = Sym::SHR;
        break;
    case Ast::Type::SHRA:
        sym = Sym::SHRA;
        break;
    case Ast::Type::LT:
        sym = Sym::LT;
        break;
    case Ast::Type::LEQ:
        sym = Sym::LEQ;
        break;
    case Ast::Type::GT:
        sym = Sym::GT;
        break;
    case Ast::Type::GEQ:
        sym = Sym::GEQ;
        break;
    case Ast::Type::INSTANCEOF:
        sym = Sym::INSTANCEOF;
        break;
    case Ast::Type::EQ:
        sym = Sym::EQ;
        break;
    case Ast::Type::NEQ:
        sym = Sym::NEQ;
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
    case Ast::Type::ASS_ADD:
        sym = Sym::ASS_ADD;
        break;
    case Ast::Type::ASS_SUB:
        sym = Sym::ASS_SUB;
        break;
    case Ast::Type::ASS_MUL:
        sym = Sym::ASS_MUL;
        break;
    case Ast::Type::ASS_DIV:
        sym = Sym::ASS_DIV;
        break;
    case Ast::Type::ASS_MOD:
        sym = Sym::ASS_MOD;
        break;
    case Ast::Type::ASS_AND:
        sym = Sym::ASS_AND;
        break;
    case Ast::Type::ASS_XOR:
        sym = Sym::ASS_XOR;
        break;
    case Ast::Type::ASS_OR:
        sym = Sym::ASS_OR;
        break;
    case Ast::Type::ASS_SHL:
        sym = Sym::ASS_SHL;
        break;
    case Ast::Type::ASS_SHR:
        sym = Sym::ASS_SHR;
        break;
    case Ast::Type::ASS_SHRA:
        sym = Sym::ASS_SHRA;
        break;
    case Ast::Type::POST_INC:
    case Ast::Type::PRE_INC:
        sym = Sym::INC;
        break;
    case Ast::Type::POST_DEC:
    case Ast::Type::PRE_DEC:
        sym = Sym::DEC;
        break;
    case Ast::Type::UNARY_PLUS:
        sym = Sym::UNARY_PLUS;
        break;
    case Ast::Type::UNARY_MINUS:
        sym = Sym::UNARY_MINUS;
        break;
    case Ast::Type::LOGIC_NOT:
        sym = Sym::LOGIC_NOT;
        break;
    case Ast::Type::BIT_NOT:
        sym = Sym::BIT_NOT;
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

