#include "ast.h"
#include "ast_scalar.h"
#include "ast_fix_size.h"
#include "ast_internal.h"
#include "ast_list.h"
#include "ast_list_bop.h"

#include <cassert>



namespace zuse
{



Ast::Ast(Type t) :
    mType(t),
    mParent(nullptr)
{
}

///
/// \brief Dump for debugging
///
void Ast::dump() const
{
    // do nothing
}

bool Ast::isList(Ast::Type type)
{
    return Type::CLASS_LIST <= type && type <= Type::LOGIC_OR_BOP_LIST;
}

bool Ast::isBopList(Ast::Type type)
{
    return Type::DOT_BOP_LIST <= type && type <= Type::LOGIC_OR_BOP_LIST;
}

bool Ast::isFixSize(Ast::Type type, size_t s)
{
    switch (s) {
    case 0:
        return Type::RETURN <= type && type <= Type::DECL_METHOD;
    case 1:
        return Type::RETURN <= type && type <= Type::BIT_NOT;
    case 2:
        return Type::DECL_VAR <= type && type <= Type::ASS_SHRA;
    case 3:
        return Type::QUESTION <= type && type <= Type::NEW_ARRAY;
    case 4:
        return Type::DECL_CLASS <= type && type <= Type::FOR;
    case 5:
        return Type::DECL_METHOD <= type && type <= Type::DECL_METHOD;
    default:
        return false;
    }
}

bool Ast::isScalar(Ast::Type type)
{
    return !isList(type) && !isFixSize(type);
}

///
/// \return True if this node is of list class
///
bool Ast::isList() const
{
    return isList(mType);
}

///
/// \return True if this node is of binary operator list class
///
bool Ast::isBopList() const
{
    return isBopList(mType);
}

///
/// \return True if this node is of fix-sized class
///
bool Ast::isFixSize(size_t s) const
{
    return isFixSize(mType, s);
}

///
/// \return True if this node is of scalar class
///
bool Ast::isScalar() const
{
    return isScalar(mType);
}

///
/// \brief Type assertion of scalar node
///
AstScalar &Ast::asScalar()
{
    assert(isScalar());
    return static_cast<AstScalar&>(*this);
}

///
/// \brief Type assertion of scalar node
///
const AstScalar &Ast::asScalar() const
{
    assert(isScalar());
    return static_cast<const AstScalar&>(*this);
}

///
/// \brief Type assertion of internal node
///
AstInternal &Ast::asInternal()
{
    assert(!isScalar());
    return static_cast<AstInternal&>(*this);
}

///
/// \brief Type assertion of internal node
///
const AstInternal &Ast::asInternal() const
{
    assert(!isScalar());
    return static_cast<const AstInternal&>(*this);
}

///
/// \brief Type assertion of fix-sized node
/// \tparam N number of subtrees
///
template<std::size_t N>
AstFixSize<N> &Ast::asFixSize()
{
    assert(isFixSize(N));
    return static_cast<AstFixSize<N>&>(*this);
}

///
/// \brief Type assertion of fix-sized node
/// \tparam N number of subtrees
///
template<std::size_t N>
const AstFixSize<N> &Ast::asFixSize() const
{
    assert(isFixSize(N));
    return static_cast<const AstFixSize<N>&>(*this);
}

///
/// \brief Type assertion of list node
///
AstList &Ast::asList()
{
    assert(isList());
    return static_cast<AstList&>(*this);
}

///
/// \brief Type assertion of list node
///
const AstList &Ast::asList() const
{
    assert(isList());
    return static_cast<const AstList&>(*this);
}

///
/// \brief Type assertion of binary operator list node
///
AstListBop &Ast::asBopList()
{
    assert(isBopList());
    return static_cast<AstListBop&>(*this);
}

///
/// \brief Type assertion of binary operator list node
///
const AstListBop &Ast::asBopList() const
{
    assert(isBopList());
    return static_cast<const AstListBop&>(*this);
}

///
/// \brief Make a statement list node from this statement node
/// \return A new statement list
///
std::unique_ptr<AstList> Ast::bodify()
{
    if (getType() == Type::STMT_LIST)
        return std::unique_ptr<AstList>(&clone().release()->asList());

    // nest with a stmt list, who takes over ownership of 'this'
    auto res = std::make_unique<AstList>(Type::STMT_LIST);
    res->append(clone());
    return res;
}

///
/// \return Precedence weight of the given type,
///         greater number indicates higher precedence
///
int Ast::precedence(Ast::Type type)
{
    switch (type) {
    case Type::DOT_BOP_LIST:
    case Type::POST_INC:
    case Type::POST_DEC:
        return 15;
    case Type::PRE_INC:
    case Type::PRE_DEC:
    case Type::UNARY_PLUS:
    case Type::UNARY_MINUS:
    case Type::LOGIC_NOT:
    case Type::BIT_NOT:
        return 14;
    case Type::NEW_CLASS:
    case Type::NEW_ARRAY:
    case Type::CAST:
        return 13;
    case Type::MUL_BOP_LIST:
        return 12;
    case Type::ADD_BOP_LIST:
        return 11;
    case Type::SHL:
    case Type::SHR:
    case Type::SHRA:
        return 10;
    case Type::LT:
    case Type::LEQ:
    case Type::GT:
    case Type::GEQ:
    case Type::INSTANCEOF:
        return 9;
    case Type::EQ:
    case Type::NEQ:
        return 8;
    case Type::BIT_AND:
        return 7;
    case Type::BIT_XOR:
        return 6;
    case Type::BIT_OR:
        return 5;
    case Type::LOGIC_AND_BOP_LIST:
        return 4;
    case Type::LOGIC_OR_BOP_LIST:
        return 3;
    case Type::QUESTION:
        return 2;
    case Type::ASSIGN:
    case Type::ASS_ADD:
    case Type::ASS_SUB:
    case Type::ASS_MUL:
    case Type::ASS_DIV:
    case Type::ASS_MOD:
    case Type::ASS_AND:
    case Type::ASS_XOR:
    case Type::ASS_OR:
    case Type::ASS_SHL:
    case Type::ASS_SHR:
    case Type::ASS_SHRA:
        return 1;
    default:
        return 0;
    }
}

///
/// \return Precedence of this node
///
int Ast::precedence() const
{
    return precedence(mType);
}

///
/// \return Node type of this node
///
Ast::Type Ast::getType() const
{
    return mType;
}

///
/// \return Parent of this node. If no parent, cause assertion failure.
///
AstInternal &Ast::getParent() const
{
    return *mParent;
}

///
/// \brief Set the parent of this node
///
void Ast::setParent(util::Observer<AstInternal *> next)
{
    mParent = next;
}

///
/// \return True of subtrees of this node should be indented
///
bool Ast::hasIndent() const
{
    Type ptype = mParent->mType;
    return ptype == Type::MEMBER_LIST || ptype == Type::STMT_LIST;
}

///
/// \return Indentation level of this node
///
int Ast::indentLevel() const
{
    int ret = 0;
    for (const Ast *a = mParent;
         a->mParent != a && a->mParent != nullptr;
         a = &a->getParent())
        if (a->getType() == Type::STMT_LIST
                || a->getType() == Type::MEMBER_LIST)
            ++ret;
    return ret;
}

// instanciation
template AstFixSize<1> &Ast::asFixSize();
template AstFixSize<2> &Ast::asFixSize();
template AstFixSize<3> &Ast::asFixSize();
template AstFixSize<4> &Ast::asFixSize();
template AstFixSize<5> &Ast::asFixSize();

// instanciation
template const AstFixSize<1> &Ast::asFixSize() const;
template const AstFixSize<2> &Ast::asFixSize() const;
template const AstFixSize<3> &Ast::asFixSize() const;
template const AstFixSize<4> &Ast::asFixSize() const;
template const AstFixSize<5> &Ast::asFixSize() const;



} // namespace zuse
