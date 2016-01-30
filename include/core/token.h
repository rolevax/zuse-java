#ifndef TOKEN_H
#define TOKEN_H

#include "ast/ast.h"
#include <string>

struct Region
{
    size_t br;
    size_t bc;
    size_t er;
    size_t ec;
};

class Token
{
public:
    enum class Role {
        BEGIN, END, META, BONE, FLESH
    };

    Token(const Ast *ast, Role role);
    Token(const Token &copy) = delete;
    Token &operator=(const Token &assign) = delete;
    virtual ~Token() = default;

    const Ast *getAst() const;
    Role getRole() const;
    virtual const std::string &getText() const = 0;

private:
    const Ast *ast;
    Role role;
};

#endif // TOKEN_H
