#ifndef ZUSE_AST_H
#define ZUSE_AST_H

#include "../util/pointers.h"

#include <cstddef>
#include <memory>
#include <iostream> // for Ast::dump()



namespace zuse
{



class AstScalar;
class AstInternal;
template<std::size_t N> class AstFixSize;
class AstList;
class AstListBop;



///
/// \brief Abstract syntax tree base class
///
class Ast
{
public:
    /**
     * @brief type of a node in the language tree
     */
    enum class Type {
        // LIST, sync with isList() definition
        CLASS_LIST, MEMBER_LIST, DECL_PARAM_LIST, STMT_LIST,
        IF_LIST, TRY_LIST,
        DECTOR_LIST, ARG_LIST, NAME_LIST,

        // BOP-LIST, sync with isBopList() and isList() definition
        DOT_BOP_LIST, MUL_BOP_LIST, ADD_BOP_LIST,
        LOGIC_AND_BOP_LIST, LOGIC_OR_BOP_LIST,

        // FIX-SIZE, sync with isFixSize() definition
        // size 1
        RETURN, BREAK, CONTINUE, THROW,
        POST_INC, POST_DEC, PRE_INC, PRE_DEC,
        UNARY_PLUS, UNARY_MINUS, LOGIC_NOT, BIT_NOT,
        // size 2
        DECL_VAR, DECL_PARAM, WHILE, DO_WHILE, IF_CONDBODY,
        CATCH,
        CAST,
        LT, LEQ, GT, GEQ, INSTANCEOF, EQ, NEQ, SHL, SHR, SHRA,
        BIT_OR, BIT_XOR, BIT_AND,
        ASSIGN, ASS_ADD, ASS_SUB, ASS_MUL, ASS_DIV, ASS_MOD,
        ASS_AND, ASS_XOR, ASS_OR, ASS_SHL, ASS_SHR, ASS_SHRA,
        // size 3
        QUESTION, NEW_CLASS, NEW_ARRAY,
        // size 4
        DECL_CLASS, DECL_INTERFACE, FOR,
        // size 5
        DECL_METHOD,

        // SCALAR, isScalar() := !isList() && !isFixSize()
        IDENT, NUMBER, STRING, META, HIDDEN
    };

    Ast(Type t);
    virtual ~Ast() = default;

    Ast(const Ast &copy) = delete;
    Ast &operator=(const Ast &assign) = delete;

    Ast(Ast &&move) = default;
    Ast &operator=(Ast &&moveAssign) = default;

    virtual void dump() const;

    static bool isList(Type mType);
    static bool isBopList(Type mType);
    static bool isFixSize(Type mType, size_t s = 0);
    static bool isScalar(Type mType);
    bool isList() const;
    bool isBopList() const;
    bool isFixSize(size_t s = 0) const;
    bool isScalar() const;

    AstScalar &asScalar();
    const AstScalar &asScalar() const;
    AstInternal &asInternal();
    const AstInternal &asInternal() const;
    template<std::size_t N> AstFixSize<N> &asFixSize();
    template<std::size_t N> const AstFixSize<N> &asFixSize() const;
    AstList &asList();
    const AstList &asList() const;
    AstListBop &asBopList();
    const AstListBop &asBopList() const;

    std::unique_ptr<AstList> bodify();
    static int precedence(Type mType);
    int precedence() const;

    ///
    /// \brief Deep copy
    /// \return Ast with same value, but parent is null
    ///
    virtual std::unique_ptr<Ast> clone() const = 0;

    Type getType() const;
    AstInternal &getParent() const;
    void setParent(util::Observer<AstInternal *> next);

    bool hasIndent() const;
    int indentLevel() const;

private:
    Type mType;
    util::Observer<AstInternal *> mParent;
};



} // namespace zuse



#endif // ZUSE_AST_H
