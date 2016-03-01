#ifndef AST_H
#define AST_H

#include <cstddef>
#include <memory>
#include <iostream> // for Ast::dump()

class ScalarAst;
class InternalAst;
template<std::size_t N> class FixSizeAst;
class ListAst;
class BopListAst;

class Ast
{
public:
    /**
     * @brief type of a node in the language tree
     * KEYTAL means "keyword literal".
     */
    enum class Type {
        /* MUST sync with isList() definition */
        CLASS_LIST, MEMBER_LIST, DECL_PARAM_LIST, STMT_LIST,
        IF_LIST,
        COMMA_LIST,

        /* MUST sync with isBopList() and isList() definition */
        DOT_BOP_LIST, MUL_BOP_LIST, ADD_BOP_LIST,

        /* MUST sync with isMap() definition */
        CLASS, METHOD, DECL_STMT, DECL_PARAM, STMT,
        RETURN, WHILE, DO_WHILE, IF_CONDBODY, IF_ELSEBODY,
        PAREN,
        ASSIGN,

        /* isScalar() == !isList() && !isMap() */
        IDENT, NUMBER, STRING, META
    };

    Ast(Type t);
    Ast(const Ast &copy) = delete;
    Ast &operator=(const Ast &assign) = delete;
    virtual ~Ast() = default;

    virtual void dump() const {} // for debug use

    bool isList() const;
    bool isBopList() const;
    bool isMap() const;
    bool isScalar() const;

    ScalarAst &asScalar();
    const ScalarAst &asScalar() const;
    InternalAst &asInternal();
    const InternalAst &asInternal() const;
    template<std::size_t N> FixSizeAst<N> &asFixSize();
    template<std::size_t N> const FixSizeAst<N> &asFixSize() const;
    ListAst &asList();
    const ListAst &asList() const;
    BopListAst &asBopList();
    const BopListAst &asBopList() const;

    ListAst *bodify();
    int precedence() const;

    /**
     * @brief Recursive value deep copy
     * @return Ast with same value but parent is null
     */
    virtual Ast *clone() const = 0;

    Type getType() const;
    InternalAst &getParent() const;
    void setParent(InternalAst *next);

    int indentLevel() const;

private:
    Type type;
    InternalAst *parent;
};

#endif // AST_H
