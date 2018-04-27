#ifndef ZUSE_AST_LIST_BOP_H
#define ZUSE_AST_LIST_BOP_H

#include "ast_list.h"
#include "bop.h"



namespace zuse
{



///
/// \brief Binary operator list abstract syntax tree node
///
class AstListBop : public AstList
{
public:
    AstListBop(Ast::Type t, std::unique_ptr<Ast> lhs, std::unique_ptr<Ast> rhs, Bop op);
    AstListBop(std::unique_ptr<Ast> name, int dims);

    AstListBop(Ast::Type t, util::Owner<Ast *> lhs, util::Owner<Ast *> rhs, Bop op);
    AstListBop(util::Owner<Ast *> name, int dims);

    AstListBop(AstListBop &&move) = default;
    AstListBop &operator=(AstListBop &&moveAssign) = default;

    static std::unique_ptr<AstListBop> makeDims(std::unique_ptr<Ast> a);
    static std::unique_ptr<AstListBop> makeDims(int dims);
    void addDims(std::unique_ptr<Ast> a);
    void addDims(int dims);

    void dump() const override;
    std::unique_ptr<Ast> clone() const override;
    std::unique_ptr<Ast> remove(size_t pos) override;
    void clear() override;

    Bop opAt(size_t pos) const;
    void setOpAt(size_t pos, Bop op);
    int numOp() const;

protected:
    void doInsert(size_t pos, std::unique_ptr<Ast> child) override;

private:
    AstListBop(Ast::Type t);

    void init(Type t, std::unique_ptr<Ast> lhs, std::unique_ptr<Ast> rhs, Bop op);
    void initDotList(std::unique_ptr<Ast> name, int dims);

    void mergeIn(AstListBop &t, Bop lead = Bop::DEFAULT);
    bool isLeftAssoc() const;
    bool isRightAssoc() const;

private:
    std::vector<Bop> mOps;
};



} // namespace zuse



#endif // ZUSE_AST_LIST_BOP_H
