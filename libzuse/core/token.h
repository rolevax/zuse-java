#ifndef ZUSE_TOKEN_H
#define ZUSE_TOKEN_H

#include "../ast/ast.h"

#include <string>



namespace zuse
{



///
/// \brief A selection region in a text file
///
struct Region
{
    size_t br; ///< begin of row
    size_t bc; ///< begin of column
    size_t er; ///< end of row
    size_t ec; ///< end of column
};



///
/// \brief A token in source files
///
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
