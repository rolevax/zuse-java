#ifndef AST_H
#define AST_H

#include <cstddef>
#include <memory>

#include <iostream>

class Ast
{
public:
    /**
     * @brief type of a node in the language tree
     * KEYTAL means "keyword literal".
     */
    enum class Type {
        /* MUST sync with isList() functions when change */
        CLASS_LIST, METHOD_LIST, DECL_PARAM_LIST, STMT_LIST,
        ARG_LIST, DECL_BEAN_LIST,
        ADDSUB_LIST, MULDIV_LIST,
        /* MUST sync with isMap() functions when change */
        CLASS, METHOD, DECL, DECL_BEAN, DECL_PARAM, STMT,
        PAREN,
        ASSIGN, CALL,
        /* isScalar() == !isList() && !isMap() */
        IDENT, NUMBER,
    };

    Ast(Type t);
    Ast(const Ast &copy) = delete;
    Ast &operator=(const Ast &assign) = delete;
    virtual ~Ast() = default;

    virtual void dump() const {} // for debug use

    bool isList() const;
    bool isMap() const;
    bool isScalar() const;
    bool isChangeable() const;

    /**
     * @brief Recursive value deep copy
     * @return Ast with same value but parent is null
     */
    virtual Ast *clone() const { return nullptr; }

    Type getType() const;
    Ast &getParent() const;

    int indentLevel() const;

private:
    Type type;
public: // TODO: properly handle this
    Ast *parent;
};

#endif // AST_H
