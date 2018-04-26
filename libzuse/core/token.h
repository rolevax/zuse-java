#ifndef ZUSE_TOKEN_H
#define ZUSE_TOKEN_H

#include "../ast/ast.h"

#include <string>



namespace zuse
{



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

    Token(const Ast *mAst, Role mRole);
    Token(const Token &copy) = delete;
    Token &operator=(const Token &assign) = delete;
    virtual ~Token() = default;

    const Ast *getAst() const;
    Role getRole() const;
    virtual const std::string &getText() const = 0;

private:
    const Ast *mAst;
    Role mRole;
};



} // namespace zuse



#endif // ZUSE_TOKEN_H
