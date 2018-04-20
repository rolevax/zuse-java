#include "token_bone.h"

#include <vector>
#include <cassert>

std::string TokenBone::mSyms[128]
{
    "class ", "interface ", " extends ", " implements ",
    " {", "{", "}", "", "(", ")", "[", "]",
    "if ", "else ", "while ", "do", "for ",
    "return",  "continue", "break", "throw ", " throws ", "new ",
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

TokenBone::TokenBone(const Ast *ast, Sym sym) :
    Token(ast, Role::BONE),
    mSym(sym)
{
}

TokenBone::TokenBone(const Ast *ast) :
    Token(ast, Role::BONE)
{
    switch (ast->getType()) {
    case Ast::Type::SHL:
        mSym = Sym::SHL;
        break;
    case Ast::Type::SHR:
        mSym = Sym::SHR;
        break;
    case Ast::Type::SHRA:
        mSym = Sym::SHRA;
        break;
    case Ast::Type::LT:
        mSym = Sym::LT;
        break;
    case Ast::Type::LEQ:
        mSym = Sym::LEQ;
        break;
    case Ast::Type::GT:
        mSym = Sym::GT;
        break;
    case Ast::Type::GEQ:
        mSym = Sym::GEQ;
        break;
    case Ast::Type::INSTANCEOF:
        mSym = Sym::INSTANCEOF;
        break;
    case Ast::Type::EQ:
        mSym = Sym::EQ;
        break;
    case Ast::Type::NEQ:
        mSym = Sym::NEQ;
        break;
    case Ast::Type::BIT_OR:
        mSym = Sym::BIT_OR;
        break;
    case Ast::Type::BIT_XOR:
        mSym = Sym::BIT_XOR;
        break;
    case Ast::Type::BIT_AND:
        mSym = Sym::BIT_AND;
        break;
    case Ast::Type::ASSIGN:
        mSym = Sym::ASSIGN;
        break;
    case Ast::Type::ASS_ADD:
        mSym = Sym::ASS_ADD;
        break;
    case Ast::Type::ASS_SUB:
        mSym = Sym::ASS_SUB;
        break;
    case Ast::Type::ASS_MUL:
        mSym = Sym::ASS_MUL;
        break;
    case Ast::Type::ASS_DIV:
        mSym = Sym::ASS_DIV;
        break;
    case Ast::Type::ASS_MOD:
        mSym = Sym::ASS_MOD;
        break;
    case Ast::Type::ASS_AND:
        mSym = Sym::ASS_AND;
        break;
    case Ast::Type::ASS_XOR:
        mSym = Sym::ASS_XOR;
        break;
    case Ast::Type::ASS_OR:
        mSym = Sym::ASS_OR;
        break;
    case Ast::Type::ASS_SHL:
        mSym = Sym::ASS_SHL;
        break;
    case Ast::Type::ASS_SHR:
        mSym = Sym::ASS_SHR;
        break;
    case Ast::Type::ASS_SHRA:
        mSym = Sym::ASS_SHRA;
        break;
    case Ast::Type::POST_INC:
    case Ast::Type::PRE_INC:
        mSym = Sym::INC;
        break;
    case Ast::Type::POST_DEC:
    case Ast::Type::PRE_DEC:
        mSym = Sym::DEC;
        break;
    case Ast::Type::UNARY_PLUS:
        mSym = Sym::UNARY_PLUS;
        break;
    case Ast::Type::UNARY_MINUS:
        mSym = Sym::UNARY_MINUS;
        break;
    case Ast::Type::LOGIC_NOT:
        mSym = Sym::LOGIC_NOT;
        break;
    case Ast::Type::BIT_NOT:
        mSym = Sym::BIT_NOT;
        break;
    default:
        throw "unhandled";
    }
}

const std::string &TokenBone::getText() const
{
    static std::vector<std::string> tabBrace;
    static std::vector<std::string> tabOnly;

    if (mSym == Sym::RBRACE || mSym == Sym::RBRACE_VT) {
        size_t level = getAst()->indentLevel();
        if (level >= tabBrace.size()) {
            for (size_t i = tabBrace.size(); i <= level; i++) {
                tabOnly.emplace_back(i * 4, ' ');
                tabBrace.push_back(tabOnly.back() + '}');
            }
        }
        return mSym == Sym::RBRACE ? tabBrace[level] : tabOnly[level];
    } else {
        return mSyms[static_cast<size_t>(mSym)];
    }
}

