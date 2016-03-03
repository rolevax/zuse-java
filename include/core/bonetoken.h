#ifndef BONETOKEN_H
#define BONETOKEN_H

#include "core/token.h"

class BoneToken : public Token
{
public:
    enum class Sym
    {
        // MUST sync order with 'syms'
        CLASS, LBRACE, LBRACE_NS, RBRACE, LPAREN, RPAREN,
        RETURN, IF, ELSE, WHILE, DO, FOR,
        SPACE, SEMICOLON, DOT, COMMA, DQUOTE, SQUOTE,
        ASSIGN, ADD, SUB, MUL, DIV,
        META
    };

    BoneToken(const Ast *ast, Sym sym);

    const std::string &getText() const override;

private:
    Sym sym;
    static std::string syms[32]; // MUST SYNC WITH # OF ENUM
};

#endif // BONETOKEN_H
