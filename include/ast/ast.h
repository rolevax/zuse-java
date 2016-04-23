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
        // LIST, sync with isList() definition
        CLASS_LIST, MEMBER_LIST, DECL_PARAM_LIST, STMT_LIST,
        IF_LIST,
        DECTOR_LIST, ARG_LIST,

        // BOP-LIST, sync with isBopList() and isList() definition
        DOT_BOP_LIST, MUL_BOP_LIST, ADD_BOP_LIST,

        // FIX-SIZE, sync with isFixSize() definition
        // size 1
        RETURN, POST_INC, POST_DEC, PRE_INC, PRE_DEC,
        UNARY_PLUS, UNARY_MINUS, LOGIC_NOT, BIT_NOT,
        // size 2
        DECL_CLASS, DECL_VAR, DECL_PARAM, WHILE, DO_WHILE, IF_CONDBODY,
        CAST,
        LT, LEQ, GT, GEQ, INSTANCEOF, EQ, NEQ, SHL, SHR, SHRA,
        LOGIC_OR, LOGIC_AND, BIT_OR, BIT_XOR, BIT_AND,
        ASSIGN, ASS_ADD, ASS_SUB, ASS_MUL, ASS_DIV, ASS_MOD,
        ASS_AND, ASS_XOR, ASS_OR, ASS_SHL, ASS_SHR, ASS_SHRA,
        // size 3
        QUESTION,
        // size 4
        DECL_METHOD,

        // SCALAR, isScalar() := !isList() && !isFixSize()
        IDENT, NUMBER, STRING, META
    };

    Ast(Type t);
    Ast(const Ast &copy) = delete;
    Ast &operator=(const Ast &assign) = delete;
    virtual ~Ast() = default;

    virtual void dump() const {} // for debug use

    static bool isList(Type type);
    static bool isBopList(Type type);
    static bool isFixSize(Type type, size_t s = 0);
    static bool isScalar(Type type);
    bool isList() const;
    bool isBopList() const;
    bool isFixSize(size_t s = 0) const;
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

    bool hasIndent() const;
    int indentLevel() const;

private:
    Type type;
    InternalAst *parent;
};

#endif // AST_H
