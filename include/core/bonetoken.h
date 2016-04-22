#ifndef BONETOKEN_H
#define BONETOKEN_H

#include "core/token.h"

class BoneToken : public Token
{
public:
    enum class Sym
    {
        // MUST sync order with 'syms'
        CLASS, LBRACE, LBRACE_NS, RBRACE, RBRACE_VT, LPAREN, RPAREN,
        RETURN, IF, ELSE, WHILE, DO, FOR,
        SPACE, SEMICOLON, DOT, COMMA, DQUOTE, SQUOTE,
        ASSIGN, ADD, SUB, MUL, DIV, LOGIC_OR, LOGIC_AND, BIT_OR, BIT_XOR, BIT_AND,
        LT, LEQ, GT, GEQ, INSTANCEOF, EQ, NEQ,
        SHL, SHR, SHRA,
        META
    };

    BoneToken(const Ast *ast, Sym sym);
    BoneToken(const Ast *ast);

    const std::string &getText() const override;

private:
    Sym sym;
    static std::string syms[64]; // MUST SYNC WITH # OF ENUM
};

#endif // BONETOKEN_H
