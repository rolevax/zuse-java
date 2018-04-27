#ifndef ZUSE_TOKEN_BONE_H
#define ZUSE_TOKEN_BONE_H

#include "token.h"



namespace zuse
{



///
/// \brief A token which structures the source code
///
class TokenBone : public Token
{
public:
    enum class Sym
    {
        // MUST sync order with 'syms'
        CLASS, INTERFACE, EXTENDS, IMPLEMENTS,
        LBRACE, LBRACE_NS, RBRACE, RBRACE_VT, LPAREN, RPAREN, LSQUARE, RSQUARE,
        IF, ELSE, WHILE, DO, FOR,
        RETURN, CONTINUE, BREAK, THROW, THROWS, NEW,
        TRY, CATCH, FINALLY,
        ABSTRACT, FINAL, PUBLIC, PROTECTED, PRIVATE, STATIC,
        TRANSIENT, VOLATILE, NATIVE, SYNCHRONIZED,
        SPACE, SEMICOLON, DOT, COMMA, DQUOTE, SQUOTE,
        ASSIGN, ASS_ADD, ASS_SUB, ASS_MUL, ASS_DIV, ASS_MOD,
        ASS_AND, ASS_XOR, ASS_OR, ASS_SHL, ASS_SHR, ASS_SHRA,
        ADD, SUB, MUL, DIV, MOD, LOGIC_OR, LOGIC_AND, BIT_OR, BIT_XOR, BIT_AND,
        LT, LEQ, GT, GEQ, INSTANCEOF, EQ, NEQ,
        SHL, SHR, SHRA, INC, DEC, UNARY_PLUS, UNARY_MINUS, LOGIC_NOT, BIT_NOT,
        QUESTION, COLON,
        META
    };

    TokenBone(const Ast *mAst, Sym mSym);
    TokenBone(const Ast *mAst);

    const std::string &getText() const override;

private:
    Sym mSym;
    static std::string mSyms[128]; // MUST SYNC WITH # OF ENUM
};



} // namespace zuse



#endif // ZUSE_TOKEN_BONE_H
